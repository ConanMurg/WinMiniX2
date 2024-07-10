#include "stdafx.h"
#include "FontAttribute.h"

/////////////////////////////////////////////////////////////////////////////
// CFontAttribute Control Helper Class
/////////////////////////////////////////////////////////////////////////////

CFontAttribute::CFontAttribute() {
	pWnd = NULL;
	pFont = NULL;
	pLogFont = NULL;
	rgbForeColor = RGB(0,0,0);
	rgbBackColor = RGB(255,255,255);
	mBrush.CreateSolidBrush(rgbBackColor);
}
CFontAttribute::~CFontAttribute() {
	delete pFont;
	delete pLogFont;
}
void CFontAttribute::Set(CWnd* pWndNew, 
						int rgbForeColorNew, int rgbBackColorNew,
						const char *pFaceName, float PointSize,	
						bool lfBold, BYTE lfCharSet) {
	pWnd = pWndNew;																	// control handle
	rgbForeColor = rgbForeColorNew;
	rgbBackColor = rgbBackColorNew;
	mBrush.DeleteObject();
	mBrush.CreateSolidBrush(rgbBackColor);

	if (!pWnd) return;																// no control handle
	if (!(pFaceName || PointSize || lfBold || lfCharSet!=DEFAULT_CHARSET)) return;	// no attrib to set

	if (pFont) {
		return;
	} else {
		pFont = new CFont();
		pLogFont = new LOGFONT();
		pWnd->GetFont()->GetLogFont(pLogFont);
		pFont->CreateFontIndirect(pLogFont);
	}

	if (pFaceName) strcpy(pLogFont->lfFaceName, pFaceName);

	if (PointSize) {
		CPaintDC dc(pWnd);
		pLogFont->lfHeight = - long(PointSize * dc.GetDeviceCaps(LOGPIXELSY) / 72);
	}

	if (lfBold) pLogFont->lfWeight = FW_BOLD;
	if (lfCharSet!=DEFAULT_CHARSET) pLogFont->lfCharSet	= lfCharSet;
		
	pFont->DeleteObject();
	pFont->CreateFontIndirect(pLogFont);
	pWnd->SetFont(pFont);
	pWnd->Invalidate();
}

HBRUSH CFontAttribute::OnCtlColor(CDC* pDC) {
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(rgbForeColor);
	return (HBRUSH)(mBrush.GetSafeHandle()); // mBrush
}
