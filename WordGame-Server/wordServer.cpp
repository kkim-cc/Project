/* 
 * Filename: wordServer.c
 * Author:   KIDUCK KIM(DUCKIE)
 * Date:     Apr. 04/14
 * Description: This file is an assignment of a SAD Assignment #4
 *              This program is the main server file of Word Games
 */

#define _REENTRANT

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>	// file constrol start
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <fstream> 		// file control end
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <signal.h> 	// signal 
#include <sys/wait.h>
#include <sys/ipc.h>	// semaphore
#include <sys/sem.h> 	// semaphore

#include <string>
#include <map>
#include <vector>
#include <algorithm> // remove_if
#include <cctype>	 // isspace

using namespace std;

#define MAX_WORD_GAME		10
#define MAX_GAME_USER		60

/*
 * prototypes
 */

/*
 * 1. install g++ - done.
 * 2. using STL
 *    - for WordAnswer. and user's answerList (?choose STL)
 * 
 * 3. Create Structure - 30 user
 * 4. Create User Threads
 * 5. Using Semaphore - tournament scores
 * 6. Uses signnal to check time..
 * 7. Multiple games..
 * 8. Process disconnects when user disconnects 
 */
typedef struct tag_quiz
{
	string 			strQuestion;
	vector<string> 	mm; 
	int 			nMaxScore;
}QUIZ;

typedef struct tag_User
{
	int 			nClientSocketId;
	string 			strUserIp;
	vector<string> 	aa;
	int				nScore;
	int				nRank;
	int 			nSendQuiz;
}USER;

typedef struct tag_WordGame
{
	int				nTimeLimit;
	QUIZ			pQuiz[MAX_WORD_GAME];
	int				nCurQuiz;
	vector<USER*>	vUser;
	int				nTimeWait;
	int				nMaxQuiz;
	int				nGameTime;
	int 			nSemId;
	int 			nGameOver;
	pthread_t		tid[MAX_GAME_USER];
	pthread_t		cid; // connection thread
	int 			server_socket;
}WORDGAME;

#include "../inc/protocol.h"

static WORDGAME		wordGame;

