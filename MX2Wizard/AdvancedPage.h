// AdvancedPage.h : header file
//

#pragma once
#include "afxwin.h"
#include "resource.h"
#include "FontAttribute.h"
#include "WizardPage.h"
#include "MiniX2.h"

// CAdvancedPage dialog
class CAdvancedPage : public CWizardPage
{
// Construction
public:
	CAdvancedPage(CWnd* pParent = NULL);	// standard constructor
	//CAdvancedPage(CvcDP5Dlg *pParentDP5, CWnd* pParent = NULL);	// standard constructor
	//CvcDP5Dlg* m_pParentMX2;
	CMiniX2Dlg* m_pParentMX2;


// Dialog Data
	enum { IDD = IDW_ADVANCED };

protected:


// Implementation
protected:
	virtual void OnOK();
	void OnSetActive();

	//HICON m_hIcon;
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	// Generated message map functions
	//{{AFX_MSG(CAdvancedPage)
	virtual BOOL OnInitDialog();
	//afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	//{{AFX_DATA(CAdvancedPage)
	CButton	m_fraAdvancedPage;
	CStatic	m_lblOnTime;
	CStatic	m_lblWarmupStep;
	CButton	m_fraAdvancedOptions;
	CStatic	m_lblAdvancedOptionStatus_2;
	CStatic	m_lblAdvancedOptionStatus_1;
	CStatic	m_lblAdvancedOptionStatus_0;
	CButton	m_cmdMX2AscCmd_2;
	CButton	m_cmdMX2AscCmd_0;
	CButton	m_cmdMX2AscCmd_1;
	CButton	m_cmdMX2Advanced_4;
	CButton	m_cmdMX2Advanced_3;
	CButton	m_cmdMX2Advanced_2;
	CButton	m_cmdMX2Advanced_1;
	CButton	m_cmdMX2Advanced_0;
	CButton	m_cmdMX2AdvancedDone;
	CEdit	m_txtMX2AdvancedDisplay;
	CButton	m_fraWarmup;
	CStatic	m_lblRunMonthlyWU;
	CButton	m_cmdWarmUpSequence_0;
	CButton	m_cmdWarmUpSequence_1;
	CButton	m_cmdWarmUpSequence_2;
	CButton	m_fraMiscData;
	CEdit	m_txtMiscData;
	CButton	m_cmdReadMiscData;
	CButton	m_cmdWriteMiscData;
	CButton	m_cmdClearMiscData;
	//}}AFX_DATA

public:
	CButton *m_cmdWarmUpSequence[3];
	CButton *m_cmdMX2Advanced[5];
	CButton *m_cmdMX2AscCmd[3];
	CStatic *m_lblAdvancedOptionStatus[3];

	CToolTipCtrl	m_tooltip;

	CFontAttribute fm_fraAdvancedOptions, fm_lblAdvancedOptionStatus_2, fm_lblAdvancedOptionStatus_1;
	CFontAttribute fm_lblAdvancedOptionStatus_0, fm_fraWarmup, fm_fraMiscData;

	afx_msg void OnBnClickedButtonMx2AdvancedDone();
	afx_msg void OnBnClickedButtonMx2Advanced0();
	afx_msg void OnBnClickedButtonMx2Advanced1();
	afx_msg void OnBnClickedButtonMx2Advanced2();
	afx_msg void OnBnClickedButtonMx2Advanced3();
	afx_msg void OnBnClickedButtonMx2Advanced4();

	void SetAdvancedIndicators();

	afx_msg void OnBnClickedButtonMx2AsciiCmd0();
	afx_msg void OnBnClickedButtonMx2AsciiCmd1();
	afx_msg void OnBnClickedButtonMx2AsciiCmd2();
	afx_msg void OnBnClickedButtonReadMiscData();
	afx_msg void OnBnClickedButtonWriteMiscData();
	afx_msg void OnBnClickedButtonClearMiscData();
	afx_msg void OnBnClickedButtonWarmupSequence0();
	afx_msg void OnBnClickedButtonWarmupSequence1();
	afx_msg void OnBnClickedButtonWarmupSequence2();
	afx_msg void OnTimer(UINT nIDEvent);

	#define tmrAdvanced_EventId 40		// Advanced Refresh Timer
	UINT_PTR tmrAdvanced_TimerId;
	bool tmrAdvanced_Enabled;
	UINT tmrAdvanced_Interval;

};

extern CMiniX2 MiniX2;

extern CAdvancedPage *AdvancedPage;
