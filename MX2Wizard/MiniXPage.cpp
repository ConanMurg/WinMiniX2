// MiniXPage.cpp : implementation file
//

#include "stdafx.h"
//#include "MiniX2.h"
#include "WizardDialog.h"
#include "MiniXPage.h"
#include ".\minixpage.h"
#include "MSecTimer.h"
CMiniXPage *MiniXPage = NULL;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


// CMiniXPage dialog

CMiniXPage::CMiniXPage(CWnd* pParent /*=NULL*/)
	: CWizardPage(CMiniXPage::IDD, pParent)
{
//
}

void CMiniXPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMiniXPage)
	DDX_Control(pDX, IDC_MINX_GROUP, m_fraMiniXControls);
	DDX_Control(pDX, IDC_STATUS_EDIT, m_lblStatus);
	DDX_Control(pDX, IDC_TEMPERATURE_SENSOR_LABEL, m_lbllblBoardTemp);
	DDX_Control(pDX, IDC_POWER_MW_LABEL, m_lbllblPower_mW);
	DDX_Control(pDX, IDC_TEMPERATURE_SENSOR, m_lblBoardTemp);
	DDX_Control(pDX, IDC_SERIAL_NUMBER, m_lblSerialNumber);
	DDX_Control(pDX, IDC_INTERLOCK_LABEL, m_lblInterLock);
	DDX_Control(pDX, IDC_XRAY_ON_LABEL, m_lblXRayOn);
	DDX_Control(pDX, IDC_POWER_MW, m_lblPower_mW);
	DDX_Control(pDX, IDC_FAULT_DISPLAY_LABEL, m_lblFaultDisplay);
	DDX_Control(pDX, IDC_MINIX2_MAIN_PANEL_LABEL, m_lblMiniX2MainPanel);
	DDX_Control(pDX, IDC_AMPTEK_LOGO_PIC, m_picAmptekLogo);
	DDX_Control(pDX, IDC_MINIX_LOGO_PIC, m_picMiniXLogo);
	DDX_Control(pDX, IDC_HIGHVOLTAGEANDCURRENTGROUP, m_fraSettings);
	DDX_Control(pDX, IDC_HIGHVOLTAGECONTROLLABEL2, m_lblMIN_0);
	DDX_Control(pDX, IDC_HIGHVOLTAGECONTROLLABEL3, m_lblMAX_0);
	DDX_Control(pDX, IDC_HIGHVOLTAGECONTROLMIN, m_lblMIN_1);
	DDX_Control(pDX, IDC_CURRENTCONTROLMIN, m_lblMIN_2);
	DDX_Control(pDX, IDC_HIGHVOLTAGECONTROLMAX, m_lblMAX_1);
	DDX_Control(pDX, IDC_CURRENTCONTROLMAX, m_lblMAX_2);
	DDX_Control(pDX, IDC_HIGHVOLTAGEMONITORTEXT, m_lblHvMonitor);
	DDX_Control(pDX, IDC_CURRENTMONITORTEXT, m_lblCurrentMonitor);
	DDX_Control(pDX, IDC_HIGHVOLTAGECONTROLLABEL, m_lblkV_0);
	DDX_Control(pDX, IDC_HIGHVOLTAGECONTROLLABEL4, m_lblkV_1);
	DDX_Control(pDX, IDC_CURRENTCONTROLLABEL, m_lbluA_0);
	DDX_Control(pDX, IDC_CURRENTCONTROLLABEL2, m_lbluA_1);
	DDX_Control(pDX, IDC_HIGHVOLTAGEMONITORLABEL, m_lbllblMonitor_0);
	DDX_Control(pDX, IDC_CURRENTMONITORLABEL, m_lbllblMonitor_1);
	DDX_Control(pDX, IDC_OUT_OF_RANGE_LBL, m_lblWattageOutOfRange);
	DDX_Control(pDX, IDC_HV_ON, m_cmdHvOn);
	DDX_Control(pDX, IDC_HV_OFF, m_cmdHvOff);
	DDX_Control(pDX, IDOK, m_cmdExit);
	DDX_Control(pDX, IDC_SETHIGHVOLTAGECONTROLEDIT, m_txtHv);
	DDX_Control(pDX, IDC_SETCURRENTCONTROLEDIT, m_txtCurrent);
	DDX_Control(pDX, IDC_SETHIGHVOLTAGEANDCURRENTBUTTON, m_cmdSetHVandCurrent);
	DDX_Control(pDX, IDC_INTERLOCK_OPEN, m_picInterLock_0);
	DDX_Control(pDX, IDC_INTERLOCK_CLOSED, m_picInterLock_1);
	DDX_Control(pDX, IDC_XRAY_ON_PIC, m_picXRayOn);
	DDX_Control(pDX, IDC_STATIC_ISO_CURVE_50KV, m_picIsoCurve_1);
	DDX_Control(pDX, IDC_INTERLOCK_SHORTED, m_picInterLock_2);
	DDX_Control(pDX, IDC_ADVANCED_BUTTON, m_cmdAdvanced);
	DDX_Control(pDX, IDC_STATUS_INFO_PIC, m_picStatusInfo);
	//	DDX_Control(pDX, IDC_STATIC_STATUS, lblStatus);
	//	DDX_Control(pDX, IDC_STATIC_MISC_STATUS, lblMiscStatus);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMiniXPage, CDialog)
	//{{AFX_MSG_MAP(CMiniXPage)
	ON_WM_TIMER()

	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
	ON_BN_CLICKED(IDC_ADVANCED_BUTTON, OnBnClickedAdvancedButton)
	ON_BN_CLICKED(IDC_HV_OFF, OnBnClickedHvOff)
	ON_BN_CLICKED(IDC_HV_ON, OnBnClickedHvOn)
	ON_BN_CLICKED(IDC_SETHIGHVOLTAGEANDCURRENTBUTTON, OnBnClickedSethighvoltageandcurrentbutton)
