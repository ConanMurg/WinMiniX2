#if !defined(AFX_AddNoteDLG_H__915D4C86_2E27_11D4_9FA9_0030DB0011C6__INCLUDED_)
#define AFX_AddNoteDLG_H__915D4C86_2E27_11D4_9FA9_0030DB0011C6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MiniX2Dlg.h : header file
#include "WizardDialog.h"
#include "MiniX2.h"
#include "ParsePacket.h"
//#pragma once
//#include "stdafx.h"
//#include "afxwin.h"
//#include "Resource.h"
//#include "ParsePacket.h"
//#include "Label.h"
//#include "PlotCanvas.h"
//#include "DP5Status.h"
//#include "stringex.h"
//#include "WinusbNotify.h"
////#include "MiniX2.h"

void PrintToOutputWindow(const std::string& message);

/////////////////////////////////////////////////////////////////////////////
// CMiniX2Dlg dialog

class CMiniX2Dlg : public CWizardDialog
{
// Construction
public:
	CMiniX2Dlg(CWnd* pParent = NULL);   // standard constructor
// Dialog Data
	//{{AFX_DATA(CMiniX2Dlg)
	enum { IDD = IDD_WIZARD };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMiniX2Dlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	// Generated message map functions
	//{{AFX_MSG(CMiniX2Dlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	void cmdMX2Advanced_Click(int Index);

	//==== Mini-X2 Control Class ============================================================================
	void SendCommand(TRANSMIT_PACKET_TYPE XmtCmd);
	bool bSetAdvancedIndicators;
    void DisplayDouble(int DlgItem, double dblValue, int dpDigits);
	void RemCallParsePacket(BYTE PacketIn[]);
	void ParsePacketEx(Packet_In PIN, DppStateType DppState);

//public:
//	afx_msg void OnDestroy();
//
	afx_msg void OnBnClickedButtonReadMiscData();
	afx_msg void OnBnClickedButtonWriteMiscData();
	string strMiscDataAdv;

//    //========================================================
//    //========================================================
//
//	void ProcessStatusEx(Packet_In PIN, DppStateType DppState);
	void ProcessTextDataEx(Packet_In PIN, DppStateType DppState);
	void ProcessAck(BYTE PID2);
//	void DisplayError(Packet_In PIN, DppStateType DppState);
	void SendCommandData(TRANSMIT_PACKET_TYPE XmtCmd, BYTE DataOut[]);

//	CLabel lblStatus; //CStatic IDC_STATIC_STATUS
//	CLabel lblMiscStatus; //CStatic IDC_STATIC_MISC_STATUS
//	void OnResetCommunications();
//
//	BOOL isConnectedTest();
//
//	//==== Configuration
//	//==============================================================
	string GetCmdData(string strCmd, string strCfgData);
//	//==============================================================
//
//	//==== Configuration
//	//==============================================================
//	void SetStartTime();
//	//==============================================================
//
//	afx_msg void OnBnClickedOk();
//	afx_msg void OnBnClickedCancel();
//
//	afx_msg BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
//
//	// communications
//	BOOL isDppConnected; // Current Interface selected and device found
//	string HwCfgDP5;
//	string HwCfgDP5Out;
//	BOOL HwCfgReady;
//	BOOL HwCfgExReady;
//	
//	CDP5Status DP5Stat;
//	HWND hWnd_Main;
//	string CreateDTS(string strFormat);
//
//=======================================================================================================
// ==== MiniX2 Development ==============================================================================
//=======================================================================================================

//	// ==== Mini-X2 Receive Packet Processing Routines ======================================================
	void ProcessStatusExMX2(Packet_In PIN, DppStateType DppState);
	void ProcessCfgReadMX2Ex(Packet_In PIN, DppStateType DppState);
	void ProcessTubeInterlockTableMX2Ex(Packet_In PIN, DppStateType DppState);
	void ProcessWarmupTableMX2Ex(Packet_In PIN, DppStateType DppState);
	void ProcessTimestampRecordMX2Ex(Packet_In PIN, DppStateType DppState);
	string strOnTime;
	string strRunMonthlyWU;
//
	void ProcessFaultRecordMX2Ex(Packet_In PIN, DppStateType DppState);
    bool ReadbackMX2_HVandI(float *sngHV, float *sngI);
	bool bMiniX2ControllerRunning;
	bool bAdvancedStatus;        //Only display status values in advanced tab when requested
	bool bAdvancedInterlock;     //Only display interlock values in advanced tab when requested
	bool bAdvancedTimeStamp;     //Only display Timestamp values in advanced tab when requested
	void SendCommandDataMX2(TRANSMIT_PACKET_TYPE XmtCmd, string strDataIn);

	void SendMX2_FaultOverride(bool bFaultOverride = false);
	void SendMX2_LimitOverride(bool bLimitOverride = false);
	void SendMX2_Volume(bool bVolumeOn = true);

	void SendMX2_HVandI(string strHV, string strI);
	void SetHvAndI_Off();
	void MX2UpdateTimestampAndWarmupInd();
	void cmdMX2AscCmd_Click(int iIndex);
//
//	//void cmdMX2Advanced_Click(int Index);
	void cmdWarmUpSequence_Click(int Index);
//
	string FloatToString(float fltValue, int dpDigits);
	float StringToFloat(string strValue);
//    void DisplayStatus(byte StatusCode);
//	float GetWindowFloat(int nID);
//    double GetWindowDouble(int nID);
	FLASHWINFO FlashTbInfo;
	void SetFlashWindow(DWORD dwWinFunc);

};

//extern CDP5Protocol DP5Proto;
//extern CDppWinUSB DppWinUSB;
//extern CWinusbNotify WinusbNotify;
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AddNoteDLG_H__915D4C86_2E27_11D4_9FA9_0030DB0011C6__INCLUDED_)
