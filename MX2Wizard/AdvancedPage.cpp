// AdvancedPage.cpp : implementation file
//
#include "stdafx.h"
#include "vcMiniX2.h"
#include "WizardDialog.h"
#include "AdvancedPage.h"
#include "MiniX2Dlg.h"
#include ".\advancedpage.h"
#include "MSecTimer.h"
#include "DebugUtils.h" // MINE - ALLOWS PrintToOutputWindow 

CAdvancedPage *AdvancedPage = NULL;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAdvancedPage dialog


//CAdvancedPage::CAdvancedPage(CvcDP5Dlg *pParentDP5, CWnd* pParent /*=NULL*/)
CAdvancedPage::CAdvancedPage(CWnd* pParent /*=NULL*/)
	: CWizardPage(CAdvancedPage::IDD, pParent)
{
//	m_pParentMX2 = pParentDP5;
	PrintToOutputWindow("CAdvancedPage - AdvancedPage.cpp\n");
}

void CAdvancedPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAdvancedPage)
	DDX_Control(pDX, IDC_GROUP_ADVANCED_PAGE, m_fraAdvancedPage);
	DDX_Control(pDX, IDC_LABEL_ON_TIME, m_lblOnTime);
	DDX_Control(pDX, IDC_LABEL_WARMUP_STEP, m_lblWarmupStep);
	DDX_Control(pDX, IDC_GROUP_ADVANCED_OPTIONS, m_fraAdvancedOptions);
	DDX_Control(pDX, IDC_LABEL_ADVANCED_OPTION_STATUS_2, m_lblAdvancedOptionStatus_2);
	DDX_Control(pDX, IDC_LABEL_ADVANCED_OPTION_STATUS_1, m_lblAdvancedOptionStatus_1);
	DDX_Control(pDX, IDC_LABEL_ADVANCED_OPTION_STATUS_0, m_lblAdvancedOptionStatus_0);
	DDX_Control(pDX, IDC_BUTTON_MX2_ASCII_CMD_2, m_cmdMX2AscCmd_2);
	DDX_Control(pDX, IDC_BUTTON_MX2_ASCII_CMD_0, m_cmdMX2AscCmd_0);
	DDX_Control(pDX, IDC_BUTTON_MX2_ASCII_CMD_1, m_cmdMX2AscCmd_1);
	DDX_Control(pDX, IDC_BUTTON_MX2_ADVANCED_4, m_cmdMX2Advanced_4);
	DDX_Control(pDX, IDC_BUTTON_MX2_ADVANCED_3, m_cmdMX2Advanced_3);
	DDX_Control(pDX, IDC_BUTTON_MX2_ADVANCED_2, m_cmdMX2Advanced_2);
	DDX_Control(pDX, IDC_BUTTON_MX2_ADVANCED_1, m_cmdMX2Advanced_1);
	DDX_Control(pDX, IDC_BUTTON_MX2_ADVANCED_0, m_cmdMX2Advanced_0);
	DDX_Control(pDX, IDC_BUTTON_MX2_ADVANCED_DONE, m_cmdMX2AdvancedDone);
	DDX_Control(pDX, IDC_EDIT_MX2_ADVANCED_DISPLAY, m_txtMX2AdvancedDisplay);
	DDX_Control(pDX, IDC_GROUP_WARMUP, m_fraWarmup);
	DDX_Control(pDX, IDC_LABEL_RUN_MONTHLY_WU, m_lblRunMonthlyWU);
	DDX_Control(pDX, IDC_BUTTON_WARMUP_SEQUENCE_0, m_cmdWarmUpSequence_0);
	DDX_Control(pDX, IDC_BUTTON_WARMUP_SEQUENCE_1, m_cmdWarmUpSequence_1);
	DDX_Control(pDX, IDC_BUTTON_WARMUP_SEQUENCE_2, m_cmdWarmUpSequence_2);
	DDX_Control(pDX, IDC_GROUP_MISC_DATA_MX2, m_fraMiscData);
	DDX_Control(pDX, IDC_EDIT_MISC_DATA_MX2, m_txtMiscData);
	DDX_Control(pDX, IDC_BUTTON_READ_MISC_DATA_MX2, m_cmdReadMiscData);
	DDX_Control(pDX, IDC_BUTTON_WRITE_MISC_DATA_MX2, m_cmdWriteMiscData);
	DDX_Control(pDX, IDC_BUTTON_CLEAR_MISC_DATA_MX2, m_cmdClearMiscData);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAdvancedPage, CDialog)
	//{{AFX_MSG_MAP(CAdvancedPage)
	//ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_MX2_ADVANCED_DONE, OnBnClickedButtonMx2AdvancedDone)
	ON_BN_CLICKED(IDC_BUTTON_MX2_ADVANCED_0, OnBnClickedButtonMx2Advanced0)
	ON_BN_CLICKED(IDC_BUTTON_MX2_ADVANCED_1, OnBnClickedButtonMx2Advanced1)
	ON_BN_CLICKED(IDC_BUTTON_MX2_ADVANCED_2, OnBnClickedButtonMx2Advanced2)
	ON_BN_CLICKED(IDC_BUTTON_MX2_ADVANCED_3, OnBnClickedButtonMx2Advanced3)
	ON_BN_CLICKED(IDC_BUTTON_MX2_ADVANCED_4, OnBnClickedButtonMx2Advanced4)
	ON_BN_CLICKED(IDC_BUTTON_MX2_ASCII_CMD_0, OnBnClickedButtonMx2AsciiCmd0)
	ON_BN_CLICKED(IDC_BUTTON_MX2_ASCII_CMD_1, OnBnClickedButtonMx2AsciiCmd1)
	ON_BN_CLICKED(IDC_BUTTON_MX2_ASCII_CMD_2, OnBnClickedButtonMx2AsciiCmd2)
	ON_BN_CLICKED(IDC_BUTTON_READ_MISC_DATA_MX2, OnBnClickedButtonReadMiscData)
	ON_BN_CLICKED(IDC_BUTTON_WRITE_MISC_DATA_MX2, OnBnClickedButtonWriteMiscData)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR_MISC_DATA_MX2, OnBnClickedButtonClearMiscData)
	ON_BN_CLICKED(IDC_BUTTON_WARMUP_SEQUENCE_0, OnBnClickedButtonWarmupSequence0)
	ON_BN_CLICKED(IDC_BUTTON_WARMUP_SEQUENCE_1, OnBnClickedButtonWarmupSequence1)
	ON_BN_CLICKED(IDC_BUTTON_WARMUP_SEQUENCE_2, OnBnClickedButtonWarmupSequence2)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CAdvancedPage message handlers