END_MESSAGE_MAP()

// CMiniXPage message handlers

BOOL CMiniXPage::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here

	ZeroMemory(m_picInterLock, sizeof(m_picInterLock));
	m_picInterLock[2] = &m_picInterLock_2;
	m_picInterLock[1] = &m_picInterLock_1;
	m_picInterLock[0] = &m_picInterLock_0;

	ZeroMemory(m_picIsoCurve, sizeof(m_picIsoCurve));
	m_picIsoCurve[1] = &m_picIsoCurve_1;
	m_picIsoCurve[0] = &m_picIsoCurve_0;

	ZeroMemory(m_lbllblMonitor, sizeof(m_lbllblMonitor));
	m_lbllblMonitor[1] = &m_lbllblMonitor_1;
	m_lbllblMonitor[0] = &m_lbllblMonitor_0;

	ZeroMemory(m_lbluA, sizeof(m_lbluA));
	m_lbluA[1] = &m_lbluA_1;
	m_lbluA[0] = &m_lbluA_0;

	ZeroMemory(m_lblkV, sizeof(m_lblkV));
	m_lblkV[1] = &m_lblkV_1;
	m_lblkV[0] = &m_lblkV_0;

	ZeroMemory(m_lblMAX, sizeof(m_lblMAX));
	m_lblMAX[2] = &m_lblMAX_2;
	m_lblMAX[1] = &m_lblMAX_1;
	m_lblMAX[0] = &m_lblMAX_0;

	ZeroMemory(m_lblMIN, sizeof(m_lblMIN));
	m_lblMIN[2] = &m_lblMIN_2;
	m_lblMIN[1] = &m_lblMIN_1;
	m_lblMIN[0] = &m_lblMIN_0;

	//Bmp_cmdHvOn.LoadBitmap(IDI_RAD32_HV_ON);
	//m_cmdHvOn.SetBitmap((HBITMAP)Bmp_cmdHvOn.GetSafeHandle());

	//Bmp_cmdHvOff.LoadBitmap(IDI_STOPSIGN);
	//m_cmdHvOff.SetBitmap((HBITMAP)Bmp_cmdHvOff.GetSafeHandle());

	//Bmp_cmdExit.LoadBitmap(IDI_EXIT32);
	//m_cmdExit.SetBitmap((HBITMAP)Bmp_cmdExit.GetSafeHandle());

	m_cmdHvOff.SetIcon(IDI_STOPSIGN, 32, 32);
	m_cmdHvOn.SetIcon(IDI_RAD32_HV_ON, 32, 32);
	m_cmdExit.SetIcon(IDI_EXIT32, 32, 32);

	m_txtHv.SetWindowText("15");
	m_txtCurrent.SetWindowText("15");
	m_tooltip.Create(this);
	m_tooltip.Activate(TRUE);
	m_tooltip.AddTool(&m_picInterLock_0, "INTERLOCK OPEN");
	m_tooltip.AddTool(&m_picInterLock_1, "INTERLOCK CLOSED");
	m_tooltip.AddTool(&m_picInterLock_2, "INTERLOCK SHORTED");

	fm_lblPower_mW.Set(GetDlgItem(IDC_POWER_MW), GetSysColor(COLOR_BTNTEXT), RGB(255,255,255));
	fm_lblMiniX2MainPanel.Set(GetDlgItem(IDC_MINIX2_MAIN_PANEL_LABEL), RGB(0,0,128), GetSysColor(COLOR_BTNFACE),"MS Sans Serif",12,true,ANSI_CHARSET);
	fm_fraSettings.Set(GetDlgItem(IDC_HIGHVOLTAGEANDCURRENTGROUP), GetSysColor(COLOR_BTNTEXT), GetSysColor(COLOR_BTNFACE),"System",9.75,true,ANSI_CHARSET);
	fm_lblWattageOutOfRange.Set(GetDlgItem(IDC_OUT_OF_RANGE_LBL), RGB(0,128,128), GetSysColor(COLOR_BTNFACE));

	m_pParentMX2 = (CMiniX2Dlg*)GetParent();

	sngMX2TubePowerMax=0.0;      //Tube Power Maximum assigned after tube type has been determined
	sngInterlockMinI=0.0;       //Interlock Minimum current assigned after tube type has been determined
	sngInterlockMaxI=0.0;        //Interlock Maximum current assigned after tube type has been determined

    tmrXRayOn_TimerId = 0;			//defined as tmrXRayOn_EventId == 10;
    tmrXRayOn_Interval = 1000;
    tmrXRayOn_Enabled = false;

    tmrMonitor_TimerId = 0;			//defined as tmrMonitor_EventId == 20;
    tmrMonitor_Interval = 500;
    tmrMonitor_Enabled = false;

    tmrInterLock_TimerId = 0;		//defined as tmrInterLock_EventId == 30;
    tmrInterLock_Interval = 4000;
    tmrInterLock_Enabled = false;
	HideIsoCurveAndMiniX();

	TimerControl(tmrMonitor_EventId,true);

	EnableMiniX2CommandButtons(mxcsMX2Ready);
	indOldInterlockStatus = ilvsHIDE;
	InterlockDisplay(indOldInterlockStatus);
	MiniX2.bHaveTubeType = false;
	MiniX2.bRequestTubeType = false;

	return TRUE;
}