int main (int argc, char *argv[])
{
	USER			tUser = {0}, *pUser;
	int 			nTimeLimit = 120; // default is 2 minutes
	QUIZ			*pCurQuiz = NULL;
	int 			nRet = 0;
	int				server_socket, client_socket;
	socklen_t		client_len;
	struct sockaddr_in client_addr, server_addr;
	char 			buffer[BUFSIZ] = "";
	pthread_t 		tid[MAX_GAME_USER] = {0};
	int				nCurGameUser = 0;
	void 			*res;
	int				k = 0;
	vector<USER*>::reverse_iterator uItr;
	
	initializeWordGame();
	
	// arg1 => quize file
	// arg2 => timelimit(seconds)
	if( argc != 3)
	{
		printf("Usage : %s <Quiz file><Game times(seconds)>\n", argv[0]);
		fflush(stdout);
		return -1;
	}
	
	nTimeLimit = atoi(argv[2]);
	if( nTimeLimit < 60 || nTimeLimit > 180 )
	{
		printf("Usage : %s <Quiz file><Game times(seconds)>\nGame times should be between 60 ~ 180 secs\n", argv[0]);
		fflush(stdout);
		return -1;
	}
	wordGame.nGameTime = nTimeLimit;
	
	nRet = load_file(argv[1]);
	if( nRet < 0 )
	{
		printf("ERR: load_file !!!\n");
		fflush(stdout);
		return -1;
	}
	
	printf ("(SEMAPHORE) Obtaining semaphores for use ...\n");
	fflush(stdout);
	wordGame.nSemId = semget (IPC_PRIVATE, 1, IPC_CREAT | 0666);

	if (wordGame.nSemId == -1)
	{
		printf ("(SEMAPHORE) GRRRRR ... couldn't get semaphores!\n");
		fflush(stdout);
		return (1);
	}

	printf ("(SEMAPHORE) Our semID is %d\n", wordGame.nSemId);
	fflush(stdout);

	printf ("(SEMAPHORE) Will initialize the semaphore to a known value ...\n");
	fflush(stdout);
	if (semctl (wordGame.nSemId, 0, SETALL, init_values) == -1) 
	{
		printf ("(SEMAPHORE) GRRRRR... Can't initialize semaphores\n");
		fflush(stdout);
		return (1);
	}
	

	/*
	* obtain a socket for the server
	*/
	printf ("[SERVER] : Obtaining STREAM Socket ...\n");
	fflush(stdout);
	if ((server_socket = socket (AF_INET, SOCK_STREAM, 0)) < 0) 
	{
		printf ("[SERVER] : Server Socket.getting - FAILED\n");
		fflush(stdout);
		return 1;
	}

	/*
	* initialize our server address info for binding purposes
	*/
	memset (&server_addr, 0, sizeof (server_addr));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl (INADDR_ANY);
	server_addr.sin_port = htons (PORT);

	printf ("[SERVER] : Binding socket to server address ...\n");
	fflush(stdout);
	if (bind (server_socket, (struct sockaddr *)&server_addr, sizeof (server_addr)) < 0) 
	{
		printf ("[SERVER] : Binding of Server Socket - FAILED\n");
		close (server_socket);
		fflush(stdout);
		return 2;
	}

	/*
	* start listening on the socket
	*/
	printf ("[SERVER] : Begin listening on socket for incoming message ...\n");
	fflush(stdout);
	if (listen (server_socket, 5) < 0) 
	{
		printf ("[SERVER] : Listen on Socket - FAILED.\n");
		close (server_socket);
		fflush(stdout);
		return 3;
	}

	wordGame.server_socket = server_socket;

	/*
	* install a signal handler for SIGCHILD (sent when the child terminates)
	*/
	printf ("[Signal] Installing signal handler to manage processes ...\n");
	fflush(stdout);
	signal (SIGALRM, alarmHandler);
	alarm (1);
	
	/*
	* for this server, run an endless loop that will
	* accept incoming requests from a remote client.
	* the server will fork a child copy of itself to handle the
	* request, and the parent will continue to listen for the
	* next request
	*/
	while (wordGame.nGameOver == 0) 
	{
		if (wordGame.nTimeLimit > 0)
		{
			// running game.. don't accept user connection
			// sleep process...
			sleep(1);
		}
		else if (wordGame.nGameOver == 0)
		{
			/*
			 * accept a packet from the client

			 client_len = sizeof (client_addr);
			if ((client_socket = accept (server_socket,(struct sockaddr *)&client_addr, &client_len)) < 0) 
			{
				printf ("[SERVER] : Accept Packet from Client - FAILED\n");
				close (server_socket);
				fflush(stdout);
				return 4;
			}
			
			// Clear out the Buffer 
			memset(buffer,0,BUFSIZ);
			
			printf("[SERVER] : Incoming CLIENT connection on SOCKET %d\n", client_socket);
			fflush(stdout);
			
			initializeUser(&tUser);
			tUser.nClientSocketId = client_socket;
			tUser.strUserIp = inet_ntoa(client_addr.sin_addr);
			wordGame.vUser.push_back(tUser);
			
			for (uItr = wordGame.vUser.rbegin(); uItr != wordGame.vUser.rend(); uItr++)
			{
				pUser = (USER *)(&(*uItr));
				if (pUser->nClientSocketId == tUser.nClientSocketId)
				{
					break;
				}
			}
			
			if (pthread_create (&tid[nCurGameUser], NULL, process_client, (void *)pUser) != 0) 
			{
				printf ("Arghh !! I cannot start thread #%d\n", nCurGameUser);
				fflush(stdout);
				//exit (2);
			}
			else
			{
				printf("[Thread] %lu / %s Thread created (%d/%d)\n", 
					   tid[nCurGameUser], tUser.strUserIp.c_str(),
					   nCurGameUser, MAX_GAME_USER);
				fflush(stdout);
			}
			nCurGameUser++;
			 */
			sleep(1);
		}
	}

	LockSemaphore();

	// close socket
	printf("[Server] Close all sockets...\n");
	fflush(stdout);
	strcpy(buffer, "gq:game quit");
	k = strlen(buffer);
	vector<USER*>::iterator iter;
	for (iter = wordGame.vUser.begin(); iter != wordGame.vUser.end(); iter++)
	{
		pUser = (USER*)(*iter);
		if (pUser->nClientSocketId > 0)
		{
			send (pUser->nClientSocketId, buffer, k, 0);
			close(pUser->nClientSocketId);
		}
		pUser->nClientSocketId = 0;
	}
	close(server_socket);

	UnLockSemaphore();
	
	printf("[Server] Cancel all threads...\n");
	fflush(stdout);
	for( k = 0; k < MAX_GAME_USER; k++ )
	{
		if( wordGame.tid[k] != 0 )
		{
			pthread_cancel(wordGame.tid[k]);
		}
	}
	for( k = 0; k < MAX_GAME_USER; k++ ) 
	{
		if( wordGame.tid[k] != 0 )
		{
			pthread_join (wordGame.tid[k], &res);
			if (res == PTHREAD_CANCELED)
			{
				printf("[Thread] %lu Thread canceled\n", tid[k]);
				fflush(stdout);
			}
		}
	}
	printf ("(SEMAPHORE) Release the semaphores\n");
	fflush(stdout);
	semctl (wordGame.nSemId, 0, IPC_RMID, 0);
	
	clear_global();
	printf("Server Terminated...\n");
	fflush(stdout);
	return 0;
}