BOOL CAdvancedPage::OnInitDialog()
{
	PrintToOutputWindow("OnInitDialog- AdvancedPage.cpp\n");

	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	ZeroMemory(m_cmdWarmUpSequence, sizeof(m_cmdWarmUpSequence));
	m_cmdWarmUpSequence[2] = &m_cmdWarmUpSequence_2;
	m_cmdWarmUpSequence[1] = &m_cmdWarmUpSequence_1;
	m_cmdWarmUpSequence[0] = &m_cmdWarmUpSequence_0;

	ZeroMemory(m_cmdMX2Advanced, sizeof(m_cmdMX2Advanced));
	m_cmdMX2Advanced[0] = &m_cmdMX2Advanced_0;
	m_cmdMX2Advanced[1] = &m_cmdMX2Advanced_1;
	m_cmdMX2Advanced[2] = &m_cmdMX2Advanced_2;
	m_cmdMX2Advanced[3] = &m_cmdMX2Advanced_3;
	m_cmdMX2Advanced[4] = &m_cmdMX2Advanced_4;

	ZeroMemory(m_cmdMX2AscCmd, sizeof(m_cmdMX2AscCmd));
	m_cmdMX2AscCmd[1] = &m_cmdMX2AscCmd_1;
	m_cmdMX2AscCmd[0] = &m_cmdMX2AscCmd_0;
	m_cmdMX2AscCmd[2] = &m_cmdMX2AscCmd_2;

	ZeroMemory(m_lblAdvancedOptionStatus, sizeof(m_lblAdvancedOptionStatus));
	m_lblAdvancedOptionStatus[0] = &m_lblAdvancedOptionStatus_0;
	m_lblAdvancedOptionStatus[1] = &m_lblAdvancedOptionStatus_1;
	m_lblAdvancedOptionStatus[2] = &m_lblAdvancedOptionStatus_2;
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	m_tooltip.AddTool(&m_lblOnTime, "Last Time On");
	m_txtMX2AdvancedDisplay.SetWindowText("Mini-X2 Display\r\n");
	m_tooltip.AddTool(&m_lblRunMonthlyWU, "Tube Warmup Recommendations");

	fm_fraAdvancedOptions.Set(GetDlgItem(IDC_GROUP_ADVANCED_OPTIONS), GetSysColor(COLOR_BTNTEXT), GetSysColor(COLOR_BTNFACE),"System",9.75,true,ANSI_CHARSET);
	fm_lblAdvancedOptionStatus_2.Set(GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_2), GetSysColor(COLOR_BTNTEXT), RGB(0,192,0));
	fm_lblAdvancedOptionStatus_1.Set(GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_1), GetSysColor(COLOR_BTNTEXT), RGB(0,192,0));
	fm_lblAdvancedOptionStatus_0.Set(GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_0), GetSysColor(COLOR_BTNTEXT), RGB(0,192,0));
	fm_fraWarmup.Set(GetDlgItem(IDC_GROUP_WARMUP), GetSysColor(COLOR_BTNTEXT), GetSysColor(COLOR_BTNFACE),"System",9.75,true,ANSI_CHARSET);
	fm_fraMiscData.Set(GetDlgItem(IDC_GROUP_MISC_DATA_MX2), GetSysColor(COLOR_BTNTEXT), GetSysColor(COLOR_BTNFACE),"System",9.75,true,ANSI_CHARSET);

	tmrAdvanced_TimerId = 0;			//defined as tmrAdvanced_EventId == 40;
	tmrAdvanced_Interval = 500;
	tmrAdvanced_Enabled = false;

	tmrAdvanced_TimerId = SetTimer(tmrAdvanced_EventId, tmrAdvanced_Interval, NULL);
	tmrAdvanced_Enabled = true;
	//KillTimer(tmrAdvanced_TimerId);

	m_pParentMX2 = (CMiniX2Dlg*)GetParent();

	return TRUE;
}