void CMiniXPage::OnOK()
{
	return;		// there is no OnOk Button, disable return key exit
	//CWizardPage::OnOK();
}

void CMiniXPage::OnSetActive()
{
	m_pParent->SetTitle("Mini-X2 Controller");
}

void CMiniXPage::OnBnClickedOk()
{
    EnableMiniX2CommandButtons(mxcsMX2Exiting);
    if (MiniX2.STATUS_MNX.HV_EN) {
        SetHvAndI(false);
    }
	TimerControl(tmrMonitor_EventId,false);
	m_pParent->OnWizardFinishEx();
}

void CMiniXPage::OnBnClickedAdvancedButton()
{
    CWizardDialog* pSheet = (CWizardDialog*)GetParent();
	int iPage = pSheet->GetPageIndex(this);
	pSheet->SetActivePage(iPage + 1);

}

void CMiniXPage::OnTimer(UINT nIDEvent)
{
	bool bTubeSet=false;
	string strStatus("");
	string strTubeType("");
    if (nIDEvent == tmrInterLock_TimerId) {
		TimerControl(tmrInterLock_EventId,false);
        InterlockDisplay(ilvsHIDE);
    }
    if (nIDEvent == tmrMonitor_TimerId) {
		m_pParentMX2->SendCommand(XMTPT_SEND_STATUS);
		if (MiniX2.STATUS_MNX.bUSBError) {
			EnableMiniX2CommandButtons(mxcsMX2Ready);
			GetDlgItem(IDC_ADVANCED_BUTTON)->ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATUS_EDIT)->SetWindowText("USB Communications Error - Please Check Mini-X2 - Exit Then Restart");
		}
		UpdateMX2MonitoredDisplayValues();
		if (! tmrXRayOn_Enabled) {         //turn off indicators when not being used
			if (GetDlgItem(IDC_XRAY_ON_LABEL)->IsWindowVisible()) {
				GetDlgItem(IDC_XRAY_ON_LABEL)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_XRAY_ON_PIC)->ShowWindow(SW_HIDE);
		    }
		}
		m_pParentMX2->bSetAdvancedIndicators = true;		// new status, refresh the advanced dialog if open
		if (MiniX2.bRequestSerNumDisplay==true) {
			MiniX2.bRequestSerNumDisplay=false;
			GetDlgItem(IDC_SERIAL_NUMBER)->SetWindowText(MiniX2.strSerNumDisplay.c_str());     // display mx2 serial number
			EnableMiniX2CommandButtons(mxcsMX2Started);		// enable the Mini-X2 controls
		}
		if (MiniX2.bRequestTubeType) {
			MiniX2.bRequestTubeType = false;
			bTubeSet = SetMX2TubeType(MiniX2.TubeInterlockTable);
			if (bTubeSet) {
				strStatus = "Tube and Interlock Table Received, Tube Type: "; 
				strTubeType = MiniX2.MX2TubeTypeEnumToString(MiniX2.mx2ttTubeType);
				strStatus += strTubeType;
				strStatus += "\r\n";
				sngInterlockMinI = MiniX2.TubeInterlockTable.Interlock_min_I_mA;
				sngInterlockMaxI = MiniX2.TubeInterlockTable.Interlock_max_I_mA;
			} else {
				strStatus = "Tube and Interlock Table Received cannot identify tube type\r\n";
			}
		}
    }
    if (nIDEvent == tmrXRayOn_TimerId) { 
        GetDlgItem(IDC_XRAY_ON_LABEL)->ShowWindow(SW_SHOW);
        if (GetDlgItem(IDC_XRAY_ON_PIC)->IsWindowVisible()) {
            GetDlgItem(IDC_XRAY_ON_PIC)->ShowWindow(SW_HIDE);
        } else {
            GetDlgItem(IDC_XRAY_ON_PIC)->ShowWindow(SW_SHOW);
        }
    }
	CDialog::OnTimer(nIDEvent);
}


void CMiniXPage::TimerControl(UINT uEventId, bool isCreate)
{
    switch (uEventId) {
        case tmrXRayOn_EventId:
			if (isCreate) {
	            tmrXRayOn_TimerId = SetTimer(tmrXRayOn_EventId, tmrXRayOn_Interval, NULL);
            } else {
                KillTimer(tmrXRayOn_TimerId);
			}
	        tmrXRayOn_Enabled = isCreate;
            break;
        case tmrMonitor_EventId:
            if (isCreate) {
	            tmrMonitor_TimerId = SetTimer(tmrMonitor_EventId, tmrMonitor_Interval, NULL);
            } else {
                KillTimer(tmrMonitor_TimerId);
			}
	        tmrMonitor_Enabled = isCreate;
            break;
        case tmrInterLock_EventId:
             if (isCreate) {
	            tmrInterLock_TimerId = SetTimer(tmrInterLock_EventId, tmrInterLock_Interval, NULL);
            } else {
                KillTimer(tmrInterLock_TimerId);
			}
	        tmrInterLock_Enabled = isCreate;
            break;
    }
}