int load_file(char * strFile)
{
	int nRet = 0, nCount = 0, nQuiz=0;
	string line;
	ifstream myfile (strFile);
	if (myfile.is_open())
	{
		while ( !myfile.eof() )
		{
			getline (myfile,line);
			
			//line.erase(std::remove_if(line.begin(), line.end(), '\r'), line.end());
			line.erase(remove(line.begin(), line.end(), '\r'), line.end());
			
			if( line.length() < 3 )
			{
				nCount ++;
				nQuiz = 0;
				if (nCount >= MAX_WORD_GAME)
				{
					// error!!!
					printf("Max Quiz is %d. Therefore, I read all buffers..\n", MAX_WORD_GAME);
					fflush(stdout);
					break;
				}
				printf("[Load File] getline size is zero[%d] %s\n", nCount, line.c_str());
				fflush(stdout);
			}
			else if( nQuiz == 0 )
			{
				wordGame.pQuiz[nCount].strQuestion = line;
				wordGame.pQuiz[nCount].nMaxScore = 0;
				nQuiz ++;
//				printf("[Load File] getline header:%s\n", line.c_str());
//				fflush(stdout);
			}
			else
			{
				wordGame.pQuiz[nCount].mm.push_back(line);
				wordGame.pQuiz[nCount].nMaxScore += line.length();
//				printf("[Load File] getline answer:%s\n", line.c_str());
			}
		}
	    myfile.close();
		printf("[Load File] MaxQuiz = %d\n", nCount);
		fflush(stdout);
		wordGame.nMaxQuiz = nCount;
	}
    else nRet = -1; 
	
	return nRet;
}

