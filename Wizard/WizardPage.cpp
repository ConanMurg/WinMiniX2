// WizardPage.cpp : implementation file
//

#include "stdafx.h"
#include "WizardPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWizardPage dialog



CWizardPage::CWizardPage(LPCTSTR lpszTemplateName, CWnd* pParent)
                   :CDialog(lpszTemplateName,pParent)
{
  m_bCreated = FALSE;
  m_bActive = FALSE;
  m_nDialogID = 0;
  m_pParent = NULL; 
}


CWizardPage::CWizardPage(UINT nIDTemplate, CWnd* pParent)
                   :CDialog(nIDTemplate,pParent)
{
  m_bCreated = FALSE;
  m_bActive = FALSE;
  m_nDialogID = nIDTemplate;
  m_pParent = NULL; 
}

CWizardPage::~CWizardPage()
{
}



void CWizardPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardPage)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardPage, CDialog)
	//{{AFX_MSG_MAP(CWizardPage)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardPage message handlers


BOOL CWizardPage::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// create the large font
	m_LargeFont.CreateFont(-16, 0, 0, 0, 
		FW_BOLD, FALSE, FALSE, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, _T("MS Sans Serif"));

  DWORD style = GetStyle();
  ASSERT((style & WS_CHILD) != 0);
  ASSERT((style & WS_BORDER) == 0);
  ASSERT((style & WS_DISABLED) != 0);

	m_Brush.CreateSolidBrush(RGB(255, 255, 255));


  return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CWizardPage::OnCreatePage()
{
	return TRUE;
}

void CWizardPage::OnDestroyPage()
{
}

// refer to CPropertyPage class members for a description of this function
void CWizardPage::OnCancel()
{
}


// refer to CPropertyPage class members for a description of this function
BOOL CWizardPage::OnKillActive()
{
	return TRUE;
}

// called when the page is set active
void CWizardPage::OnSetActive()
{
}

// refer to CPropertyPage class members for a description of this function
BOOL CWizardPage::OnQueryCancel()
{
	return TRUE;
}


// refer to CPropertyPage class members for a description of this function
// return -1 to prevent changing pages
// to move to a page other than the previous one, return the 
// Dialog resource ID of the page to display
LRESULT CWizardPage::OnWizardBack()
{
	return 0;
}

// refer to CPropertyPage class members for a description of this function
// return -1 to prevent changing pages
// to move to a page other than the next one, return the 
// Dialog resource ID of the page to display
LRESULT CWizardPage::OnWizardNext()
{
	return 0;
}



// refer to CPropertyPage class members for a description of this function
BOOL CWizardPage::OnWizardFinish()
{
	return TRUE;
}


