//=======================================================================================================
// ==== Mini-X2 Interlock Control =======================================================================
//=======================================================================================================
void CMiniXPage::InterlockDisplay(InterlockViewState ILockView) {
	GetDlgItem(IDC_INTERLOCK_LABEL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INTERLOCK_OPEN)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INTERLOCK_CLOSED)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_INTERLOCK_SHORTED)->ShowWindow(SW_HIDE);
    if(ILockView == ilvsOPEN) {
		GetDlgItem(IDC_INTERLOCK_LABEL)->SetWindowText("INTERLOCK OPEN");
		GetDlgItem(IDC_INTERLOCK_LABEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_INTERLOCK_OPEN)->ShowWindow(SW_SHOW);
		TimerControl(tmrInterLock_EventId,false);
	} else if (ILockView == ilvsCLOSED) {
		GetDlgItem(IDC_INTERLOCK_LABEL)->SetWindowText("INTERLOCK CLOSED");
		GetDlgItem(IDC_INTERLOCK_LABEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_INTERLOCK_CLOSED)->ShowWindow(SW_SHOW);
		TimerControl(tmrInterLock_EventId,true);
	} else if(ILockView == ilvsSHORTED) {
		GetDlgItem(IDC_INTERLOCK_LABEL)->SetWindowText("INTERLOCK SHORTED");
		GetDlgItem(IDC_INTERLOCK_LABEL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_INTERLOCK_SHORTED)->ShowWindow(SW_SHOW);
 		TimerControl(tmrInterLock_EventId,false);
   } else if(ILockView == ilvsHIDE) {
		//GetDlgItem(IDC_INTERLOCK_LABEL)->SetWindowText("INTERLOCK STATUS");
		//TimerControl(tmrInterLock_EventId,false);
	}
}

void CMiniXPage::InterLockStatusTest()
{
	string strStatus;
    bool bUpdateInterlock=true;
	//INTERLOCK OPEN
	//INTERLOCK CLOSED
	//INTERLOCK SHORTED
	//12345678901234567890
	string strILOCK("");
	string strCmp("");

	strILOCK.reserve(20);

    indOldInterlockStatus = MiniX2.indInterlockStatus;
    if ((MiniX2.STATUS_MNX.LAMP_I >= sngInterlockMinI) && (MiniX2.STATUS_MNX.LAMP_I <= sngInterlockMaxI)) {
        MiniX2.indInterlockStatus =  ilvsCLOSED;      //interlock closed
    } else if (MiniX2.STATUS_MNX.LAMP_I < sngInterlockMinI) {
        MiniX2.indInterlockStatus = ilvsOPEN;      //interlock open
    } else if (MiniX2.STATUS_MNX.LAMP_I > sngInterlockMaxI) {
        MiniX2.indInterlockStatus = ilvsSHORTED;      //interlock shorted
    } else {
        MiniX2.indInterlockStatus = ilvsHIDE;     //interlock status unknown
    }
    if (indOldInterlockStatus != MiniX2.indInterlockStatus) {
        bUpdateInterlock = true;
    } else {
		switch (MiniX2.indInterlockStatus) {
            case ilvsCLOSED:
				GetDlgItem(IDC_INTERLOCK_LABEL)->GetWindowText((char*)strILOCK.c_str(),20);
				strCmp = strILOCK.c_str();
				if (strCmp.compare("INTERLOCK CLOSED") == 0) {
                    bUpdateInterlock = false;
                }
				break;
            case ilvsOPEN:
                if (GetDlgItem(IDC_INTERLOCK_OPEN)->IsWindowVisible()) {
                    bUpdateInterlock = false;
                }
				break;
            case ilvsSHORTED:
                if (GetDlgItem(IDC_INTERLOCK_SHORTED)->IsWindowVisible()) {
                    bUpdateInterlock = false;
                }
				break;
             default:
                bUpdateInterlock = true;
 				break;
       }
    }
    
    if (bUpdateInterlock) {
        if (MiniX2.indInterlockStatus == ilvsOPEN) {
			EnableMiniX2CommandButtons(mxcsMX2Ready);
			if (MiniX2.STATUS_MNX.HV_EN) {
				SetHvAndI(false);
			}			
			InterlockDisplay(ilvsOPEN);
			TimerControl(tmrInterLock_EventId,false);
			indOldInterlockStatus = MiniX2.indInterlockStatus;
        } else if (MiniX2.indInterlockStatus == ilvsCLOSED) {
			InterlockDisplay(ilvsCLOSED);
			TimerControl(tmrInterLock_EventId,true);
 			indOldInterlockStatus = MiniX2.indInterlockStatus;
			EnableMiniX2CommandButtons(mxcsMX2Started);
       } else if (MiniX2.indInterlockStatus == ilvsSHORTED) {
			EnableMiniX2CommandButtons(mxcsMX2Ready);
			if (MiniX2.STATUS_MNX.HV_EN) {
				SetHvAndI(false);
			}			
			InterlockDisplay(ilvsOPEN);
			InterlockDisplay(ilvsSHORTED);
			TimerControl(tmrInterLock_EventId,false);
			indOldInterlockStatus = MiniX2.indInterlockStatus;
		} else {
			InterlockDisplay(ilvsHIDE);
			TimerControl(tmrInterLock_EventId,false);
		}
    }
}

