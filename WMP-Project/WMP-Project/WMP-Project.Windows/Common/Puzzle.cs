// FILE				: Puzzle.cs
// PROJECT			: Windows Programming(PROG2120) – Assignment 7: Windows Surface Programming
// PROGRAMMER(S)    : KiDuck Kim(DUCKIE), Robin Dagenais(ROB)
// FIRST VERSION	: NOV25-2014
// DESCRIPTION		: This source file contents the main logic for the puzzle game
//
using System;
using System.Collections.Generic;
using System.Linq;  
using System.Text;
using System.Threading.Tasks;
using Windows.Foundation;
using Windows.Graphics.Imaging;
using Windows.Storage;
using Windows.Storage.Streams;
using Windows.UI;
using Windows.UI.Xaml.Controls;
using Windows.UI.Xaml.Media;
using Windows.UI.Xaml.Media.Imaging;
using Windows.UI.Xaml.Shapes;
using Windows.Data.Json;

namespace WMP_Project.Common
{
    class Puzzle
    {
        public const int moveUp = 0x01;
        public const int moveDown = 0x20;
        public const int moveLeft = 0x40;
        public const int moveRight = 0x80;
        public const int nMaxPiece = 15;
        public const int nImagePiece = 440; // 110*4

        private const string nEmptyPieceKey = "nEmptyPiece";
        private const string bGameInitKey = "bGameInit";
        private const string bGameEndKey = "bGameEnd";
        private const string bgFileKey = "bgFile";
        private const string PieceKey = "Piece";
        
