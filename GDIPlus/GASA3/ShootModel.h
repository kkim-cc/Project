/*
* FILE : ShootModel.h
* PROJECT : GAS - Assignment #3
* PROGRAMMER : Ki Duck Kim
* FIRST VERSION : 2015-01-28
* DESCRIPTION: interface of the ShootModel class
*/

#pragma once
#include "BackGroundImage.h"
#include "Reptile.h"
#include "BombBox.h"

class ShootModel
{
private:
	BackGroundImage *m_pBackground;
	Reptile *m_pReptile;
	BombBox *m_pBombBox[MAX_BOMB];
	CRect	m_rcClient;

public:
	ShootModel();
	~ShootModel();
	BOOL Load(CString strBack, CString strMid, CString strFore, CString strBird, CString strBomb);
	void FreeShootModel();
	void Paint(HDC hDC, RECT rcClient);
	BOOL HitTest(CPoint pt);
	void SetHit();
};