void CMiniXPage::UpdateMX2MonitoredDisplayValues()
{
    float sngTubePower;
	string strMiniX2Fault("");
    if (MiniX2.STATUS_MNX.HV_EN != tmrXRayOn_Enabled) {
		TimerControl(tmrXRayOn_EventId,MiniX2.STATUS_MNX.HV_EN);
    }
    sngTubePower = (MiniX2.STATUS_MNX.HV_MON * MiniX2.STATUS_MNX.I_MON);
    DisplayDouble(IDC_HIGHVOLTAGEMONITORTEXT,MiniX2.STATUS_MNX.HV_MON,1);
    DisplayDouble(IDC_CURRENTMONITORTEXT,MiniX2.STATUS_MNX.I_MON,1);
    DisplayDouble(IDC_TEMPERATURE_SENSOR,MiniX2.STATUS_MNX.Temp,0);
    DisplayDouble(IDC_POWER_MW,sngTubePower,0);

    if (sngTubePower > sngMX2TubePowerMax) {		// Power Out Of Range
		GetDlgItem(IDC_OUT_OF_RANGE_LBL)->ShowWindow(SW_SHOW);
    } else {
		GetDlgItem(IDC_OUT_OF_RANGE_LBL)->ShowWindow(SW_HIDE);
    }
    InterLockStatusTest();
    strMiniX2Fault = MiniX2.MiniX2FaultToString(MiniX2.STATUS_MNX.INTERLOCK_STATE);
	GetDlgItem(IDC_FAULT_DISPLAY_LABEL)->SetWindowText(strMiniX2Fault.c_str());
}

void CMiniXPage::DisplayDouble(int DlgItem, double dblValue, int dpDigits=0)
{
    string strValue;
    string strFormat;
	stringex strfn;
    strFormat = strfn.Format("%%.%df",dpDigits);           // get decimal point precision
    strValue = strfn.Format(strFormat.c_str(),dblValue);          // format data
    GetDlgItem(DlgItem)->SetWindowText(strValue.c_str());  // display in control
}

void CMiniXPage::HideIsoCurveAndMiniX()
{
	GetDlgItem(IDC_STATIC_ISO_CURVE_50KV_4W)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ISO_CURVE_50KV_10W)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_STATIC_ISO_CURVE_70KV_10W)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_HW_IMAGE_50KV_4W)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_HW_IMAGE_50KV_10W)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_HW_IMAGE_70KV_10W)->ShowWindow(SW_HIDE);
}

void CMiniXPage::DisplayIsoCurveAndMiniX(MX2TubeTypeEnum iMX)
{
	HideIsoCurveAndMiniX();
	UpdateData(false);
	UpdateWindow();
	switch (iMX) {
		case mx2tubetype50kV4W:
			GetDlgItem(IDC_HW_IMAGE_50KV_4W)->ShowWindow(SW_SHOW);
			UpdateData(false);
			UpdateWindow();
			GetDlgItem(IDC_STATIC_ISO_CURVE_50KV_4W)->ShowWindow(SW_SHOW);
			UpdateData(false);
			UpdateWindow();
			break;
		case mx2tubetype50kV10W:
			GetDlgItem(IDC_HW_IMAGE_50KV_10W)->ShowWindow(SW_SHOW);
			UpdateData(false);
			UpdateWindow();
			GetDlgItem(IDC_STATIC_ISO_CURVE_50KV_10W)->ShowWindow(SW_SHOW);
			UpdateData(false);
			UpdateWindow();
			break;
		case mx2tubetype70kV10W:
			GetDlgItem(IDC_HW_IMAGE_70KV_10W)->ShowWindow(SW_SHOW);
			UpdateData(false);
			UpdateWindow();
			GetDlgItem(IDC_STATIC_ISO_CURVE_70KV_10W)->ShowWindow(SW_SHOW);
			UpdateData(false);
			UpdateWindow();
			break;
		case mx2tubetypeNONE:
			break;
		default:
			break;
	}		
}

//=======================================================================================================
// ==== Mini-X2 Command Button Handlers =================================================================
//=======================================================================================================
//GetDlgItem(IDC_SETHIGHVOLTAGEANDCURRENTBUTTON)->EnableWindow(indSetHVandCurrent);
//GetDlgItem(IDOK)->EnableWindow(indExit);
//GetDlgItem(IDC_HV_ON)->EnableWindow(false);
void CMiniXPage::OnBnClickedHvOff()
{
	CMSecTimer tmr;
////    indDisableMonitorCmds = true;
////    GetDlgItem(IDC_HV_OFF)->EnableWindow(false);
	GetDlgItem(IDC_HV_OFF)->EnableWindow(false);
    SetHvAndI(false);
	tmr.msTimer(100);
	m_pParentMX2->SetFlashWindow(FLASHW_STOP);
    EnableMiniX2CommandButtons(mxcsMX2HVOff);
////    indDisableMonitorCmds = false;
}