void CAdvancedPage::OnOK()
{
	return;		// there is no OnOk Button, disable return key exit
	//CWizardPage::OnOK();
}

void CAdvancedPage::OnSetActive()
{
	m_pParent->SetTitle("Mini-X2 Advanced Features");
}

void CAdvancedPage::OnBnClickedButtonMx2AdvancedDone()
{
	if (tmrAdvanced_Enabled) {
		tmrAdvanced_Enabled = false;
		KillTimer(tmrAdvanced_TimerId);
	}
	//CDialog::OnOK();
	CWizardDialog* pSheet = (CWizardDialog*)GetParent();
	int iPage = pSheet->GetPageIndex(this);
	pSheet->SetActivePage(iPage - 1);
}

void CAdvancedPage::OnBnClickedButtonMx2Advanced0()
{
	CMSecTimer tmr;
	m_pParentMX2->cmdMX2Advanced_Click(0);
	tmr.msTimer(200);
	GetDlgItem(IDC_EDIT_MX2_ADVANCED_DISPLAY)->SetWindowText(MiniX2.strMX2AdvancedDisplay.c_str());
}

void CAdvancedPage::OnBnClickedButtonMx2Advanced1()
{
	CMSecTimer tmr;
	m_pParentMX2->cmdMX2Advanced_Click(1);
	tmr.msTimer(200);
	GetDlgItem(IDC_EDIT_MX2_ADVANCED_DISPLAY)->SetWindowText(MiniX2.strMX2AdvancedDisplay.c_str());
}

