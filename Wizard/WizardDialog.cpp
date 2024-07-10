// WizardDialog.cpp : implementation file
//

#include "stdafx.h"
#include "WizardDialog.h"
#include "WizardPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




/////////////////////////////////////////////////////////////////////////////
// CWizardDialog dialog



CWizardDialog::CWizardDialog(LPCTSTR lpszTemplateName, 
   CWnd* pParent):CDialog(lpszTemplateName,pParent)
{
  Init();
}

CWizardDialog::CWizardDialog(UINT nIDTemplate, 
	CWnd* pParent):CDialog(nIDTemplate,pParent)
{
  Init();
}

CWizardDialog::~CWizardDialog()
{
}



void CWizardDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWizardDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWizardDialog, CDialog)
	//{{AFX_MSG_MAP(CWizardDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(ID_WIZFINISH, OnWizardFinish)
	ON_BN_CLICKED(ID_WIZBACK, OnWizardBack)
	ON_BN_CLICKED(ID_WIZNEXT, OnWizardNext)
	ON_BN_CLICKED(IDCANCEL, OnCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizardDialog message handlers


void CWizardDialog::Init()
{
  m_nPlaceholderID = 0;
}


BOOL CWizardDialog::DestroyPage(CWizardPage* pPage)
{
	if (pPage->m_bCreated)
	{
		if (pPage->OnKillActive() == FALSE) return FALSE;
		pPage->OnDestroyPage();
		pPage->DestroyWindow();
		pPage->m_bCreated = FALSE;
	}

	// return TRUE even if the page was never created
	return TRUE;
}

CWizardPage* CWizardDialog::GetFirstPage()
{
	return (CWizardPage*)m_PageList.GetHead();	
}

CWizardPage* CWizardDialog::GetLastPage()
{
	return (CWizardPage*)m_PageList.GetTail();	
}


CWizardPage* CWizardDialog::GetActivePage() const
{
	CWizardPage* pPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	while (Pos)
	{
		pPage = (CWizardPage*)m_PageList.GetNext(Pos);
		if (pPage->m_bActive)
		{
			return pPage;
		}
	}
	return NULL;
}


// function to get the next page
CWizardPage* CWizardDialog::GetNextPage()
{
	CWizardPage* pPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	while (Pos)
	{
		pPage = (CWizardPage*)m_PageList.GetNext(Pos);
		if (pPage->m_bActive)
		{
			if (Pos == NULL) return NULL;
			return (CWizardPage*)m_PageList.GetAt(Pos);
		}
	}
	return NULL;
}


void CWizardDialog::SetPlaceholderID(int nPlaceholderID)
{
	m_nPlaceholderID = nPlaceholderID;
}


// returns TRUE if the new page is activated
BOOL CWizardDialog::ActivatePage(CWizardPage* pPage)
{
	ASSERT(m_nPlaceholderID != 0);
	ASSERT(pPage != NULL);
	ASSERT(::IsWindow(m_hWnd));

	// if the page has not been created, then create it
	if (pPage->m_bCreated == FALSE)
	{
		if (pPage->Create(pPage->m_nDialogID, this) == FALSE) return FALSE;
		pPage->m_bCreated = TRUE;
		pPage->m_pParent = this;

	  if (pPage->OnCreatePage() == FALSE) return FALSE;
	}

	// deactivate the current page
	if (!DeactivatePage()) return FALSE;

  CRect rect;
  CWnd *pWnd = GetDlgItem(m_nPlaceholderID);
  ASSERT(pWnd != NULL);
  ASSERT(IsWindow(pWnd->m_hWnd) != FALSE);

  pWnd->GetWindowRect(&rect);
  ScreenToClient(&rect);
  pPage->SetWindowPos(NULL, rect.left, rect.top, 0, 0, 
                       SWP_NOZORDER | SWP_NOSIZE | SWP_NOACTIVATE );
  pPage->EnableWindow(TRUE);

  pPage->ShowWindow(SW_SHOW);
  pPage->InvalidateRect(NULL);
  pPage->UpdateWindow();
  pPage->OnSetActive();
  pPage->m_bActive = TRUE;
	return TRUE;
}

BOOL CWizardDialog::DeactivatePage()
{
	CWizardPage* pPage = GetActivePage();
	if (pPage == NULL) return TRUE;

  ASSERT(pPage->m_bCreated != FALSE);
  if (!pPage->OnKillActive()) return FALSE;
  pPage->ShowWindow(SW_HIDE);
  pPage->m_bActive = FALSE;
	return TRUE;
}


BOOL CWizardDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	ModifyStyleEx (0, WS_EX_CONTROLPARENT);	

	ASSERT(m_nPlaceholderID != 0); // Be sure to call SetPlaceholderID from
																// your dialogs OnInitDialog
	HideButtons();
	// make the first page of the wizard active
	SetFirstPage();



	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CWizardDialog::OnDestroy() 
{
	CWizardPage* pPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	while (Pos)
	{
		pPage = (CWizardPage*)m_PageList.GetNext(Pos);

		// this function could return FALSE, but if it happens here
		// it is too late to do anything about it!
		VERIFY(DestroyPage(pPage));
	}

  CDialog::OnDestroy();
}


// function to add a page to the the list of pages
// nID is the reource ID of the page we are adding
void CWizardDialog::AddPage(CWizardPage* pPage, UINT nID)
{
	m_PageList.AddTail(pPage);
	pPage->m_nDialogID = nID;
}



// Activate the page with the specified dialog resource
void CWizardDialog::SetActivePageByResource(UINT nResourceID)
{
	CWizardPage* pPage = GetPageByResourceID(nResourceID);
	if (pPage == NULL) return;

	if (!DeactivatePage()) return;

	ActivatePage(pPage);
}


// function to return a page object based on it's dialog resource ID
CWizardPage* CWizardDialog::GetPageByResourceID(UINT nResourceID)
{
	CWizardPage* pPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	while (Pos)
	{
		pPage = (CWizardPage*)m_PageList.GetNext(Pos);
		if (pPage->m_nDialogID == nResourceID)
		{
			return pPage;
		}
	}
	return NULL;
}


// Set first page as active page
BOOL CWizardDialog::SetFirstPage()
{
  CWizardPage* pPage = GetFirstPage();

	if (!DeactivatePage()) return FALSE;

	EnableBack(FALSE);
	
	if (m_PageList.GetCount() > 1)
	{
		EnableFinish(FALSE);
		EnableNext(TRUE);
	}
	else // wizard has only one page
	{
		EnableFinish(TRUE);
		EnableNext(FALSE);
	}

	if (ActivatePage(pPage)) return TRUE;
	return FALSE;
}


// Set next page as active page
void CWizardDialog::SetNextPage()
{
  CWizardPage* pPage = GetNextPage();
	if (ActivatePage(pPage))
	{
		EnableBack(TRUE);
	}
}


/////////////////////////////////////////////////////////////
//  CWizardDlg Button Processing


// user pressed the Finish button
void CWizardDialog::OnWizardFinish() 
{
	CWizardPage* pPage;

	pPage = GetActivePage();

	// can we kill the active page?
	if (!pPage->OnKillActive()) return;

	
	// notify all pages that we are finishing
	POSITION Pos = m_PageList.GetHeadPosition();
	while (Pos)
	{
		pPage = (CWizardPage*)m_PageList.GetNext(Pos);
		if (pPage->m_bCreated)
		{
			if (!pPage->OnWizardFinish())
			{
				// data validation failed for one of the pages so we can't close
				return;
			}
		}
	}

	// The only reason this line would be needed is if you had controls
	// place in your main wizard dialog outside of the wizard pages.
	// In most "normal" implementations, this line does nothing
	UpdateData(TRUE);

	// close the dialog and return ID_WIZFINISH
	CDialog::EndDialog(ID_WIZFINISH);
}

void CWizardDialog::OnWizardBack() 
{
	CWizardPage* pPage = GetActivePage();
	ASSERT(pPage);

	LRESULT lResult = pPage->OnWizardBack();
	if (lResult == -1) return; // don't change pages

	if (lResult == 0)
	{
		POSITION Pos = m_PageList.Find(pPage);
		ASSERT(Pos);
		m_PageList.GetPrev(Pos);
		if (Pos == NULL) return; // the current page was the first page
		pPage = (CWizardPage*)m_PageList.GetAt(Pos);
	}
	else
	{
		pPage = GetPageByResourceID(lResult);
		if (pPage == NULL) return;
	}

	if (!ActivatePage(pPage)) return;	

	// if we are on the first page, then disable the back button
	if (pPage == GetFirstPage())
	{
		EnableBack(FALSE);
		EnableFinish(FALSE);
	} else {
		EnableFinish(FALSE);
	}

	// enable the next button
	EnableNext(TRUE);
}


void CWizardDialog::OnWizardNext() 
{
	CWizardPage* pPage = GetActivePage();
	ASSERT(pPage);

	//CString cstrPage;
	
	LRESULT lResult = pPage->OnWizardNext();
	if (lResult == -1) return; // don't change pages

	if (lResult == 0)
	{
		POSITION Pos = m_PageList.Find(pPage);
		ASSERT(Pos);
		m_PageList.GetNext(Pos);
		if (Pos == NULL) return; // the current page was the last page
		pPage = (CWizardPage*)m_PageList.GetAt(Pos);
	}
	else
	{
		pPage = GetPageByResourceID(lResult);
		if (pPage == NULL) return;
	}

	if (!ActivatePage(pPage)) return;	

	// if we are on the last page, then disable the next button
	if (pPage == GetLastPage())
	{
		EnableNext(FALSE);
		//EnableFinish(TRUE);
	}
	EnableBack(TRUE);
}


void CWizardDialog::EnableFinish(BOOL bEnable)
{
	ASSERT(::IsWindow(m_hWnd));
	CWnd* pWnd = GetDlgItem(ID_WIZFINISH);
	ASSERT(pWnd); // You must have an ID_WIZFINISH on your dialog
	if (pWnd)
	{
		pWnd->EnableWindow(bEnable);		
	}
}


void CWizardDialog::EnableBack(BOOL bEnable)
{
	ASSERT(::IsWindow(m_hWnd));
	CWnd* pWnd = GetDlgItem(ID_WIZBACK);
	ASSERT(pWnd); // You must have an ID_WIZBACK on your dialog
	if (pWnd)
	{
		pWnd->EnableWindow(bEnable);		
	}
}


void CWizardDialog::EnableNext(BOOL bEnable)
{
	ASSERT(::IsWindow(m_hWnd));
	CWnd* pWnd = GetDlgItem(ID_WIZNEXT);
	ASSERT(pWnd); // You must have an ID_WIZNEXT on your dialog
	if (pWnd)
	{
		pWnd->EnableWindow(bEnable);		
	}
}


///////////////////////////////////////////////////////////////
//  Functions to mimic the behavior of CPropertySheet


// returns the index of the currently active page
int CWizardDialog::GetActiveIndex() const
{
	CWizardPage* pPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	int nIndex = 0;
	while (Pos)
	{
		pPage = (CWizardPage*)m_PageList.GetNext(Pos);
		if (pPage->m_bActive)
		{
			return nIndex;
		}
		++nIndex;
	}
	return -1;
}


int CWizardDialog::GetPageIndex(CWizardPage* pPage) const
{
	CWizardPage* pTestPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	int nIndex = 0;
	while (Pos)
	{
		pTestPage = (CWizardPage*)m_PageList.GetNext(Pos);
		if (pTestPage == pPage)
		{
			return nIndex;
		}
		++nIndex;
	}
	return -1;
}


int CWizardDialog::GetPageCount()
{
	return m_PageList.GetCount();
}


// get a page based on it's placement index in the list
CWizardPage* CWizardDialog::GetPage(int nPage) const
{
	POSITION Pos = m_PageList.FindIndex(nPage);
	if (Pos == NULL) return NULL;
	return (CWizardPage*)m_PageList.GetAt(Pos);
}



// activate a page based on its place in the list
BOOL CWizardDialog::SetActivePage(int nPage)
{
	CWizardPage* pPage = GetPage(nPage);
	if (pPage == NULL) return FALSE;
	ActivatePage(pPage);
	return TRUE;
}


BOOL CWizardDialog::SetActivePage(CWizardPage* pPage)
{
	ActivatePage(pPage);
	return TRUE;
}


// set the title of the main wizard window
void CWizardDialog::SetTitle(LPCTSTR lpszText)
{
	ASSERT(::IsWindow(m_hWnd));
	SetWindowText(lpszText);
}


void CWizardDialog::SetTitle(UINT nIDText)
{
	CString s;
	s.LoadString(nIDText);
	SetTitle(s);
}


// set the text on the Finish button
void CWizardDialog::SetFinishText(LPCTSTR lpszText)
{
	ASSERT(::IsWindow(m_hWnd));
	CWnd* pWnd = GetDlgItem(ID_WIZFINISH);
	ASSERT(pWnd); // You must have an ID_WIZFINISH on your dialog
	if (pWnd)
	{
		pWnd->SetWindowText(lpszText);		
	}
}

void CWizardDialog::SetFinishText(UINT nIDText)
{
	CString s;
	s.LoadString(nIDText);
	SetFinishText(s);	
}

void CWizardDialog::OnWizardFinishEx()
{
	OnWizardFinish();
}

void CWizardDialog::HideButtons()
{
	GetDlgItem(ID_WIZBACK)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_WIZNEXT)->ShowWindow(SW_HIDE);
	GetDlgItem(ID_WIZFINISH)->ShowWindow(SW_HIDE);
	GetDlgItem(IDCANCEL)->ShowWindow(SW_HIDE);
}


// user pressed the cancel button
void CWizardDialog::OnCancel()
{
	CWizardPage* pPage;
	POSITION Pos = m_PageList.GetHeadPosition();
	while (Pos)
	{
		pPage = (CWizardPage*)m_PageList.GetNext(Pos);
		if (pPage->m_bCreated)
		{
			// can we cancel?
			if (pPage->OnQueryCancel() == FALSE) return;
		}
	}

	CDialog::OnCancel();
}