void CMiniXPage::OnBnClickedHvOn()
{
	CMSecTimer tmr;
    int msgResponse;
////    indDisableMonitorCmds = true;
	GetDlgItem(IDC_HV_ON)->EnableWindow(false);
    msgResponse = AfxMessageBox("Turn X-RAY High Voltage ON?",MB_YESNO | MB_ICONQUESTION | MB_TOPMOST);
    if (msgResponse == IDYES) {
		m_pParentMX2->SetFlashWindow(FLASHW_ALL | FLASHW_TIMER);		// flash window and taskbar
        CheckHvAndCurrentDisplayLimits();
        SetHvAndI();
        tmr.msTimer(100);
        EnableMiniX2CommandButtons(mxcsMX2HVOn);
	} else {
		OnBnClickedHvOff();
	}

////    indDisableMonitorCmds = false;
}

void CMiniXPage::OnBnClickedSethighvoltageandcurrentbutton()
{
	CMSecTimer tmr;
////    double dblValue;
////    MiniX_Settings MiniXSettings;
////    indDisableMonitorCmds = true;
////	GetDlgItem(IDC_SETHIGHVOLTAGEANDCURRENTBUTTON)->EnableWindow(false);
////	GetDlgItem(IDC_HV_ON)->EnableWindow(false);
////	GetDlgItem(IDC_HV_OFF)->EnableWindow(false);
    EnableMiniX2CommandButtons(mxcsMX2Waiting);
    CheckHvAndCurrentDisplayLimits();
////        dblValue = GetWindowDouble(IDC_SETHIGHVOLTAGECONTROLEDIT);  //sent hv value
////        SetMiniXHV(dblValue);
////        Sleep(100);
////        dblValue = GetWindowDouble(IDC_SETCURRENTCONTROLEDIT);      //send current value
////        SetMiniXCurrent(dblValue);
////        Sleep(100);
////        SendMiniXCommand((byte)mxcSetHVandCurrent);
////        Sleep(1000);
////        ReadMiniXSettings(&MiniXSettings);     //read the corrected values back
////        DisplayDouble(IDC_SETHIGHVOLTAGECONTROLEDIT,MiniXSettings.HighVoltage_kV,0);
////        DisplayDouble(IDC_SETCURRENTCONTROLEDIT,MiniXSettings.Current_uA,0);
    SetHvAndI();
    tmr.msTimer(100);
    EnableMiniX2CommandButtons(mxcsMX2HVOn);
////    indDisableMonitorCmds = false;
}

//void CMiniXPage::OnBnClickedButton4()
//{
//	// TODO: Add your control notification handler code here
//	GetDlgItem(IDC_INTERLOCK_LABEL)->ShowWindow(SW_SHOW);
//	GetDlgItem(IDC_INTERLOCK_LABEL)->SetWindowText("INTERLOCK LABEL");
//}

void CMiniXPage::ShowMX2Status(string strStatus)
{
	GetDlgItem(IDC_STATUS_EDIT)->SetWindowText(strStatus.c_str());     // display mx2 status
}


//Turns tube on/off, sets high voltage and current
//defaults to tube off if no bTubeOn is specified
//reads values to send from user HV (txtHv) and uA (txtCurrent)
//also resets user values if out of range
//HV and UA min/max values were read from the tube table
void CMiniXPage::SetHvAndI(bool bTubeOn)
{
    CMSecTimer tmr;
    float sngHV;
    float sngI;
    string strHV;
    string strI;
    bool bValueChanged;
    //string strStatus;
   
    if (bTubeOn) {
        sngHV = GetWindowFloat(IDC_SETHIGHVOLTAGECONTROLEDIT);
        strHV = FloatToString(sngHV,1);
        sngI = GetWindowFloat(IDC_SETCURRENTCONTROLEDIT);
        strI = FloatToString(sngI,1);
    } else {
        sngHV = 0.0;
        sngI = 0.0;
        strHV = "0.0";
        strI = "0.0";
    }
    //strStatus = FloatToString(sngHV,1) + "kV : " + FloatToString(sngI,1) + "uA";
    m_pParentMX2->SendMX2_HVandI(strHV, strI);
    tmr.msTimer(100);
    bValueChanged = m_pParentMX2->ReadbackMX2_HVandI(&sngHV, &sngI);
    //strStatus = FloatToString(sngHV,1) + "kV : " + FloatToString(sngI,1) + "uA";
    //strStatus = FloatToString(sngHV, 1) + "kV" + " : " + FloatToString(sngI, 1) + "uA";
    if (bValueChanged) {
        strHV = FloatToString(sngHV, 1);
		DisplayDouble(IDC_SETHIGHVOLTAGECONTROLEDIT, sngHV, 1);
        strI = FloatToString(sngI, 1);
		DisplayDouble(IDC_SETCURRENTCONTROLEDIT, sngI, 1);
    }
}