void* process_connection(void *vData)
{
	USER 			*pUser, *pTmpUser;
	socklen_t 		client_len = 0;
	int 			client_socket = 0;
	int 			server_socket = wordGame.server_socket;
	struct sockaddr_in client_addr;
	char 			buffer[BUFSIZ] = "";
	pthread_t 		tid[MAX_GAME_USER] = {0};
	int				nCurGameUser = 0;
	void			*res;
	int				k = 0;
	vector<USER*>::reverse_iterator uItr;
	
	
	while(wordGame.nGameOver == 0)
	{
		/*
			* accept a packet from the client
			*/
		client_len = sizeof (client_addr);
		if ((client_socket = accept (server_socket,(struct sockaddr *)&client_addr, &client_len)) < 0) 
		{
			printf ("[SERVER] : Accept Packet from Client - FAILED\n");
			close (server_socket);
			fflush(stdout);
			return NULL;
		}
		
		/* Clear out the Buffer */
		memset(buffer,0,BUFSIZ);
		
		printf("[SERVER] : Incoming CLIENT connection on SOCKET %d\n", client_socket);
		fflush(stdout);
		
		LockSemaphore();

		nCurGameUser = wordGame.vUser.size();
		pUser = new USER;
		initializeUser(pUser);
		pUser->nClientSocketId = client_socket;
		pUser->strUserIp = inet_ntoa(client_addr.sin_addr);
		wordGame.vUser.push_back(pUser);
		
		for (uItr = wordGame.vUser.rbegin(); uItr != wordGame.vUser.rend(); uItr++)
		{
			pTmpUser = (USER *)(*uItr);
			if (pUser->nClientSocketId == pTmpUser->nClientSocketId)
			{
				break;
			}
		}
		UnLockSemaphore();

		if (pthread_create (&(wordGame.tid[nCurGameUser]), NULL, process_client, (void *)pUser) != 0) 
		{
			printf ("Arghh !! I cannot start thread #%d\n", nCurGameUser);
			fflush(stdout);
			//exit (2);
		}
		else
		{
			printf("[Thread] %lu / %s Thread created (%d/%d)\n", 
					wordGame.tid[nCurGameUser], pUser->strUserIp.c_str(),
					nCurGameUser, MAX_GAME_USER);
			fflush(stdout);
		}
		
	}
	return NULL;
}

void* process_client (void *vData)
{
	USER 	*pUser = (USER *)vData;
	char 	buffer[BUFSIZ];
	vector<string>::iterator itr;
	string 	strWords, strAns;
	int 	nAnswerStatus = 0, nRetRcv=0;
	
	printf("[THREAD] Starting[%s, %d]...\n", 
		   pUser->strUserIp.c_str(), pUser->nClientSocketId);
	printf("[THREAD] Quiz[cur:%d, max:%d] check...\n", 
		   wordGame.nCurQuiz, wordGame.nMaxQuiz);
	fflush(stdout);
	while (wordGame.nGameOver == 0)
	{
		// close.. when all games ended
		
		if (wordGame.nCurQuiz >= wordGame.nMaxQuiz)
		{
			printf("[THREAD] Quiz[cur:%d, max:%d] quit \n", 
				   wordGame.nCurQuiz, wordGame.nMaxQuiz);
			fflush(stdout);
			break;
		}
		// read user's data..
		memset(buffer,0,BUFSIZ);
        nRetRcv = recv (pUser->nClientSocketId, buffer, BUFSIZ, 0);
        if (nRetRcv <= 0)
        {
			break;
		}
		
		// check and parse..
		nAnswerStatus = 0;
		strWords = buffer;
		
		printf("[THREAD] %s - %s -- ", pUser->strUserIp.c_str(), strWords.c_str());
		fflush(stdout);
		
		if (wordGame.nTimeLimit == 0)
		{
			sprintf(buffer, "go:%s|", strWords.c_str());
			send(pUser->nClientSocketId, buffer, BUFSIZ, 0);
			printf("timeout\n");
			fflush(stdout);
			continue;
		}
		
		//LockSemaphore();

		// check the word user input is right.
		for (itr = wordGame.pQuiz[wordGame.nCurQuiz].mm.begin(); 
			 itr != wordGame.pQuiz[wordGame.nCurQuiz].mm.end(); itr ++)
		{
			strAns = (string)(*itr);

			if (strWords == strAns)
			{
				nAnswerStatus = 1;
				break;
			}
		}
		
		if (nAnswerStatus == 0) // send client to wrong word
		{
			sprintf(buffer, "wr:%s|", strWords.c_str());
			send(pUser->nClientSocketId, buffer, BUFSIZ, 0);
			printf("wrong\n");
			fflush(stdout);
		}
		else // check user's current words list
		{
			for (itr = pUser->aa.begin(); itr != pUser->aa.end(); itr ++)
			{
				strAns = (string)(*itr);
				if (strWords == strAns)
				{
					nAnswerStatus = 2;
					break;
				}
			}
			if ( nAnswerStatus == 2 ) // send client to duplicate word
			{
				sprintf(buffer, "du:%s|", strWords.c_str());
				send(pUser->nClientSocketId, buffer, BUFSIZ, 0);
				printf("duplicate\n");
				fflush(stdout);
			}
			else // insert user's word list and send user to message 
			{
				pUser->aa.push_back(strWords);
				sprintf(buffer, "in:%s|", strWords.c_str());
				send(pUser->nClientSocketId, buffer, BUFSIZ, 0);
				printf("inserted\n");
				fflush(stdout);
				pUser->nScore += strWords.length();
				//pUser->nScore = pUser->aa.size();
				SendClientSubMessage(pUser, 0);
			}
		}
		//UnLockSemaphore();
	}
	
	printf("[***THREAD] Terminated [%s, %d]...\n", pUser->strUserIp.c_str(),
			pUser->nClientSocketId);
	fflush(stdout);
	if (nRetRcv < 0)
		close(pUser->nClientSocketId);
	pUser->nClientSocketId = 0;
	return NULL;
}

