#if !defined(AFX_WizardDIALOG_H__915D4C89_2E27_11D4_9FA9_0030DB0011C6__INCLUDED_)
#define AFX_WizardDIALOG_H__915D4C89_2E27_11D4_9FA9_0030DB0011C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WizardDialog.h : header file
class CWizardPage;

/////////////////////////////////////////////////////////////////////////////
// CWizardDialog dialog

class CWizardDialog : public CDialog
{
// Construction
public:
	CWizardDialog (LPCTSTR lpszTemplate, CWnd* pParent = NULL);
  CWizardDialog (UINT nIDTemplate, CWnd* pParent = NULL);
  virtual ~CWizardDialog ();


// Attributes
public:


protected:
	CObList m_PageList; // this list of CWizardPage objects
	UINT m_nPlaceholderID; // control ID for the palceholder

// Operations
public:
	void AddPage(CWizardPage* pPage, UINT nID);

  void SetActivePageByResource(UINT nResourceID);
  BOOL SetFirstPage();
  void SetNextPage();

	void EnableFinish(BOOL bEnable);
	void EnableBack(BOOL bEnable);
	void EnableNext(BOOL bEnable);
	int GetActiveIndex() const;
	int GetPageIndex(CWizardPage* pPage) const;
	int GetPageCount();
	CWizardPage* GetPage(int nPage) const;
	BOOL SetActivePage(int nPage);
	BOOL SetActivePage(CWizardPage* pPage);
	void SetTitle(LPCTSTR lpszText);
	void SetTitle(UINT nIDText);
	void SetFinishText(LPCTSTR lpszText);
	void SetFinishText(UINT nIDText);

protected:
  BOOL ActivatePage(CWizardPage* pPage);
  BOOL DeactivatePage();
	void SetPlaceholderID(int nPlaceholderID);
	CWizardPage* GetPageByResourceID(UINT nResourceID);


private:
  void Init();
  BOOL DestroyPage(CWizardPage* pPage);
  CWizardPage* GetFirstPage();
  CWizardPage* GetLastPage();
  CWizardPage* GetActivePage() const;
  CWizardPage* GetNextPage();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWizardDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CWizardDialog)
	afx_msg void OnDestroy();
	afx_msg void OnWizardFinish();
	afx_msg void OnWizardBack(); 
	afx_msg void OnWizardNext(); 
	afx_msg void OnCancel(); 
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//AddNote
public:

	void OnWizardFinishEx();
	void HideButtons();

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WizardDIALOG_H__915D4C89_2E27_11D4_9FA9_0030DB0011C6__INCLUDED_)
