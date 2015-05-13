/*
* FILE : Reptile.h
* PROJECT : GAS - Assignment #3
* PROGRAMMER : Ki Duck Kim
* FIRST VERSION : 2015-01-15
* DESCRIPTION: interface of the Reptile class
*/
#pragma once
#include "stdafx.h"
#include <gdiplus.h>
using namespace Gdiplus;

#define MAX_BOMBIMAGE	7
#define MAX_BOMB		6

class BombBox
{
private:
	CString m_strImagePath;
	int     m_nPosX;
	int     m_nPosY;
	BOOL    m_bHit;
	int 	m_nOrder;
	int		m_nImagePos;
	Gdiplus::Image *m_pImage[MAX_BOMBIMAGE];

	void	GetPosition(RECT rcClient);
public:
	BombBox();
	~BombBox();
	BombBox(CString strImage, int nOrder);
	BOOL LoadBombBoxImage();
	void UnloadBombBoxImage();
	BOOL DrawBombBoxImage(HDC hDC, RECT rcClient, RECT rcReptile, BOOL bRedrawBox);
};