void initializeWordGame(void)
{
	wordGame.nCurQuiz = 0;
	wordGame.nTimeLimit = 0;
	wordGame.nTimeWait = DEFAULT_TIME_WAIT;
	wordGame.nMaxQuiz = 0;
	wordGame.nGameTime = 0;
	wordGame.nSemId = 0;
	wordGame.nGameOver = 0;
	for (int k = 0; k < MAX_GAME_USER; k++)
	{
		wordGame.tid[k] = 0;
	}
	wordGame.cid = 0;
	wordGame.server_socket = 0;
}

void initializeUser(USER *pUser)
{
	pUser->nClientSocketId = 0;
	pUser->strUserIp = "";
	pUser->nScore = 0;
	pUser->nRank = 0;
	pUser->nSendQuiz = 0;
}

void initializeAllUser(int nTime)
{
	USER *pUser = NULL;
	vector<USER*>::iterator viter;
	for(viter = wordGame.vUser.begin(); viter != wordGame.vUser.end(); viter++)
	{
		pUser = (USER *)(*viter);
		// game start!! initialize all
		if (nTime == wordGame.nGameTime)
		{
			pUser->aa.clear();
			pUser->nRank = 0;
			pUser->nScore = 0;
			pUser->nSendQuiz = 0;
		}
		// timer setting 
	}
}

void SendClientSubMessage(USER *pUser, int nSendQuiz)
{
	int  	nLen = 0;
	char 	szMsg[BUFSIZ] = {0};
	
	// error checking
	if (pUser->nClientSocketId <= 0)
		return;
	
	// Rank / Total scores / Time remained information
	sprintf (szMsg, "ra:%03d/%03d|ts:%d|tr:%d|", 
			 pUser->nRank, (int)wordGame.vUser.size(), pUser->nScore,
			 wordGame.nTimeLimit);
	if (nSendQuiz > 0 && pUser->nSendQuiz == 0)
	{
		char szQuiz[MAX_USER_INPUT];
		// Time remained information
		sprintf (szQuiz, "qz:%s|ms:%d|", wordGame.pQuiz[wordGame.nCurQuiz].strQuestion.c_str(),
				 wordGame.pQuiz[wordGame.nCurQuiz].nMaxScore);
		strcat(szMsg, szQuiz);
		pUser->nSendQuiz = 1;
	}
	nLen = strlen(szMsg);
	send (pUser->nClientSocketId, szMsg, nLen, 0);	
}

