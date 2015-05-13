/*
* FILE : ShootModel.cpp
* PROJECT : GAS - Assignment #3
* PROGRAMMER : Ki Duck Kim
* FIRST VERSION : 2015-01-28
* DESCRIPTION: implementation of the ShootModel class
*/
#include "stdafx.h"
#include "ShootModel.h"


ShootModel::ShootModel()
{
	m_pBackground = NULL;
	m_pReptile = NULL;
	for (int i = 0; i < MAX_BOMB; i++)
	{
		m_pBombBox[i] = NULL;
	}
	m_rcClient = CRect(0, 0, 0, 0);
}

ShootModel::~ShootModel()
{
	FreeShootModel();
}

/*
* FUNCTION : Load
*
* DESCRIPTION : Set file path to local handle and call load image
*
* PARAMETERS : CString strBack	// file path of background image
*			   CString strMid   // file path of midground image
*			   CString strFore  // file path of foreground image
*			   CString strBird  // file path of reptile image
*			   CString strBomb  // file Path of bombbox image
*
* RETURNS : BOOL : True - Success, False - Error
*/
BOOL ShootModel::Load(CString strBack, CString strMid, CString strFore, CString strBird, CString strBomb)
{
	BOOL bRet;

	if (m_pBackground == NULL)
		m_pBackground = new BackGroundImage(strBack, strMid, strFore);
	
	bRet = m_pBackground->LoadBackGroundImage();
	if (!bRet)
		return bRet;

	if (m_pReptile == NULL)
		m_pReptile = new Reptile(strBird);

	bRet = m_pReptile->LoadReptileImage();
	if (!bRet)
		return bRet;

	for (int i = 0; i < MAX_BOMB; i++)
	{
		if (m_pBombBox[i] == NULL)
		{
			m_pBombBox[i] = new BombBox(strBomb, i);
			bRet = m_pBombBox[i]->LoadBombBoxImage();
			if (!bRet)
				return bRet;
		}
	}


	return TRUE;
}


/*
* FUNCTION : FreeShootModel
*
* DESCRIPTION : free background image class and reptile class
*
* PARAMETERS : nothing
*
* RETURNS : nothing
*/
void ShootModel::FreeShootModel()
{
	if (m_pBackground != NULL)
	{
		delete m_pBackground;
		m_pBackground = NULL;
	}
	if (m_pReptile != NULL)
	{
		delete m_pReptile;
		m_pReptile = NULL;
	}
	if (m_pBombBox[0] != NULL)
	{
		for (int i = 0; i < MAX_BOMB; i++)
		{
			if (m_pBombBox[i] != NULL)
			{
				delete m_pBombBox[i];
				m_pBombBox[i] = NULL;
			}
		}
	}
}

/*
* FUNCTION : Paint
*
* DESCRIPTION : call the draw functions of background class and reptile class
*
* PARAMETERS :	HDC hDC			- Memory DC
*				RECT rcClient	- Client size
*
* RETURNS : nothing
*/
void ShootModel::Paint(HDC hDC, RECT rcClient)
{
	BOOL bRedrawBox = FALSE;

	if (m_pBackground == NULL || m_pReptile == NULL)
		return;

	m_pBackground->DrawImage(hDC, rcClient);
	m_pReptile->DrawReptileImage(hDC, rcClient);

	if (m_rcClient != rcClient)
	{
		m_rcClient = rcClient;
		bRedrawBox = TRUE;
	}
	RECT rcReptile = m_pReptile->GetRect();
	for (int i = 0; i < MAX_BOMB; i++)
	{
		if (m_pBombBox[i] != NULL)
		{
			if (m_pBombBox[i]->DrawBombBoxImage(hDC, rcClient, rcReptile, bRedrawBox))
			{
				m_pReptile->Restart(rcClient);
			}
		}
	}
}

/*
* FUNCTION : HitTest
*
* DESCRIPTION : call the HitTest functions of reptile class
*
* PARAMETERS :	CPoint pt - the point of Button Clicked
*
* RETURNS : bool True - hit, False - miss
*/
BOOL ShootModel::HitTest(CPoint pt)
{
	if (m_pReptile == NULL)
		return FALSE;
	return m_pReptile->HitTest(pt);
}

/*
* FUNCTION : HitTest
*
* DESCRIPTION : call the SetHit functions of reptile class
*
* PARAMETERS :	nothing
*
* RETURNS : nothing
*/
void ShootModel::SetHit()
{
	if (m_pReptile != NULL)
	{
		m_pReptile->SetHit();
	}
}