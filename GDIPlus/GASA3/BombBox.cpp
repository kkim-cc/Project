/*
* FILE : BombBox.cpp
* PROJECT : GAS - Assignment #3
* PROGRAMMER : Ki Duck Kim
* FIRST VERSION : 2015-01-15
* DESCRIPTION: implementation of the BombBox class
*/

#include "stdafx.h"
#include "BombBox.h"
#include <stdlib.h> // srand, rand

BombBox::BombBox()
{
	m_strImagePath = L"";
	m_nPosX = 0;
	m_nPosY = 0;
	m_nOrder = 0;
	m_nImagePos = 0;
	for (int i = 0; i < MAX_BOMBIMAGE; i++)
	{
		m_pImage[i] = NULL;
	}
	m_bHit = false;
	srand((unsigned int)time(NULL));
}


BombBox::~BombBox()
{
	UnloadBombBoxImage();
}

/*
* FUNCTION : BombBox
*
* DESCRIPTION : Constructor
*
* PARAMETERS : CString strImage : Filepath of image file format
*			   int nOrder : The order of bomb
*			   RECT rcClient : Rectangle of Client
*/
BombBox::BombBox(CString strImage, int nOrder)
	: BombBox()
{
	m_strImagePath = strImage;
	m_nOrder = nOrder;
}

/*
* FUNCTION : GetPosition
*
* DESCRIPTION : Make a position of BombBox
*
* PARAMETERS : RECT rcClient : Rectangle of Client
*
* RETURNS : Nothing
*/
void BombBox::GetPosition(RECT rcClient)
{
	int width = rcClient.right - rcClient.left;

	m_nPosX = rand() % ((width) / MAX_BOMB);

	if (m_nPosX + (int)m_pImage[0]->GetWidth() > (width / MAX_BOMB))
	{
		m_nPosX = (width / MAX_BOMB) - (int)m_pImage[0]->GetWidth();
	}

	if (m_nOrder > 0)
	{
		m_nPosX += (width / MAX_BOMB) * m_nOrder;
	}

	m_nPosY = rcClient.bottom - (m_pImage[0]->GetHeight());
}

/*
* FUNCTION : LoadBombBoxImage
*
* DESCRIPTION : Load Image to local handle
*
* PARAMETERS : Nothing
*
* RETURNS : BOOL : True - Success, False - Fail
*/
BOOL BombBox::LoadBombBoxImage()
{
	CString strFile;
	if (m_strImagePath.IsEmpty())
		return FALSE;

	if (m_pImage[0] != NULL)
		return TRUE;

	for (int i = 0; i < MAX_BOMBIMAGE; i++)
	{
		strFile.Format(L"%s%d.png", m_strImagePath, i);
		m_pImage[i] = Image::FromFile(strFile);
	}

	if (m_pImage[0] != NULL)
		return TRUE;
	else
		return FALSE;
}

/*
* FUNCTION : UnloadBombBoxImage
*
* DESCRIPTION : Delete local hanlde
*
* PARAMETERS : Nothing
*
* RETURNS : Nothing
*/

void BombBox::UnloadBombBoxImage()
{
	if (m_pImage[0] != NULL)
	{
		for (int i = 0; i < MAX_BOMBIMAGE; i++)
		{
			delete m_pImage[i];
			m_pImage[i] = NULL;
		}
	}
}

/*
* FUNCTION : DrawBombBoxImage
*
* DESCRIPTION : Draw BombBox Image to given DC handle.
*
* PARAMETERS : HDC hDC: Memory DC
*              RECT rcClient : Rectangle of Client
*			   RECT rcReptile : Rectangle of Reptile
*
* RETURNS : Nothing
*/
BOOL BombBox::DrawBombBoxImage(HDC hDC, RECT rcClient, RECT rcReptile, BOOL bRedrawBox)
{
	if (m_nPosY == 0 || bRedrawBox)
	{
		GetPosition(rcClient);
	}

	BOOL bRet = FALSE;

	if (m_bHit)
	{
		if (m_nImagePos < MAX_BOMBIMAGE - 1)
		{
			m_nImagePos++;
		}
		else if (m_nImagePos >= MAX_BOMBIMAGE - 1)
		{
			m_bHit = false;
			m_nImagePos = 0;
			GetPosition(rcClient);
		}
	}
	else 
	{
		CPoint pt1(rcReptile.left, rcReptile.top), pt2(rcReptile.right, rcReptile.bottom);
		CRect rcBox(m_nPosX+20, m_nPosY+20, 
			m_nPosX + m_pImage[m_nImagePos]->GetWidth()-20,
			m_nPosY + m_pImage[m_nImagePos]->GetHeight()-20);
		if (rcBox.PtInRect(pt1) || rcBox.PtInRect(pt2))
		//if (rcIntersect.IntersectRect(CRect(rcReptile), rcBox))
		{
			m_bHit = TRUE;
			m_nImagePos++;
			bRet = TRUE;
		}
	}

	Graphics graphics(hDC);

	graphics.DrawImage(m_pImage[m_nImagePos], m_nPosX, m_nPosY,
		m_pImage[m_nImagePos]->GetWidth(), m_pImage[m_nImagePos]->GetHeight());

	return bRet;
}