void SendClientTimeMessage(int nSendQuiz)
{
	USER 	*pUser = NULL;
	
	vector<USER*>::iterator viter;
	for(viter = wordGame.vUser.begin(); viter != wordGame.vUser.end(); viter++)
	{
		pUser = (USER *)(*viter);
		// error checking
		if (pUser->nClientSocketId > 0)
		{
			SendClientSubMessage(pUser, nSendQuiz);
		}
	}
}

void clear_global(void)
{
	USER *pUser = NULL;
	vector<USER*>::iterator viter;
	
	// clear user's word list
	for(viter = wordGame.vUser.begin(); viter != wordGame.vUser.end(); viter++)
	{
		pUser = (USER *)(*viter);
		pUser->aa.clear();
		delete pUser;
	}
	wordGame.vUser.clear();
	
	// clear word list
	for(int k = 0; k < MAX_WORD_GAME; k++)
	{
		wordGame.pQuiz[k].mm.clear();
	}
}

void checkRank(void)
{
	// Make Rank...
	multimap<int,int> 			mm;
	multimap<int,int>::reverse_iterator iter;
	USER 						*pUser = NULL;
	vector<USER*>::iterator 	viter;
	int 						nUser = 0, nRank = 1, nCurRank = 1, nScore = -1;
	
	// get user scores and save to multimap
	for (viter = wordGame.vUser.begin(); viter != wordGame.vUser.end(); viter++)
	{
		
		pUser = (USER *)(*viter);
		mm.insert(make_pair(pUser->nScore, nUser));
		nUser ++;
	}

	// get users ranking using multimap reverse search
	for (iter = mm.rbegin(); iter != mm.rend(); iter++)
	{
		nUser = (*iter).second;
		pUser = (USER *)(wordGame.vUser[nUser]);
		if (nScore == -1)
		{
			nScore = pUser->nScore;
			pUser->nRank = nCurRank;
			nRank ++;
		}
		else if (nScore == pUser->nScore)
		{
			pUser->nRank = nCurRank;
			nRank ++;
		}
		else
		{
			nScore = pUser->nScore;
			nCurRank = nRank;
			pUser->nRank = nCurRank;
			nRank ++;
		}
	}

	mm.clear();	
}

