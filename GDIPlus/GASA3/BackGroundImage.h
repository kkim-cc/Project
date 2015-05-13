/*
* FILE : BackGroundImage.h
* PROJECT : GAS - Assignment #3
* PROGRAMMER : Ki Duck Kim
* FIRST VERSION : 2015-01-28
* DESCRIPTION: interface of the BackGroundImage class
*/

#pragma once

#include "stdafx.h"
#include <gdiplus.h>
using namespace Gdiplus;

class BackGroundImage
{
private:
	CString m_strBack, m_strMid, m_strFore;
	Gdiplus::Bitmap* m_bmpBack;
	Gdiplus::Image* m_bmpMid;
	Gdiplus::Image* m_bmpFore;
	const double alpha = 0.5f;

public:
	BackGroundImage();
	BackGroundImage(CString strBack, CString strMid, CString strFore);
	~BackGroundImage();

	BOOL LoadBackGroundImage();
	void DrawImage(HDC hDC, RECT rcClient);
	void UnloadBackGroundImage();
};

