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

#define MOVE_PIXEL		5
#define MAX_REPTIMAGE	6
#define HIT_MOVE_PIXEL	4
#define MAX_MOVE		8

class Reptile
{
private:
	CString m_strImagePath;
	int     m_nPosX;
	int     m_nPosY;
	BOOL    m_bHit;
	float   m_nAngle;
	BOOL	m_bIOrder;
	int		m_nImagePos;
	Gdiplus::Image *m_pImage[MAX_REPTIMAGE];

public:
	Reptile();
	~Reptile();
	Reptile(CString strImage);
	BOOL LoadReptileImage();
	void UnloadReptileImage();
	void DrawReptileImage(HDC hDC, RECT rcClient);
	BOOL HitTest(CPoint pt);
	void SetHit();
	RECT GetRect();
	void Restart(RECT rcClient);
};