void CAdvancedPage::OnBnClickedButtonMx2Advanced2()
{
	CMSecTimer tmr;
	m_pParentMX2->cmdMX2Advanced_Click(2);
	tmr.msTimer(200);
	GetDlgItem(IDC_EDIT_MX2_ADVANCED_DISPLAY)->SetWindowText(MiniX2.strMX2AdvancedDisplay.c_str());
}

void CAdvancedPage::OnBnClickedButtonMx2Advanced3()
{
	CMSecTimer tmr;
	m_pParentMX2->cmdMX2Advanced_Click(3);
	tmr.msTimer(200);
	GetDlgItem(IDC_EDIT_MX2_ADVANCED_DISPLAY)->SetWindowText(MiniX2.strMX2AdvancedDisplay.c_str());
}

void CAdvancedPage::OnBnClickedButtonMx2Advanced4()
{
	CMSecTimer tmr;
	m_pParentMX2->cmdMX2Advanced_Click(4);
	tmr.msTimer(200);
	GetDlgItem(IDC_EDIT_MX2_ADVANCED_DISPLAY)->SetWindowText(MiniX2.strMX2AdvancedDisplay.c_str());
}

void CAdvancedPage::SetAdvancedIndicators()
{
	int rgbBackColor;
	string strWarmupStep="";
	stringex strfn;


	if (MiniX2.STATUS_MNX.DISABLE_LIMIT_CHECKS) {
		GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_0)->SetWindowText("Limits: Disabled");
		rgbBackColor = RGB(255,0,0);
	} else {
		GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_0)->SetWindowText("Limits: Enabled");
		rgbBackColor = RGB(0,192,0);
	}
	fm_lblAdvancedOptionStatus_0.Set(GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_0), GetSysColor(COLOR_BTNTEXT), rgbBackColor);
	//Invalidate();
	//UpdateWindow();

	if (MiniX2.STATUS_MNX.DISABLE_FAULT_CHECKS) {
		GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_1)->SetWindowText("Faults: Disabled");
		rgbBackColor = RGB(255,0,0);
	} else {
		GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_1)->SetWindowText("Faults: Enabled");
		rgbBackColor = RGB(0,192,0);
	}
	fm_lblAdvancedOptionStatus_1.Set(GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_1), GetSysColor(COLOR_BTNTEXT), rgbBackColor);
	//Invalidate();
	//UpdateWindow();

	if (MiniX2.STATUS_MNX.SPEAKER_EN) {
		GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_2)->SetWindowText("Speaker: ON");
		rgbBackColor = RGB(0,192,0);
	} else {
		GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_2)->SetWindowText("Speaker: OFF");
		rgbBackColor = RGB(255,0,0);
	}
	fm_lblAdvancedOptionStatus_2.Set(GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_2), GetSysColor(COLOR_BTNTEXT), rgbBackColor);

	 //display advanced warmup indicators if running warmup sequence
    if (MiniX2.STATUS_MNX.WARMUP_STEP > 127) {    // MSB is set if warmup is running
		GetDlgItem(IDC_LABEL_WARMUP_STEP)->ShowWindow(SW_SHOW);
		strWarmupStep = " Warmup Step\r\n";
        strWarmupStep += " Step: ";
		strWarmupStep += strfn.Format("%d\r\n",(MiniX2.STATUS_MNX.WARMUP_STEP & 15));
        strWarmupStep += " Step Time: ";
		strWarmupStep += strfn.Format("%d\r\n",MiniX2.STATUS_MNX.WARMUP_TIME);
		GetDlgItem(IDC_LABEL_WARMUP_STEP)->SetWindowText(strWarmupStep.c_str());
    } else {
        GetDlgItem(IDC_LABEL_WARMUP_STEP)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_LABEL_WARMUP_STEP)->SetWindowText("");
    }

	GetDlgItem(IDC_LABEL_RUN_MONTHLY_WU)->SetWindowText(m_pParentMX2->strRunMonthlyWU.c_str());
	GetDlgItem(IDC_LABEL_ON_TIME)->SetWindowText(m_pParentMX2->strOnTime.c_str());

	Invalidate();
	UpdateWindow();
}