void CMiniXPage::EnableMiniX2CommandButtons(unsigned char mxmEnabledCmds)
{
    int indStartMiniX;
    int indHVOn;
    int indHVOff;
    int indSetHVandCurrent;
    int indExit;
    int indMonitor;

    indStartMiniX = CBool(mxmEnabledCmds & 0x01);
    indHVOn = CBool(mxmEnabledCmds & 0x02);
    indHVOff = CBool(mxmEnabledCmds & 0x04);
    indSetHVandCurrent = CBool(mxmEnabledCmds & 0x08);
    indExit = CBool(mxmEnabledCmds & 0x10);
    indMonitor = CBool(mxmEnabledCmds & 0x20);

	GetDlgItem(IDC_HV_ON)->EnableWindow(indHVOn);
	GetDlgItem(IDC_HV_OFF)->EnableWindow(indHVOff);
	GetDlgItem(IDC_SETHIGHVOLTAGEANDCURRENTBUTTON)->EnableWindow(indSetHVandCurrent);
	GetDlgItem(IDOK)->EnableWindow(indExit);

}

int CMiniXPage::CBool(unsigned char mxmEnabledCmds)
{
	if (mxmEnabledCmds == 0) {
		return(0);
	}  else {
		return(1);
	}
}

string CMiniXPage::FloatToString(float fltValue, int dpDigits=0)
{
    string strValue;
    string strFormat;
	stringex strfn;
    strFormat = strfn.Format("%%.%df",dpDigits);           // get decimal point precision
    strValue = strfn.Format(strFormat.c_str(),fltValue);          // format data
	return(strValue);
}


float CMiniXPage::GetWindowFloat(int nID)
{
    string strValue;
    float fltValue;
	char chValue[20];
	stringex strfn;
    GetDlgItem(nID)->GetWindowText(chValue,20);
	strValue = strfn.Format("%s",chValue);
    fltValue = (float)atof(strValue.c_str());
    return fltValue;
}

//Always check upper limits
//if limit override, allow values below min
//check if power is within iso range
void CMiniXPage::CheckHvAndCurrentDisplayLimits()
{
    float sngKV;         //display voltage
    float sngI;          //display current
    bool bLimits;
    float sngPower;
    float sngNewI;
    float sngKV_old;         //display voltage
    float sngI_old;          //display current
    
    float sngMinKV;
    float sngMaxKV;
    float sngMinI;
    float sngMaxI;
    float sngMaxPower;

	string strStatus;
	stringex strfn;
	string strTemp;

    sngKV = GetWindowFloat(IDC_SETHIGHVOLTAGECONTROLEDIT);                   //display values controlling tube
    sngI = GetWindowFloat(IDC_SETCURRENTCONTROLEDIT);
    
    sngKV_old = sngKV;
    sngI_old = sngI;
    
    sngMinKV = MiniX2.TubeInterlockTable.HVmin_kV;          //min values
    sngMinI = MiniX2.TubeInterlockTable.Imin_uA;
    sngMaxKV = MiniX2.TubeInterlockTable.HVmax_kV;          //max values
    sngMaxI = MiniX2.TubeInterlockTable.Imax_uA;
    if (sngKV > sngMaxKV) { sngKV = sngMaxKV; }     //test and fix upper limits
    if (sngI > sngMaxI) { sngI = sngMaxI; }
     
    strStatus = "kV: " + FloatToString(sngKV, 1) + "   uA: " + FloatToString(sngI, 1);

    bLimits = (!MiniX2.STATUS_MNX.DISABLE_LIMIT_CHECKS);
    if (bLimits) {                               //if the limit overide is not set, test the lower limits too
		if (sngKV < sngMinKV) { sngKV = sngMinKV; }
		if (sngI < sngMinI) { sngI = sngMinI; }
    } else {                                            //never let the values go below zero
		if (sngKV < 0) { sngKV = sngMinKV; }
		if (sngI < 0) { sngI = sngMinI; }
    }
    
    strStatus = "kV: " + FloatToString(sngKV, 1) + "   uA: " + FloatToString(sngI, 1);

    sngMaxPower = (float)(MiniX2.TubeInterlockTable.Pmax_Anode_W * 1000.0);   //test power
    sngPower = sngKV * sngI;
    if (sngPower > sngMaxPower) {                //lower the current to within range
        if (sngKV > 0) {
            sngNewI = sngMaxPower / sngKV;
			strTemp = strfn.Format("%0.1f", sngNewI);
			sngI = (float)atof(strTemp.c_str());
            //============================================
            sngPower = sngKV * sngI;
            if (sngPower > sngMaxPower) {                //lower the current to within range
                if (sngKV > 0) {
                    sngNewI = (float)(sngMaxPower * 0.98) / sngKV;
					strTemp = strfn.Format("%0.1f", sngNewI);
					sngI = (float)atof(strTemp.c_str());
                }
            }
            //============================================
        }
    }

    strStatus = "kV: " + FloatToString(sngKV, 1) + "   uA: " + FloatToString(sngI, 1);
    
    if (sngKV_old != sngKV) {                            //return new display values for controlling tube
		DisplayDouble(IDC_SETHIGHVOLTAGECONTROLEDIT, sngKV, 1);
    }
    if (sngI_old != sngI) {
		DisplayDouble(IDC_SETCURRENTCONTROLEDIT, sngI, 1);
    }
}

