/*
* FILE : Reptile.cpp
* PROJECT : GAS - Assignment #3
* PROGRAMMER : Ki Duck Kim
* FIRST VERSION : 2015-01-15
* DESCRIPTION: implementation of the Reptile class
*/

#include "stdafx.h"
#include "Reptile.h"
#include <stdlib.h> // srand, rand

Reptile::Reptile()
{
	m_strImagePath = L"";
	m_nPosX = 0;
	m_nPosY = 0;
	m_bIOrder = true;
	m_nImagePos = 0;
	for (int i = 0; i < MAX_REPTIMAGE; i++)
	{
		m_pImage[i] = NULL;
	}
	m_bHit = false;
	m_nAngle = 0;
	srand((unsigned int)time(NULL));
}


Reptile::~Reptile()
{
	UnloadReptileImage();
}

/*
* FUNCTION : Reptile
*
* DESCRIPTION : Constructor
*
* PARAMETERS : CString strImage : Filepath of image file format
*/
Reptile::Reptile(CString strImage)
	: Reptile()
{
	m_strImagePath = strImage;
}

/*
* FUNCTION : LoadReptileImage
*
* DESCRIPTION : Load Image to local handle
*
* PARAMETERS : Nothing
*
* RETURNS : BOOL : True - Success, False - Fail
*/
BOOL Reptile::LoadReptileImage()
{
	CString strFile;
	if (m_strImagePath.IsEmpty())
		return FALSE;

	if (m_pImage[0] != NULL)
		return TRUE;

	for (int i = 0; i < MAX_REPTIMAGE; i++)
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
* FUNCTION : UnloadReptileImage
*
* DESCRIPTION : Delete local hanlde
*
* PARAMETERS : Nothing
*
* RETURNS : Nothing
*/
void Reptile::UnloadReptileImage()
{
	if (m_pImage[0] != NULL)
	{
		for (int i = 0; i < MAX_REPTIMAGE; i++)
		{
			delete m_pImage[i];
			m_pImage[i] = NULL;
		}
	}
}

void Reptile::Restart(RECT rcClient)
{
	int height = rcClient.bottom - rcClient.top;
	m_nPosY = rand() % ((height - (int)m_pImage[0]->GetHeight()) / 3);
	//m_nPosY = (height - (int)m_pImage[0]->GetHeight()) / 3;
	m_nPosX = 0;
	m_nAngle = 0;
	m_bHit = FALSE;
	m_nImagePos = 0;
	m_bIOrder = true;
}

/*
* FUNCTION : DrawReptileImage
*
* DESCRIPTION : Draw Reptile Image to given DC handle.
*
* PARAMETERS : HDC hDC: Memory DC
*              RECT rcClient : Rectangle of Client
*
* RETURNS : Nothing
*/
void Reptile::DrawReptileImage(HDC hDC, RECT rcClient)
{
	if (LoadReptileImage() == FALSE)
		return;

	int left = rcClient.left;
	int top = rcClient.top;
	int width = rcClient.right - rcClient.left;
	int height = rcClient.bottom - rcClient.top;
	BOOL bRestart = FALSE;
	int nXMove = 0, nYMove = 0;

	if (m_bHit == FALSE)
	{
		nXMove = m_nPosX + (rand() % (MAX_MOVE*2) - MAX_MOVE/2);
		nYMove = m_nPosY + (rand() % (MAX_MOVE*2) - MAX_MOVE);

		if (nXMove + (int)m_pImage[0]->GetWidth() > width)
		{
			bRestart = TRUE;
		}
		else
			m_nPosX = nXMove;

		if (nYMove <= 0)
		{
			m_nPosY = 0;
		}
		else if (nYMove > (height - (int)m_pImage[0]->GetHeight()) / 3)
		{
			m_nPosY = (height - (int)m_pImage[0]->GetHeight()) / 3;
		}
		else
		{
			m_nPosY = nYMove;
		}
	}
	else
	{
		// calculate again..
		if (m_nPosY + MOVE_PIXEL + (int)m_pImage[0]->GetHeight() > height ||
			m_nPosX + MOVE_PIXEL + (int)m_pImage[0]->GetWidth() > width)
		{
			bRestart = TRUE;
		}
		else
		{
			m_nPosY += HIT_MOVE_PIXEL;
			m_nPosX += HIT_MOVE_PIXEL;
			m_nAngle += 5;
			if (m_nAngle >= 360)
				m_nAngle = 0;
		}
	}

	if (bRestart)
	{
		Restart(rcClient);
	}

	Graphics graphics(hDC);

	if (m_bHit)
	{
		float Tx = m_nPosX + (float)m_pImage[5]->GetWidth() / 2;
		float Ty = m_nPosY + (float)m_pImage[5]->GetHeight() / 2;

		graphics.ScaleTransform(1.0f, 1.0f, MatrixOrder::MatrixOrderAppend);
		graphics.TranslateTransform(-Tx, -Ty, MatrixOrder::MatrixOrderAppend);
		graphics.RotateTransform(m_nAngle, MatrixOrder::MatrixOrderAppend);
		graphics.TranslateTransform(Tx, Ty, MatrixOrder::MatrixOrderAppend);
		graphics.DrawImage(m_pImage[5], m_nPosX, m_nPosY, m_pImage[5]->GetWidth(), m_pImage[5]->GetHeight());
	}
	else
	{
		if (m_nImagePos <= 0 && m_bIOrder == FALSE)
		{
			m_nImagePos++;
			m_bIOrder = TRUE;
		}
		else if (m_nImagePos >= MAX_REPTIMAGE - 1 && m_bIOrder == TRUE)
		{
			m_nImagePos--;
			m_bIOrder = FALSE;
		}
		else if (m_bIOrder)
		{
			m_nImagePos++;
		}
		else
		{
			m_nImagePos--;
		}
		graphics.DrawImage(m_pImage[m_nImagePos], m_nPosX, m_nPosY, 
			m_pImage[m_nImagePos]->GetWidth(), m_pImage[m_nImagePos]->GetHeight());
	}
	
}


/*
* FUNCTION : HitTest
*
* DESCRIPTION : Check the point exist the rectangle of bird image.
*
* PARAMETERS : CPoint pt: Point
*
* RETURNS : BOOL if exist in the rectangle of bird image then return true else false
*/
BOOL Reptile::HitTest(CPoint pt)
{
	CRect rcImage;
	rcImage.left = m_nPosX+40; 
	rcImage.top = m_nPosY+40;
	rcImage.right = m_nPosX + (int)m_pImage[0]->GetWidth()-25;
	rcImage.bottom = m_nPosY + (int)m_pImage[0]->GetHeight()-25;
	if (rcImage.PtInRect(pt))
		return TRUE;
	else
		return FALSE;
}

/*
* FUNCTION : SetHit
*
* DESCRIPTION : Set the bird is hit.
*
* PARAMETERS : Nothing
*
* RETURNS : Nothing
*/
void Reptile::SetHit()
{
	m_bHit = TRUE;
}

/*
* FUNCTION : GetRect
*
* DESCRIPTION : Get the bird's Real Rect
*
* PARAMETERS : Nothing
*
* RETURNS : RECT
*/
RECT Reptile::GetRect()
{
	RECT rcImage;
	rcImage.left = m_nPosX +40;
	rcImage.top = m_nPosY +40;
	rcImage.right = m_nPosX + (int)m_pImage[0]->GetWidth() -25;
	rcImage.bottom = m_nPosY + (int)m_pImage[0]->GetHeight() -25;
	return rcImage;
}