void CAdvancedPage::OnBnClickedButtonMx2AsciiCmd0()
{
	int rgbBackColor;
	CMSecTimer tmr;

	int msgResponse;
	CString strMessage;

	strMessage = "Set Limit Overrided?\n\n";
	strMessage += "WARNING: TUBE DAMAGE COULD RESULT \n";
	strMessage += "FROM OPERATING THE TUBE OUTSIDE OF \n";
	strMessage += "	RECOMMENDED LIMITS. \n";

    msgResponse = AfxMessageBox(strMessage,MB_YESNO | MB_ICONQUESTION | MB_TOPMOST);
    if (msgResponse == IDYES) {
		m_pParentMX2-> cmdMX2AscCmd_Click(0);
		tmr.msTimer(100);
		m_pParentMX2->cmdMX2Advanced_Click(0);
		tmr.msTimer(100);
		if (MiniX2.STATUS_MNX.DISABLE_LIMIT_CHECKS) {
			GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_0)->SetWindowText("Limits: Disabled");
			rgbBackColor = RGB(255,0,0);
		} else {
			GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_0)->SetWindowText("Limits: Enabled");
			rgbBackColor = RGB(0,192,0);
		}
		fm_lblAdvancedOptionStatus_0.Set(GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_0), GetSysColor(COLOR_BTNTEXT), rgbBackColor);
		Invalidate();
		UpdateWindow();
	}
}

void CAdvancedPage::OnBnClickedButtonMx2AsciiCmd1()
{
	int rgbBackColor;
	CMSecTimer tmr;
	int msgResponse;
	CString strMessage;

	strMessage = "Set Fault Overrided?\n\n";
	strMessage += "WARNING: TUBE DAMAGE COULD RESULT \n";
	strMessage += "FROM OPERATING THE TUBE OUTSIDE OF \n";
	strMessage += "	RECOMMENDED LIMITS. \n";

    msgResponse = AfxMessageBox(strMessage,MB_YESNO | MB_ICONQUESTION | MB_TOPMOST);
    if (msgResponse == IDYES) {
		m_pParentMX2-> cmdMX2AscCmd_Click(1);
		tmr.msTimer(100);
		m_pParentMX2->cmdMX2Advanced_Click(0);
		tmr.msTimer(100);
		if (MiniX2.STATUS_MNX.DISABLE_FAULT_CHECKS) {
			GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_1)->SetWindowText("Faults: Disabled");
			rgbBackColor = RGB(255,0,0);
		} else {
			GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_1)->SetWindowText("Faults: Enabled");
			rgbBackColor = RGB(0,192,0);
		}
		fm_lblAdvancedOptionStatus_1.Set(GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_1), GetSysColor(COLOR_BTNTEXT), rgbBackColor);
		Invalidate();
		UpdateWindow();
	}
}

void CAdvancedPage::OnBnClickedButtonMx2AsciiCmd2()
{
	int rgbBackColor;
	CMSecTimer tmr;
	int msgResponse;
	CString strMessage;

	strMessage = "Set Mini-X2 Speaker Volume Control?\n\n";
	strMessage += "WARNING: Disabling the speaker will disable the \n";
	strMessage += "  once-per-second warning beep \n";
	strMessage += "while the tube is producing x-rays. \n";

    msgResponse = AfxMessageBox(strMessage,MB_YESNO | MB_ICONQUESTION | MB_TOPMOST);
    if (msgResponse == IDYES) {
		m_pParentMX2-> cmdMX2AscCmd_Click(2);
		tmr.msTimer(100);
		m_pParentMX2->cmdMX2Advanced_Click(0);
		tmr.msTimer(100);
		if (MiniX2.STATUS_MNX.SPEAKER_EN) {
			GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_2)->SetWindowText("Speaker: ON");
			rgbBackColor = RGB(0,192,0);
		} else {
			GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_2)->SetWindowText("Speaker: OFF");
			rgbBackColor = RGB(255,0,0);
		}
		fm_lblAdvancedOptionStatus_2.Set(GetDlgItem(IDC_LABEL_ADVANCED_OPTION_STATUS_2), GetSysColor(COLOR_BTNTEXT), rgbBackColor);
		Invalidate();
		UpdateWindow();
	}
}