        private int nEmptyPiece = 15;
        private bool bGameInit = false;
        private List<Piece> pieces;
        private bool bGameEnd = false;
        private string bgFile = "";

		
		/// <summary>
		///  Default constructor 
		/// </summary>
        public Puzzle()
        {
            bGameInit = false;
            bGameEnd = false;
            pieces = new List<Piece>();
            bgFile = "";
        }

		
		/// <summary>
		///  Construct a puzzle piece using the JSON  data interchange format
		/// </summary>
        /// <param name="jsonString">string</param>
        public Puzzle(string jsonString) : this()
        {
            JsonObject jsonObject = JsonObject.Parse(jsonString);
            bgFile = jsonObject.GetNamedString(bgFileKey);
            bGameEnd = jsonObject.GetNamedBoolean(bGameEndKey);
            bGameInit = jsonObject.GetNamedBoolean(bGameInitKey);
            nEmptyPiece = (int)jsonObject.GetNamedNumber(nEmptyPieceKey);

            foreach (IJsonValue jsonValue in jsonObject.GetNamedArray(PieceKey, new JsonArray()))
            {
                if (jsonValue.ValueType == JsonValueType.Object)
                {
                    pieces.Add(new Piece(jsonValue.GetObject()));
                }
            }
            // start create image and setting...
        }

		
		/// <summary>
		///  Bundle up the data into the JSON object
		/// </summary>
        /// <returns>string</returns>		
        public string Stringify()
        {
            JsonArray jsonArray = new JsonArray();
            foreach (Piece m in pieces)
            {
                jsonArray.Add(m.ToJsonObject());
            }

            JsonObject jsonObject = new JsonObject();
            jsonObject[PieceKey] = jsonArray;
            jsonObject[nEmptyPieceKey] = JsonValue.CreateNumberValue(nEmptyPiece);
            jsonObject[bGameInitKey] = JsonValue.CreateBooleanValue(bGameInit);
            jsonObject[bGameEndKey] = JsonValue.CreateBooleanValue(bGameEnd);
            jsonObject[bgFileKey] = JsonValue.CreateStringValue(bgFile);

            return jsonObject.Stringify();
        }

		
		/// <summary>
		///  Reset the puzzle piece data
		/// </summary>		
        public void Clear()
        {
            bGameInit = false;
            if (pieces.Count > 0)
                pieces.Clear();
            nEmptyPiece = nMaxPiece;
        }

		
		/// <summary>
		///  Split the image into the puzzle pieces
		/// </summary>
        /// <param name="filename">string</param>
        /// <returns>Task</returns>		
        public async Task SplitImage(string filename)
        {
            Rectangle rc;
            Piece pp;

            // black brush..
            SolidColorBrush blackBrush = new SolidColorBrush();
            blackBrush.Color = Colors.Black;

            if (pieces.Count > 0)
                pieces.Clear();

            BitmapBounds bounds = new BitmapBounds();
            bounds.Height = 110;
            bounds.Width = 110;
            for (uint i = 0; i < 4; i++)
            {
                for (uint k = 0; k < 4; k++)
                {
                    if (i == 3 && k == 3)
                        break;

                    StorageFile file = await StorageFile.GetFileFromPathAsync(filename);
                    if (file == null)
                        return;

                    // create a stream from the file and decode the image
                    using (IRandomAccessStream fileStream = await file.OpenAsync(Windows.Storage.FileAccessMode.Read))
                    {
                        BitmapDecoder decoder = await BitmapDecoder.CreateAsync(fileStream);

                        // create a new stream and encoder for the new image
                        InMemoryRandomAccessStream ras = new InMemoryRandomAccessStream();
                        BitmapEncoder enc = await BitmapEncoder.CreateForTranscodingAsync(ras, decoder);

                        // convert the entire bitmap to a 440px by 440px bitmap
                        enc.BitmapTransform.ScaledHeight = nImagePiece;
                        enc.BitmapTransform.ScaledWidth = nImagePiece;

                        bounds.X = i * 110;
                        bounds.Y = k * 110;
                        enc.BitmapTransform.Bounds = bounds;
                        // write out to the stream
                        try
                        {
                            await enc.FlushAsync();
                        }
                        catch (Exception ex)
                        {
                            string s = ex.ToString();
                        }
                        ras.Seek(0);
                        // render the stream to the screen
                        // create bitmap image
                        WriteableBitmap bImg = new WriteableBitmap(110, 110);
                        bImg.SetSource(ras);
                    
                        //create rectangle & setting
                        rc = new Rectangle();
                        rc.Height = 114;
                        rc.Width = 114;    
                        // Set Rectangle's width and colour
                        rc.StrokeThickness = 2;
                        rc.Stroke = blackBrush;
                        // create image brush and setting image
                        ImageBrush ib = new ImageBrush();
                        ib.ImageSource = bImg;
                        // fill rectangle
                        rc.Fill = ib;

                        // create piece
                        pp = new Piece();
                        pp.NOrder = (int)(i*4 + k);
                        pp.OnePiece = rc;

                        pieces.Add(pp);
                    }
                }
            }

            // process LAST piece
            //create rectangle & setting
            rc = new Rectangle();
            rc.Height = 114;
            rc.Width = 114;
            // Set Rectangle's width and colour
            rc.StrokeThickness = 2;
            rc.Stroke = blackBrush;
            
            // fill rectangle
            rc.Fill = blackBrush;

            // create LAST piece
            pp = new Piece();
            pp.NOrder = nMaxPiece;
            pp.OnePiece = rc;

            pieces.Add(pp);

            bGameInit = true;
        }

		
		/// <summary>
		///  Ready the game for first use
		/// </summary>
        /// <param name="filename">string</param>
        /// <returns>Task</returns>		
        public async Task InitializeGame(string filename)
        {
            if(string.IsNullOrEmpty(filename))
            {
                MainPage.Current.NotifyUser("Please click Load/Capture Image first.", 
                    MainPage.NotifyType.ErrorMessage);
                return;
            }
            // 1. split image 110*110 and assign to list
            await SplitImage(filename);
            
            if(bGameInit)
            {
                bgFile = filename;
                // 2. assign rectangle
                Shuffle();
            }
        }

		
		/// <summary>
		///  Organize the puzzle pieces into a list<>
		/// </summary>
        /// <returns>List<Rectangle></returns>		
        public List<Rectangle> GetList()
        {
            List<Rectangle> lstRect = new List<Rectangle>();

            for (int i = 0; i <= nMaxPiece; i++)
            {
                lstRect.Add(pieces[i].OnePiece);
            }
            return lstRect;
        }

		
		/// <summary>
		///  Place pieces into a temporary rectangle
		/// </summary>
        /// <param name="nId">int</param>
        /// <returns>Rectangle</returns>		
        public Rectangle GetRect(int nId)
        {
            Rectangle temp = new Rectangle();
            if(nId <= nMaxPiece)
            {
                temp = pieces[nId].OnePiece;
            }
            return temp;
        }

		
		/// <summary>
		///  Retrieve the piece ID
		/// </summary>
        /// <param name="nOrder">int</param>
        /// <returns>int</returns>		
        public int GetIdPiece(int nOrder)
        {
            int nId = -1;

            if (nOrder >= pieces.Count)
            {
                return nId;
            }

            nId = pieces[nOrder].NOrder;
            return nId;
        }

		
		/// <summary>
		///  MIx up the image segments 
		/// </summary>		
        private void Shuffle()
        {
            Random rand = new Random((int)DateTime.Now.Ticks & 0x0000FFFF);
            int n = pieces.Count;
            while (n > 1)
            {
                n--;
                int k = rand.Next(n + 1);
                Piece value = pieces[k];
                pieces[k] = pieces[n];
                pieces[n] = value;
            }
            for (int i = 0; i < pieces.Count; i++)
            {
                if(pieces[i].NOrder == nMaxPiece)
                {
                    nEmptyPiece = i;
                    break;
                }
            }
        }
        
		
		/// <summary>
		///  Move a puzzle segment down
		/// </summary>
        /// <param name="nId">int</param>
        /// <returns>bool</returns>		
        private bool MoveDown(int nId)
        {
            bool bRet = false;

            if ((nId + 1 == nEmptyPiece) && (nId % 4 != 3))
            {
                // down
                Piece temp;
                temp = pieces[nId + 1];
                pieces[nId + 1] = pieces[nId];
                pieces[nId] = temp;
                nEmptyPiece = nId;
                bRet = true;
            }

            return bRet;
        }

		
		/// <summary>
		///   Move a puzzle segment up
		/// </summary>
        /// <param name="nId">int</param>
        /// <returns>bool</returns>		
        private bool MoveUp(int nId)
        {
            bool bRet = false;

            if ((nId - 1 == nEmptyPiece) && (nId % 4 != 0))
            {
                // up
                Piece temp;
                temp = pieces[nId - 1];
                pieces[nId - 1] = pieces[nId];
                pieces[nId] = temp;
                nEmptyPiece = nId;
                bRet = true;
            }

            return bRet;
        }

		
		/// <summary>
		///   Move a puzzle segment left
		/// </summary>
        /// <param name="nId">int</param>
        /// <returns>bool</returns>		
        private bool MoveLeft(int nId)
        {
            bool bRet = false;

            if ((nId - 4 == nEmptyPiece) && (nId >= 4))
            {
                // left
                Piece temp;
                temp = pieces[nId - 4];
                pieces[nId - 4] = pieces[nId];
                pieces[nId] = temp;
                nEmptyPiece = nId;
                bRet = true;
            }

            return bRet;
        }

		
		/// <summary>
		///   Move a puzzle segment right
		/// </summary>
        /// <param name="nId">int</param>
        /// <returns>bool</returns>		
        private bool MoveRight(int nId)
        {
            bool bRet = false;

            if ((nId + 4 == nEmptyPiece) && (nId <= 12))
            {
                // right
                Piece temp;
                temp = pieces[nId + 4];
                pieces[nId + 4] = pieces[nId];
                pieces[nId] = temp;
                nEmptyPiece = nId;
                bRet = true;
            }

            return bRet;
        }

		
		/// <summary>
		///  Process the moves
		/// </summary>
        /// <param name="nId">int</param>
        /// <returns>int</returns>		
        public int TabProcess(int nId)
        {
            int nRet = 0;

            // 1. check up/down
            if (MoveDown(nId))
            {
                // down
                nRet = moveDown;
            }
            else if (MoveUp(nId))
            {
                // up
                nRet = moveUp;
            }
            else if(MoveLeft(nId))
            {
                // left
                nRet = moveLeft;
            }
            else if(MoveRight(nId))
            {
                // right
                nRet = moveRight;
            }

            if (nRet > 0)
                CheckGameEnd();
            return nRet;
        }

		
		/// <summary>
		///  Check for completion
		/// </summary>		
        private void CheckGameEnd()
        {
            bool bCheck = true;
            
            for (int i = 0; i < pieces.Count; i++)
            {
                if(i != pieces[i].NOrder)
                {
                    bCheck = false;
                    break;
                }
            }
            if(bCheck)
            {
                bGameEnd = true;
            }
        }

		
		/// <summary>
		///  Check for end of game condition
		/// </summary>
        /// <returns>bool</returns>		
        public bool IsGameEnd()
        {
            return bGameEnd;
        }

		
		/// <summary>
		///  
		/// </summary>
        /// <param name="nId">int</param>
		/// <param name="nDirection">int</param>
        /// <returns></returns>		
        public void MoveProcess(int nId, int nDirection)
        {

        }
    }
}
