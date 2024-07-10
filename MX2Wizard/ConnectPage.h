#pragma once
#ifndef CONNECTPAGE_H
#define CONNECTPAGE_H
#include "afxwin.h"
#include "Label.h"
#include "NetFinder.h"
#include "DP5Status.h"
#include "SendCommand.h"
#include "WinusbNotify.h"
#include "WizardPage.h"


#ifdef _DEBUG
#define isDebug true
#else
#define isDebug false
#endif

// CConnectPage dialog

//DS_FIXEDSYS | 
//#define IDD_MX2_CONNECT                 129

class CConnectPage : public CWizardPage
{
	//DECLARE_DYNAMIC(CConnectPage)

public:
	CConnectPage(CWnd* pParent = NULL);   // standard constructor
	//CConnectPage();
	//virtual ~CConnectPage();

// Dialog Data
	enum { IDD = IDW_CONNECT };

protected:
	virtual void OnOK();
	void OnSetActive();

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	//CToolTipCtrl m_ToolTip;								// Tooltip control

public:
	virtual BOOL OnInitDialog();
	// ===== USB ============================================
	afx_msg void OnBnClickedButtonFindUsbDevice();
	CNetFinder FindDppUSB;					// netfinder helper class
	// ======================================================

	// ===== shared functions ===============================
	void EnableCommFunctions(int ControlGroup);		// enables/disables control groups
	// radio button selects communications type
	afx_msg void OnBnClickedRadioCommtype2();		// usb
	CLabel lblDppFound[3];							// displays dpp found by comm type
	// ======================================================

	//afx_msg void OnBnClickedOk();
	CSpinButtonCtrl m_SpinSelectDevice;
	afx_msg void OnDeltaposSpinSelectDevice(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonCloseDevice();

	afx_msg void OnBnClickedConnectOK();
	//virtual void OnOK();
	virtual BOOL DestroyWindow();
	//afx_msg BOOL OnHelpInfo(HELPINFO* pHelpInfo);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonCountDevices();

	bool bMiniX2Connected;
};

extern CDP5Protocol DP5Proto;
extern CSendCommand SndCmd;
extern CDppWinUSB DppWinUSB;
extern CWinusbNotify WinusbNotify;


#endif //CONNECTPAGE_H
























