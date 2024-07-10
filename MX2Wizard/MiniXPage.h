// MiniXPage.h : header file
//

#pragma once
#include "afxwin.h"
#include "resource.h"
#include "FontAttribute.h"
#include "WizardPage.h"
#include "MiniX2.h"
#include "MiniX2Dlg.h"
#include "NetFinder.h"
#include "DP5Status.h"
#include "SendCommand.h"
#include "WinusbNotify.h"
#include "WizardPage.h"
#include "SXButton.h"			// Access local header

// CMiniXPage dialog
class CMiniXPage : public CWizardPage
{
// Construction
public:
	CMiniXPage(CWnd* pParent = NULL);	// standard constructor


// Dialog Data
	enum { IDD = IDW_MINIX2 };

// Implementation
protected:
	virtual void OnOK();
	void OnSetActive();
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	//HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CMiniXPage)
	virtual BOOL OnInitDialog();
	//afx_msg void OnPaint();
	//afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()

	//{{AFX_DATA(CMiniXPage)
	CButton	m_fraMiniXControls;
	CStatic	m_lblStatus;
	CStatic	m_lbllblBoardTemp;
	CStatic	m_lbllblPower_mW;
	CStatic	m_lblBoardTemp;
	CStatic	m_lblSerialNumber;
	CStatic	m_lblInterLock;
	CStatic	m_lblXRayOn;
	CStatic	m_lblPower_mW;
	CStatic	m_lblFaultDisplay;
	CStatic	m_lblMiniX2MainPanel;
	CStatic	m_picAmptekLogo;
	CStatic	m_picMiniXLogo;
	CStatic	m_picMiniXHw;
	CStatic	m_picIsoCurve_0;
	CButton	m_fraSettings;
	CStatic	m_lblMIN_0;
	CStatic	m_lblMAX_0;
	CStatic	m_lblMIN_1;
	CStatic	m_lblMIN_2;
	CStatic	m_lblMAX_1;
	CStatic	m_lblMAX_2;
	CStatic	m_lblHvMonitor;
	CStatic	m_lblCurrentMonitor;
	CStatic	m_lblkV_0;
	CStatic	m_lblkV_1;
	CStatic	m_lbluA_0;
	CStatic	m_lbluA_1;
	CStatic	m_lbllblMonitor_0;
	CStatic	m_lbllblMonitor_1;
	CStatic	m_lblWattageOutOfRange;

	//CButton	m_cmdHvOn;
	//CButton	m_cmdHvOff;
	//CButton	m_cmdExit;

	CSXButton	m_cmdHvOn;
	CSXButton	m_cmdHvOff;
	CSXButton	m_cmdExit;

	CEdit	m_txtHv;
	CEdit	m_txtCurrent;
	CButton	m_cmdSetHVandCurrent;
	CStatic	m_picInterLock_0;
	CStatic	m_picInterLock_1;
	CStatic	m_picXRayOn;
	CStatic	m_picIsoCurve_1;
	CStatic	m_picInterLock_2;
	CButton	m_cmdAdvanced;
	CStatic	m_picStatusInfo;
	//}}AFX_DATA

public:
	CMiniX2Dlg* m_pParentMX2;
	CStatic *m_picInterLock[3];
	CStatic *m_picIsoCurve[2];
	CStatic *m_lbllblMonitor[2];
	CStatic *m_lbluA[2];
	CStatic *m_lblkV[2];
	CStatic *m_lblMAX[3];
	CStatic *m_lblMIN[3];

	CBitmap	Bmp_cmdHvOn;
	CBitmap	Bmp_cmdHvOff;
	CBitmap	Bmp_cmdExit;
	CToolTipCtrl	m_tooltip;

	CFontAttribute fm_lblPower_mW, fm_lblMiniX2MainPanel, fm_fraSettings, fm_lblWattageOutOfRange;

	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT nIDEvent);

	//==== Mini-X2 Timers ===================================================================================
	#define tmrXRayOn_EventId 10		// X-Ray ON Graphics Timer
    UINT_PTR tmrXRayOn_TimerId;
    bool tmrXRayOn_Enabled;
    UINT tmrXRayOn_Interval;

	#define tmrMonitor_EventId 20		// Mini-X2 Controller Monitor Timer
    UINT_PTR tmrMonitor_TimerId;
    bool tmrMonitor_Enabled;
    UINT tmrMonitor_Interval;

	#define tmrInterLock_EventId 30		// Mini-X2 Controller Interlock Timer
    UINT_PTR tmrInterLock_TimerId;
    bool tmrInterLock_Enabled;
    UINT tmrInterLock_Interval;
    void TimerControl(UINT uEventId, bool isCreate);

	InterlockViewState indOldInterlockStatus;
	void InterLockStatusTest();
    void InterlockDisplay(InterlockViewState ILockView);
	void UpdateMX2MonitoredDisplayValues();

	float sngMX2TubePowerMax;      //Tube Power Maximum assigned after tube type has been determined
	float sngInterlockMinI;       //Interlock Minimum current assigned after tube type has been determined
	float sngInterlockMaxI;        //Interlock Maximum current assigned after tube type has been determined

	void DisplayDouble(int DlgItem, double dblValue, int dpDigits);

	void HideIsoCurveAndMiniX();
	void DisplayIsoCurveAndMiniX(MX2TubeTypeEnum iMX);

    // ==== Mini-X2 Command Button Handlers =================================================================
	afx_msg void OnBnClickedHvOff();
	afx_msg void OnBnClickedHvOn();
	afx_msg void OnBnClickedSethighvoltageandcurrentbutton();
	afx_msg void OnBnClickedAdvancedButton();

	void ShowMX2Status(string strStatus);
	void SetHvAndI(bool bTubeOn = true);
	void EnableMiniX2CommandButtons(unsigned char mxmEnabledCmds);
	int CBool(unsigned char mxmEnabledCmds);
	string FloatToString(float fltValue, int dpDigits);
	float GetWindowFloat(int nID);
    void CheckHvAndCurrentDisplayLimits();
    //void EnableMiniX_Commands(byte mxmEnabledCmds);
//	void ShowStatus(string strStatus);
	bool SetMX2TubeType(TubeInterlockTableType TubeInterlockTable);




};

extern CDP5Protocol DP5Proto;
extern CSendCommand SndCmd;
extern CDppWinUSB DppWinUSB;
extern CWinusbNotify WinusbNotify;
extern CMiniX2 MiniX2;

extern CMiniXPage *MiniXPage;