void CAdvancedPage::OnBnClickedButtonReadMiscData()
{
	CMSecTimer tmr;
	m_pParentMX2->bAdvancedStatus = true;
	m_pParentMX2->OnBnClickedButtonReadMiscData();
	tmr.msTimer(200);
	GetDlgItem(IDC_EDIT_MISC_DATA_MX2)->SetWindowText(m_pParentMX2->strMiscDataAdv.c_str());
}

void CAdvancedPage::OnBnClickedButtonWriteMiscData()
{
	stringex strfn;
	char byteMiscData[512+1];

	memset(&byteMiscData,0,sizeof(byteMiscData));
	GetDlgItem(IDC_EDIT_MISC_DATA_MX2)->GetWindowText(byteMiscData, 512+1);
	m_pParentMX2->strMiscDataAdv = strfn.Format("%s",byteMiscData);
	m_pParentMX2->bAdvancedStatus = true;
	m_pParentMX2->OnBnClickedButtonWriteMiscData();
}

void CAdvancedPage::OnBnClickedButtonClearMiscData()
{
	GetDlgItem(IDC_EDIT_MISC_DATA_MX2)->SetWindowText("");
}

void CAdvancedPage::OnBnClickedButtonWarmupSequence0()
{
	////SendCommand XMTPT_INITIATE_WARMUP_DAILY_SEQUENCE_MX2	//Daily Warmup
	//CMSecTimer tmr;
	m_pParentMX2->cmdWarmUpSequence_Click(0);
	//tmr.msTimer(200);
}

void CAdvancedPage::OnBnClickedButtonWarmupSequence1()
{
	////SendCommand XMTPT_INITIATE_WARMUP_MONTHLY_SEQUENCE_MX2	//Monthly Warmup
	//CMSecTimer tmr;
	m_pParentMX2->cmdWarmUpSequence_Click(1);
	//tmr.msTimer(200);
}

void CAdvancedPage::OnBnClickedButtonWarmupSequence2()
{
	////SetHvAndI False		//Stop Warmup
	CMSecTimer tmr;
	m_pParentMX2->cmdWarmUpSequence_Click(2);
	//SetHvAndI(false);
	tmr.msTimer(200);
}

HBRUSH CAdvancedPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	switch(pWnd->GetDlgCtrlID()) {
		case IDC_GROUP_ADVANCED_OPTIONS:	return fm_fraAdvancedOptions.OnCtlColor(pDC);
		case IDC_LABEL_ADVANCED_OPTION_STATUS_2:	return fm_lblAdvancedOptionStatus_2.OnCtlColor(pDC);
		case IDC_LABEL_ADVANCED_OPTION_STATUS_1:	return fm_lblAdvancedOptionStatus_1.OnCtlColor(pDC);
		case IDC_LABEL_ADVANCED_OPTION_STATUS_0:	return fm_lblAdvancedOptionStatus_0.OnCtlColor(pDC);
		case IDC_GROUP_WARMUP:	return fm_fraWarmup.OnCtlColor(pDC);
		case IDC_GROUP_MISC_DATA_MX2:	return fm_fraMiscData.OnCtlColor(pDC);
	}
	return CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CAdvancedPage::OnTimer(UINT nIDEvent)
{
	if (m_pParentMX2->bSetAdvancedIndicators) {
		m_pParentMX2->bSetAdvancedIndicators = false;
		SetAdvancedIndicators();
	}
	CDialog::OnTimer(nIDEvent);
}