//void CMiniXPage::EnableMiniX_Commands(byte mxmEnabledCmds)
//{
//    bool indStartMiniX;
//    bool indHVOn;
//    bool indHVOff;
//    bool indSetHVandCurrent;
//
//    //if (! MiniX2.indDisableMonitorCmds) {
//        indStartMiniX = ((mxmEnabledCmds & mxcStartMiniX) == mxcStartMiniX);
//        indHVOn = ((mxmEnabledCmds & mxcHVOn) == mxcHVOn);
//        indHVOff = ((mxmEnabledCmds & mxcHVOff) == mxcHVOff);
//        indSetHVandCurrent = ((mxmEnabledCmds & mxcSetHVandCurrent) == mxcSetHVandCurrent);
//        //GetDlgItem(IDC_START_MINIX_CONTROLLER)->EnableWindow(indStartMiniX);
//	    GetDlgItem(IDC_HV_ON)->EnableWindow(indHVOn);
//	    GetDlgItem(IDC_HV_OFF)->EnableWindow(indHVOff);
//	    GetDlgItem(IDC_SETHIGHVOLTAGEANDCURRENTBUTTON)->EnableWindow(indSetHVandCurrent);
//    //}
//}

//void CMiniXPage::ShowStatus(string strStatus)
//{
//	//GetDlgItem(IDC_STATIC_STATUS)->SetWindowText(strStatus.c_str());
//}

bool CMiniXPage::SetMX2TubeType(TubeInterlockTableType TubeInterlockTable)
{
	bool bSetMX2TubeType;
    float sngHV;
    float sngWatt;
	string strDisplay("");
	stringex strfn;

	double dblDefaultdblHighVoltage=15.0;		// Default High Voltage kV
	double dblDefaultCurrent=15.0;				// Default Current

    bSetMX2TubeType = false;
    sngHV = TubeInterlockTable.HVmax_kV;
    sngWatt = TubeInterlockTable.Pmax_Anode_W;
    if ((sngHV == 50.0) && (sngWatt == 4.0)) {
        MiniX2.mx2ttTubeType = mx2tubetype50kV4W;
		MiniX2.WarmUpTableTypeMX2 = mx2tbltyp50kV4W;
		DisplayIsoCurveAndMiniX(mx2tubetype50kV4W);
        sngMX2TubePowerMax = 4000.0;
        MiniX2.bHaveTubeType = true;
    } else if ((sngHV == 50.0) && (sngWatt == 10.0)) {
        MiniX2.mx2ttTubeType = mx2tubetype50kV10W;
		MiniX2.WarmUpTableTypeMX2 = mx2tbltyp50kV10W;
        DisplayIsoCurveAndMiniX(mx2tubetype50kV10W);
        sngMX2TubePowerMax = 10000.0;
        MiniX2.bHaveTubeType = true;
    } else if ((sngHV == 70.0) && (sngWatt == 10.0)) {
        MiniX2.mx2ttTubeType = mx2tubetype70kV10W;
		MiniX2.WarmUpTableTypeMX2 = mx2tbltyp70kV10W;
        DisplayIsoCurveAndMiniX(mx2tubetype70kV10W);
        sngMX2TubePowerMax = 10000.0;
        MiniX2.bHaveTubeType = true;
    } else {
 		HideIsoCurveAndMiniX();
		MiniX2.bHaveTubeType = false;
        sngMX2TubePowerMax = 4000.0;
		MiniX2.mx2ttTubeType = mx2tubetypeNONE;
		MiniX2.WarmUpTableTypeMX2 = mx2tbltypCustom;
    }
    if (MiniX2.bHaveTubeType) {
		strDisplay = strfn.Format("%.0f",TubeInterlockTable.HVmin_kV);
		GetDlgItem(IDC_HIGHVOLTAGECONTROLMIN)->SetWindowText(strDisplay.c_str());
		strDisplay = strfn.Format("%.0f",TubeInterlockTable.HVmax_kV);
		GetDlgItem(IDC_HIGHVOLTAGECONTROLMAX)->SetWindowText(strDisplay.c_str());

		strDisplay = strfn.Format("%.1f",dblDefaultdblHighVoltage);
		GetDlgItem(IDC_SETHIGHVOLTAGECONTROLEDIT)->SetWindowText(strDisplay.c_str());
		//GetDlgItem(IDC_HIGHVOLTAGEMONITORTEXT)->SetWindowText(strDisplay.c_str());

		strDisplay = strfn.Format("%.0f",TubeInterlockTable.Imin_uA);
		GetDlgItem(IDC_CURRENTCONTROLMIN)->SetWindowText(strDisplay.c_str());
		strDisplay = strfn.Format("%.0f",TubeInterlockTable.Imax_uA);
		GetDlgItem(IDC_CURRENTCONTROLMAX)->SetWindowText(strDisplay.c_str());

		strDisplay = strfn.Format("%.1f",dblDefaultCurrent);
		GetDlgItem(IDC_SETCURRENTCONTROLEDIT)->SetWindowText(strDisplay.c_str());
		//GetDlgItem(IDC_CURRENTMONITORTEXT)->SetWindowText(strDisplay.c_str());

    }
    bSetMX2TubeType = MiniX2.bHaveTubeType;
	return(bSetMX2TubeType);
}




