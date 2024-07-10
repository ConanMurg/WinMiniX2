#ifndef FontAttribute_H
#define FontAttribute_H

class CFontAttribute {
public:
	CFontAttribute();
	~CFontAttribute();
	void Set(CWnd* pWndNew, 
				int rgbForeColorNew = RGB(0,0,0), int rgbBackColorNew = RGB(255,255,255),
				const char *pFaceName = NULL, float PointSize	= 0,
				bool lfBold	= false, BYTE lfCharSet = DEFAULT_CHARSET);
	HBRUSH OnCtlColor(CDC* pDC);
protected:
	CWnd* pWnd;
	int rgbForeColor;
	int rgbBackColor;
	CFont *pFont;
	LOGFONT *pLogFont;
	CBrush mBrush;
};

#endif // FontAttribute_H