void alarmHandler(int signal_number)
{
	if (wordGame.nCurQuiz >= wordGame.nMaxQuiz)
	{
		// quit game
		wordGame.nGameOver = 1;
		printf("[ALARM] GameOver...\n");
		return;
	}
	else if (wordGame.nTimeWait > 0)
	{
		alarm (1);	// reset alarm
		if (wordGame.nTimeWait >= DEFAULT_TIME_WAIT)
		{
			printf ("%d seconds are remained to start next game!\n", 
					wordGame.nTimeWait);
			fflush(stdout);
			// create thread.
			if (pthread_create (&(wordGame.cid), NULL, process_connection, NULL) != 0) 
			{
				printf ("Arghh !! I cannot start thread #%lu\n", wordGame.cid);
				fflush(stdout);
				//exit (2);
			}
			else
			{
				printf("[Thread] %lu / Connection Thread created\n", wordGame.cid);
				fflush(stdout);
			}
		}
		else if(wordGame.nTimeWait < 5)
		{
			printf ("%d seconds are remained to start next game!\n", 
					wordGame.nTimeWait);
			fflush(stdout);
		}
		wordGame.nTimeWait --;
		
		
		if (wordGame.nTimeWait == 0)
		{
			LockSemaphore();
			// game start!!!
			// initialize all user's game.
			initializeAllUser(wordGame.nGameTime);
			// start timer setting
			wordGame.nTimeLimit = wordGame.nGameTime;
			// send message to all client
			SendClientTimeMessage(0);

			UnLockSemaphore();

			// stop thread.
			if (wordGame.cid != 0)
			{
				void	*res;
				pthread_cancel(wordGame.cid);
				pthread_join (wordGame.cid, &res);
				if (res == PTHREAD_CANCELED)
				{
					printf("[Thread] %lu Connection Thread canceled\n", wordGame.cid);
					fflush(stdout);
				}
			}
		}
	}
	else if (wordGame.nTimeLimit > 0)
	{
		alarm (1);	// reset alarm
		if (wordGame.nTimeLimit >= wordGame.nGameTime)
		{
			printf ("Game Starting!\n");
			fflush(stdout);
		}
				
		// Send Message...
		if (wordGame.nTimeLimit > 0)
			wordGame.nTimeLimit --;

		if (wordGame.nTimeLimit == 0)
		{
			printf("[ALARM] GameEnd[%d/%d]...\n", 
				   wordGame.nCurQuiz, wordGame.nMaxQuiz);
			
			print_log();
			
			
			wordGame.nCurQuiz ++;
			
			if (wordGame.nCurQuiz >= wordGame.nMaxQuiz)
			{
				wordGame.nGameOver = 1;
				printf("[ALARM] GameOver...\n");
				return;
			}
			else
			{
				// initialize the timer and close message.
				wordGame.nTimeWait = DEFAULT_TIME_WAIT;
			}
		}
		
		LockSemaphore();
		
		initializeAllUser(wordGame.nTimeLimit);
		if (wordGame.nTimeLimit > 0)
		{
			checkRank();
			SendClientTimeMessage(1);
		}
		
		UnLockSemaphore();
	}
	
	// reactivate signal handler for next time ...
	signal (signal_number, alarmHandler);
}

void print_log(void)
{
	// Make Rank...
	multimap<int,int> 			mm;
	multimap<int,int>::reverse_iterator iter;
	USER 						*pUser = NULL;
	vector<USER*>::iterator 	viter;
	int 						nUser = 0, nRank = 1, nCurRank = 1, nScore = -1;
	
	// get user scores and save to multimap
	for (viter = wordGame.vUser.begin(); viter != wordGame.vUser.end(); viter++)
	{
		
		pUser = (USER *)(*viter);
		mm.insert(make_pair(pUser->nScore, nUser));
		nUser ++;
	}

	// get users ranking using multimap reverse search
	for (iter = mm.rbegin(); iter != mm.rend(); iter++)
	{
		nUser = (*iter).second;
		pUser = (USER *)(wordGame.vUser[nUser]);
		if (nScore == -1)
		{
			nScore = pUser->nScore;
			pUser->nRank = nCurRank;
			nRank ++;
		}
		else if (nScore == pUser->nScore)
		{
			pUser->nRank = nCurRank;
			nRank ++;
			printf("\n");
			fflush(stdout);
		}
		else
		{
			nScore = pUser->nScore;
			nCurRank = nRank;
			pUser->nRank = nCurRank;
			nRank ++;
			printf("\n");
			fflush(stdout);
		}
		printf("[LOG] %s Rank:%d Score:%d\n", 
			   pUser->strUserIp.c_str(), pUser->nRank, pUser->nScore);
		fflush(stdout);
	}

	mm.clear();	

}


void LockSemaphore(void)
{
	/*
	 * attempt to decrement the semaphore
	 */
	if (semop (wordGame.nSemId, &acquire_operation, 1) == -1) 
	{
		printf ("     (LockSemaphore) AHHHHH... Can't start critical region\n");
		fflush(stdout);
	}
}
void UnLockSemaphore(void)
{
	/*
	 * attempt to increment the semaphore
	 */
	if (semop (wordGame.nSemId, &release_operation, 1) == -1) 
	{
		printf ("     (UnLockSemaphore) AHHHHH... Can't end critical region\n");
		fflush(stdout);
	}	
}