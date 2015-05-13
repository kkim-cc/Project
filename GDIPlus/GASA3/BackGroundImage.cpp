/*
* FILE : BackGroundImage.cpp
* PROJECT : GAS - Assignment #2
* PROGRAMMER : Ki Duck Kim
* FIRST VERSION : 2015-01-28
* DESCRIPTION: implementation of the BackGroundImage class
*/

#include "stdafx.h"
#include "BackGroundImage.h"


BackGroundImage::BackGroundImage()
{
	m_strBack = L"";
	m_strMid = L"";
	m_strFore = L"";
	m_bmpBack = NULL; 
	m_bmpMid = NULL;
	m_bmpFore = NULL;
}

/*
* FUNCTION : GroundImage
*
* DESCRIPTION : Constructor assign file path of the image, Is image a background.
*
* PARAMETERS : CString strImageFile : file name
*              BOOL bIsBack : Is BackgroundImage
*/
BackGroundImage::BackGroundImage(CString strBack, CString strMid, CString strFore)
	: BackGroundImage()
{
	m_strBack = strBack;
	m_strMid = strMid;
	m_strFore = strFore;
}

BackGroundImage::~BackGroundImage()
{
	UnloadBackGroundImage();
}

/*
* FUNCTION : LoadGroundImage
*
* DESCRIPTION : Load image to memory handle
*
* PARAMETERS : Nothing
*
* RETURNS : BOOL if success TRUE else FALSE
*/
BOOL BackGroundImage::LoadBackGroundImage()
{
	if (m_strBack.IsEmpty() || m_strMid.IsEmpty() || m_strFore.IsEmpty())
		return FALSE;

	if (m_bmpBack == NULL)
	{
		m_bmpBack = Bitmap::FromFile(m_strBack);
	}
	if (m_bmpMid == NULL)
	{
		m_bmpMid = Image::FromFile(m_strMid);
	}
	if (m_bmpFore == NULL)
	{
		m_bmpFore = Image::FromFile(m_strFore);
	}

	if (m_bmpBack && m_bmpMid && m_bmpFore)
		return TRUE;
	else
		return FALSE;
}

/*
* FUNCTION : DrawImage
*
* DESCRIPTION : Draw Ground Image to given DC handle.
*
* PARAMETERS : HDC hDC: Memory DC
*              RECT rcClient : Rectangle of Client
*
* RETURNS : Nothing
*/
void BackGroundImage::DrawImage(HDC hDC, RECT rcClient)
{
	if (LoadBackGroundImage() == FALSE)
		return;

	Gdiplus::REAL left = Gdiplus::REAL(rcClient.left);
	Gdiplus::REAL top = Gdiplus::REAL(rcClient.top);
	Gdiplus::REAL width = Gdiplus::REAL(rcClient.right - rcClient.left);
	Gdiplus::REAL height = Gdiplus::REAL(rcClient.bottom - rcClient.top);

	Graphics graphics(hDC);
	ImageAttributes ImgAttr;
	// Draw Background
	graphics.DrawImage(m_bmpBack, RectF(left, top, width, height),
		0, 0, Gdiplus::REAL(m_bmpBack->GetWidth()), Gdiplus::REAL(m_bmpBack->GetHeight()),
		UnitPixel, &ImgAttr);

	// Draw Midground
	ImgAttr.SetColorKey(Gdiplus::Color(0, 255, 0), Gdiplus::Color(0, 255, 0));
	
	graphics.DrawImage(m_bmpMid, RectF(left, top, width, height), 
		0, 0, Gdiplus::REAL(m_bmpMid->GetWidth()), Gdiplus::REAL(m_bmpMid->GetHeight()),
		UnitPixel, &ImgAttr);

	// Draw Foreground
	ImgAttr.SetColorKey(Gdiplus::Color(0, 255, 0), Gdiplus::Color(0, 255, 0));

	graphics.DrawImage(m_bmpFore, RectF(left, top, width, height),
		0, 0, Gdiplus::REAL(m_bmpFore->GetWidth()), Gdiplus::REAL(m_bmpFore->GetHeight()),
		UnitPixel, &ImgAttr);
}

/*
* FUNCTION : UnloadGroundImage
*
* DESCRIPTION : Delete memory handle
*
* PARAMETERS : Nothing
*
* RETURNS : Nothing
*/
void BackGroundImage::UnloadBackGroundImage()
{
	if (m_bmpBack != NULL)
	{
		delete m_bmpBack;
		m_bmpBack = NULL;
	}
	if (m_bmpMid != NULL)
	{
		delete m_bmpMid;
		m_bmpMid = NULL;
	}
	if (m_bmpFore != NULL)
	{
		delete m_bmpFore;
		m_bmpFore = NULL;
	}
}