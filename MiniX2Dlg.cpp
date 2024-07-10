// MiniX2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "vcMiniX2.h"
#include "MiniX2Dlg.h"
#include "WizardPage.h"
#include ".\minix2dlg.h"
#include "DppWinUSB.h"
#include "DP5Protocol.h"
#include "WinusbNotify.h"
#include "SProfile.h"
#include "SendCommand.h"
#include "DebugUtils.h" // MINE - ALLOWS PrintToOutputWindow 


#include ".\msectimer.h"
//#include ".\waitdlg\CmdWaitDlg.h"
//#include "UserMsgs.h"
//#include "FileBrowse.h"
//#include "MsgBox.h"
//#include "AsciiCmdUtilities.h"
////#include <math.h>
//#include "AdvancedPage.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



CWinusbNotify WinusbNotify;
CDppWinUSB DppWinUSB;		// WinUSB
CSendCommand SndCmd;
CDP5Protocol DP5Proto;
CSProfile Profile;
CMiniX2 MiniX2;
CParsePacket ParsePkt;


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiniX2Dlg dialog


CMiniX2Dlg::CMiniX2Dlg(CWnd* pParent /*=NULL*/)
	: CWizardDialog(CMiniX2Dlg::IDD, pParent)

{
	//{{AFX_DATA_INIT(CMiniX2Dlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    PrintToOutputWindow("CMiniX2Dlg - MiniXDlg.cpp\n");
}

void CMiniX2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMiniX2Dlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMiniX2Dlg, CWizardDialog)
	//{{AFX_MSG_MAP(CMiniX2Dlg)
	ON_WM_SYSCOMMAND()



	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMiniX2Dlg message handlers

BOOL CMiniX2Dlg::OnInitDialog() 
{
    PrintToOutputWindow("OnInitDialog - MiniXDlg.cpp\n");

	SetPlaceholderID(IDC_SHEETRECT);
	CWizardDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon


	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString cstrAboutMenu;
		cstrAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!cstrAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, cstrAboutMenu);
		}
	}

	// no device selected upon startup
	DppWinUSB.isDppFound = FALSE;
	bAdvancedStatus=false;        //Only display status values in advanced tab when requested
	bAdvancedInterlock=false;     //Only display interlock values in advanced tab when requested
	bAdvancedTimeStamp=false;     //Only display Timestamp values in advanced tab when requested
	bMiniX2ControllerRunning = false;
	MiniX2.STATUS_MNX.bUSBError = false;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CMiniX2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

//void CvcDP5Dlg::OnDestroy()
//{
//	isDppConnected = FALSE;
//	CDialog::OnDestroy();
//	CMSecTimer tmr;
//	Profile.SaveProfile();
//	OnResetCommunications();
//}

void CMiniX2Dlg::RemCallParsePacket(BYTE PacketIn[])
{
    ParsePkt.DppState.ReqProcess = ParsePkt.ParsePacket(PacketIn, &DP5Proto.PIN);
    ParsePacketEx(DP5Proto.PIN, ParsePkt.DppState);
}

void CMiniX2Dlg::ParsePacketEx(Packet_In PIN, DppStateType DppState)
{
    switch (DppState.ReqProcess) {
//        case preqProcessStatus:
//			MiniX2.isMiniX2 = false;
//            ProcessStatusEx(PIN, DppState);
//            break;
        case preqProcessStatusMX2:
			MiniX2.isMiniX2 = true;
            ProcessStatusExMX2(PIN, DppState);
            break;
        case preqProcessTextData:
            ProcessTextDataEx(PIN, DppState);
            break;
        case preqProcessCfgRead:
            if (MiniX2.isMiniX2) {
                ProcessCfgReadMX2Ex(PIN, DppState);
            }
            break;
///====================================================================
//       Process Mini-X2 Receive Packets
//====================================================================
        case preqProcessTubeInterlockTableMX2:
            ProcessTubeInterlockTableMX2Ex(PIN, DppState);
            break;
        case preqProcessWarmupTableMX2:
            ProcessWarmupTableMX2Ex(PIN, DppState);
            break;
        case preqProcessTimestampRecordMX2:
            ProcessTimestampRecordMX2Ex(PIN, DppState);
            break;
        case preqProcessFaultRecordMX2:
            ProcessFaultRecordMX2Ex(PIN, DppState);
            break;
        case preqProcessAck:
            ProcessAck(PIN.PID2);
            break;
//        case preqProcessError:
//            DisplayError(PIN, DppState);
//            break;
        default:
            //do nothing
            break;
    }
}

//void CvcDP5Dlg::DisplayError(Packet_In PIN, DppStateType DppState)
//{
//	string strError;
//	stringex strfn;
//	ShowStatus(ParsePkt.PID2_TextToString("Received Packet", PIN.STATUS));
//    // bad PID, assigned by ParsePacket
//    if (PIN.STATUS == PID2_ACK_PID_ERROR) {
//		strError = strfn.Format("Received packet: PID1=0x.2X PID1=0x.2X LEN=%d",PIN.PID1,PIN.PID2,PIN.LEN);
//		ShowStatus(strError);
//    }
//}

void CMiniX2Dlg::ProcessAck(BYTE PID2) 
{
    //ShowStatus(ParsePkt.PID2_TextToString("ACK", PID2));
}

//void CvcDP5Dlg::ProcessStatusEx(Packet_In PIN, DppStateType DppState)
//{
//    long idxStatus;
//	string strStatus;
//
//	TimerControl(tmrMonitor_EventId,false);
//	TimerControl(tmrXRayOn_EventId,false);
//	TimerControl(tmrInterLock_EventId,false);
//
//	for(idxStatus=0;idxStatus<64;idxStatus++) {
//        DP5Stat.m_DP5_Status.RAW[idxStatus] = PIN.DATA[idxStatus];
//	}
//    DP5Stat.Process_Status(&DP5Stat.m_DP5_Status);
//	strStatus = DP5Stat.GetStatusValueStrings(DP5Stat.m_DP5_Status);
//	GetDlgItem(IDC_STATIC_SPECTRUM_STATUS)->SetWindowText(strStatus.c_str());
//
//
//    ShowStatus("Received Status");
//}

void CMiniX2Dlg::ProcessTextDataEx(Packet_In PIN, DppStateType DppState)
{
    long idxTextData;
	string strTextData;
	string strCh;
	stringex strfn;	// = strfn.

    strTextData = "";
	for(idxTextData=0;idxTextData<512;idxTextData++) {
		strCh = strfn.Format("%c",PIN.DATA[idxTextData]);
        strTextData += strCh;
	}
	if (bAdvancedStatus) {
		bAdvancedStatus = false;
		strMiscDataAdv = strTextData;
	}
}

void CMiniX2Dlg::SendCommand(TRANSMIT_PACKET_TYPE XmtCmd)
{
    BOOL HaveBuffer;
    BOOL SentPkt;

    HaveBuffer = SndCmd.DP5_CMD(DP5Proto.BufferOUT, XmtCmd);
    if (HaveBuffer) {
        SentPkt = DppWinUSB.SendPacketUSB(DP5Proto.BufferOUT, DppWinUSB.devInfo, DP5Proto.PacketIn);
        if (SentPkt) {
            RemCallParsePacket(DP5Proto.PacketIn);
		} else {
            MiniX2.STATUS_MNX.bUSBError = true; 
        }
    }
}

void CMiniX2Dlg::SendCommandData(TRANSMIT_PACKET_TYPE XmtCmd, BYTE DataOut[])
{
    BOOL HaveBuffer;
    BOOL SentPkt;

    HaveBuffer = SndCmd.DP5_CMD_Data(DP5Proto.BufferOUT, XmtCmd, DataOut);
    if (HaveBuffer) {
        SentPkt = DppWinUSB.SendPacketUSB(DP5Proto.BufferOUT, DppWinUSB.devInfo, DP5Proto.PacketIn);
        if (SentPkt) {
            RemCallParsePacket(DP5Proto.PacketIn);
		} else {
            MiniX2.STATUS_MNX.bUSBError = true;
        }
    }
}

void CMiniX2Dlg::OnBnClickedButtonReadMiscData()
{
	SendCommand(XMTPT_SEND_512_BYTE_MISC_DATA);
}

void CMiniX2Dlg::OnBnClickedButtonWriteMiscData()
{
	string strMiscData;
	//char charMiscData[512+1];
	stringex strfn;
	BYTE byteMiscData[512];
	int idxCh;
	int DataLen;
	strMiscData.reserve(512+1);
	
	if (bAdvancedStatus) {
		bAdvancedStatus = false;
		strMiscData = strMiscDataAdv;
	} else {
		//memset(&charMiscData,0,sizeof(charMiscData));
		//GetDlgItem(IDC_EDIT_MISC_DATA)->GetWindowText(charMiscData, 512+1);
		//strMiscData = strfn.Format("%s",charMiscData);
		return;
	}
	DataLen = (int)strMiscData.length();
	for(idxCh=0;idxCh<512;idxCh++) {
		if (idxCh < DataLen) {
			byteMiscData[idxCh] = (BYTE)strMiscData.at(idxCh);
		} else {
			byteMiscData[idxCh] = 0;
		}
	}
    SendCommandData(XMTPT_WRITE_512_BYTE_MISC_DATA, byteMiscData);
}

//// sets/returns if DPP is connected based on USB Interface and isDppFound[]
//BOOL CvcDP5Dlg::isConnectedTest() 
//{
//	BOOL isDppFound;
//	CMSecTimer tmr;
//
//	isDppFound = DppWinUSB.isDppFound;
//	isDppConnected = isDppFound;
//	if (isDppConnected ) {
//		SendCommand(XMTPT_SEND_STATUS); // request status
//		tmr.msTimer(500);
//		Profile.device_type = DP5Stat.m_DP5_Status.DEVICE_ID;
//		DeviceType = DP5Stat.m_DP5_Status.DEVICE_ID;
//		return isDppFound;
//	} else {
//		isDppConnected = FALSE;
//		return FALSE;
//	}
//}

string CMiniX2Dlg::GetCmdData(string strCmd, string strCfgData)
{
	int iStart,iEnd,iCmd;
	string strCmdData;

	strCmdData = "";
	if (strCfgData.length() < 7) { return strCmdData; }	// no data
	if (strCmd.length() != 4) {	return strCmdData; }		// bad command
	iCmd = (int)strCfgData.find(strCmd+"=",0);
	if (iCmd == -1) { return strCmdData; }			// cmd not found	
	iStart = (int)strCfgData.find("=",iCmd);						 
	if (iStart == -1) { return strCmdData; }		// data start not found	
	iEnd = (int)strCfgData.find(";",iCmd);
	if (iEnd == -1) {return strCmdData; }			// data end found	
	if (iStart >= iEnd) { return strCmdData; }		// data error
	strCmdData = strCfgData.substr(iStart+1,(iEnd - (iStart+1)));
	return strCmdData;
}

//void CvcDP5Dlg::OnBnClickedOk()
//{
//    EnableMiniX2CommandButtons(mxcsMX2Exiting);
//    if (MiniX2.STATUS_MNX.HV_EN) {
//        SetHvAndI(false);
//    }
//	TimerControl(tmrMonitor_EventId,false);
//	OnOK();
//}
//
//void CvcDP5Dlg::OnBnClickedCancel()
//{
//	OnCancel();
//}
//
////EnableToolTips();
////ON_NOTIFY_EX_RANGE(TTN_NEEDTEXT, 0, 0xFFFF, OnToolTipNotify)
////afx_msg BOOL OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult);
//BOOL CvcDP5Dlg::OnToolTipNotify(UINT id, NMHDR *pNMHDR, LRESULT *pResult)
//{
//    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pNMHDR;
//    UINT_PTR CtrlHandle = pNMHDR->idFrom;
//    if (pTTT->uFlags & TTF_IDISHWND) {
//        UINT_PTR nID = ::GetDlgCtrlID(HWND(CtrlHandle));
//		CWnd *p = CWnd::FromHandle(pNMHDR->hwndFrom);
//		p->SendMessage(TTM_SETMAXTIPWIDTH, 0, 300);
//        switch(nID) {
//        case IDC_BUTTON_SELECT_COMM:
//            pTTT->lpszText = _T("Open Connect Dialog \r\nto select Amptek DPP device.");
//            break;
//        case IDC_RADIO_PLOT_TYPE_LINE:
//            pTTT->lpszText = _T("Line Plot Type");
//            break;
//        default:
//            //pTTT->lpszText = _T("Default Tooltip");
//            break;
//        }
//        return TRUE;
//	} else {
//		AfxMessageBox("!");
//    }
//    return FALSE;
//}
//

////CTime theTime = CTime::GetCurrentTime();
////string strTime = theTime.Format( "%m/%d/%Y %H:%M:%S" );
////StartTime = strTime;
//
////#include <time.h>
//string CvcDP5Dlg::CreateDTS(string strFormat)
//{
//	//string strFormat("_%Y%m%d_%H%M%S");		// for append filename dts
//	//string strFormat("%m/%d/%Y %H:%M:%S");   // for spectrum dts
//	char buffer[256];
//	time_t rawtime;
//	struct tm * timeinfo;
//	string str("");
//	stringex strfn;
//
//	time (&rawtime);
//	timeinfo = localtime (&rawtime);
//	strftime(buffer, sizeof(buffer), strFormat.c_str(), timeinfo);
//	str = strfn.Format("%s",buffer);
//	return str;
//}

//=======================================================================================================
//=======================================================================================================
// ==== MiniX2 Development ==============================================================================
//=======================================================================================================
//=======================================================================================================

//=======================================================================================================
// ==== Mini-X2 Receive Packet Processing Routines ======================================================
//=======================================================================================================

void CMiniX2Dlg::ProcessStatusExMX2(Packet_In PIN, DppStateType DppState)
{
    long idxStatus;
	string strMX2Status("");	// display MNX_Status Block content
	string strStatus("");		// display MX2 controller status
	stringex strfn;				// std string extended functions
	string strSerNum("");		// display controller serial number

	for(idxStatus=0;idxStatus<64;idxStatus++) {
        MiniX2.STATUS_MNX.RAW[idxStatus] = PIN.DATA[idxStatus];
	}
	MiniX2.Process_MNX_Status(&MiniX2.STATUS_MNX);
	strMX2Status = MiniX2.MiniX2_StatusToString(MiniX2.STATUS_MNX);
	//GetDlgItem(IDC_STATIC_SPECTRUM_STATUS)->SetWindowText(strMX2Status.c_str());
	
	if (!bMiniX2ControllerRunning) {
		strOnTime="";
		strRunMonthlyWU="";
		bSetAdvancedIndicators = false;
		bMiniX2ControllerRunning = true;
		//EnableMiniX2CommandButtons(mxcsMX2Started);
        MX2UpdateTimestampAndWarmupInd();
		//bStartMonitor = true;
		//TimerControl(tmrMonitor_EventId,true);

	}
    //ShowStatus("Received Status");
//////    clsStatusTT.ToolText(picStatusInfo) = strStatusTT			// future status ToolTip

//////    //display advanced warmup indicators if running warmup sequence
//////    if (STATUS_MNX.WARMUP_STEP > 127) {    ' MSB is set if warmup is running
//////        lblWarmupStep.Visible = true
//////        strWarmupStep = " Warmup Step" & vbNewLine
//////        strWarmupStep = strWarmupStep & " Step: " & Format(STATUS_MNX.WARMUP_STEP And 15, "#0") & vbNewLine
//////        strWarmupStep = strWarmupStep & " Step Time: " & Format(STATUS_MNX.WARMUP_TIME, "##0s") & vbNewLine
//////        lblWarmupStep = strWarmupStep
//////    } else {
//////        lblWarmupStep.Visible = false
//////        lblWarmupStep = ""
//////    }

    if (bAdvancedStatus) {
        bAdvancedStatus = false;
		MiniX2.strMX2AdvancedDisplay = strMX2Status;
    }
   if (MiniX2.bHaveTubeType) {
        strStatus = "Mini-X2 Status Received\r\n";
    } else {
        SendCommand(XMTPT_SEND_TUBE_ILOCK_TABLE_MX2);
        strStatus = "Mini-X2 Status Received Reading Tube Type\r\n";
        strSerNum = "Mini-X2 Serial Number " + strfn.Format("%04d", MiniX2.STATUS_MNX.SN);
		MiniX2.strSerNumDisplay=strSerNum;
		MiniX2.bRequestSerNumDisplay=true;
		//GetDlgItem(IDC_SERIAL_NUMBER)->SetWindowText(strSerNum.c_str());     // display mx2 status
	}
	//GetDlgItem(IDC_STATUS_EDIT)->SetWindowText(strStatus.c_str());     // display mx2 status
	//ShowMX2Status(strStatus);
}

void CMiniX2Dlg::ProcessCfgReadMX2Ex(Packet_In PIN, DppStateType DppState)
{
    string strRawCfgIn;
	string strCh;
	stringex strfn;	// = strfn.
        
    strRawCfgIn = "";
    MiniX2.strMX2CfgIn = "";
    MiniX2.bMX2CfgReady = false;
    // =============================
    // === Create Raw Configuration Buffer From Hardware ===
    //For idxCfg = 0 To PIN.LEN - 1
	for (int idxCfg=0;idxCfg<PIN.LEN;idxCfg++) {
		strCh = strfn.Format("%c",PIN.DATA[idxCfg]);
		strRawCfgIn += strCh;
    }
    // =============================

    if (strRawCfgIn.length() > 0) {
        MiniX2.strMX2CfgIn = strRawCfgIn;
        MiniX2.bMX2CfgReady = true;
    }
}

void CMiniX2Dlg::ProcessTubeInterlockTableMX2Ex(Packet_In PIN, DppStateType DppState)
{
    string strTubeInterlockTable("");
	bool bTubeSet=false;
	string strStatus("");
	string strTubeType("");
    strTubeInterlockTable = MiniX2.Process_MNX_Tube_Table(PIN, &MiniX2.TubeInterlockTable);
    if (bAdvancedInterlock) {
        bAdvancedInterlock = false;
        MiniX2.strMX2AdvancedDisplay = strTubeInterlockTable;
    }
    MiniX2.strMX2AdvancedDisplay = MiniX2.Process_MNX_Tube_Table(PIN, &MiniX2.TubeInterlockTable);
	// The Tube and Interlock Table is needed to setup the graphics and parameter constraints

    if (MiniX2.bHaveTubeType) {
        strStatus = "Tube and Interlock Table Received\r\n";
    } else {
		MiniX2.bRequestTubeType = true;
		//////////bTubeSet = SetMX2TubeType(MiniX2.TubeInterlockTable);
  //////////      if (bTubeSet) {
  //////////          strStatus = "Tube and Interlock Table Received, Tube Type: "; 
  //////////          strTubeType = MiniX2.MX2TubeTypeEnumToString(MiniX2.mx2ttTubeType);
		//////////	strStatus += strTubeType;
  //////////          strStatus += "\r\n";
  //////////          sngInterlockMinI = MiniX2.TubeInterlockTable.Interlock_min_I_mA;
  //////////          sngInterlockMaxI = MiniX2.TubeInterlockTable.Interlock_max_I_mA;
  //////////      } else {
  //////////          strStatus = "Tube and Interlock Table Received cannot identify tube type\r\n";
  //////////      }
    }
	//GetDlgItem(IDC_STATUS_EDIT)->SetWindowText(strStatus.c_str());     // display mx2 status
}

void CMiniX2Dlg::ProcessWarmupTableMX2Ex(Packet_In PIN, DppStateType DppState)
{
    MiniX2.strMX2AdvancedDisplay = MiniX2.Process_MNX_Warmup_Table(PIN, &MiniX2.WarmUpTable, MiniX2.WarmUpTableTypeMX2);
    //pnlStatus = "Warmup Table Received" + vbCrLf
}

void CMiniX2Dlg::ProcessTimestampRecordMX2Ex(Packet_In PIN, DppStateType DppState)
{
    string strTimeStamp;
    string strTitle;
    long lInterval=0;
    string strInterval;
    stringex strfn;
	string strStatus("");
	time_t one_second = 1;
	time_t one_minute = 60;
	time_t one_hour = 3600;
	time_t one_day = 86400;
    time_t thirty_days = one_day * 30;
	time_t ttNow;
	struct tm tmNow;

	ttNow = time(NULL);						// Get World Time
	tmNow = *localtime(&ttNow);		// init with current local time
	ttNow = mktime(&tmNow);

	//char buffer[256];
	//string strFormat("%m/%d/%Y %I:%M:%S %p"); 
	//strftime(buffer, sizeof(buffer), strFormat.c_str(), &tmNow);		// current time string

	time_t ttTimeStamp;					// test time in seconds
    strTimeStamp = MiniX2.Process_MNX_Timestamp(PIN, &ttTimeStamp);
	
    if (bAdvancedTimeStamp) {        //Display in Advanced
		bAdvancedTimeStamp = false;
        strTitle = "======= Timestamp Record =========\r\n";
        MiniX2.strMX2AdvancedDisplay = strTitle + "Last ON time: " + strTimeStamp + "\r\n";
    } else {                                // Read the last on time
        strOnTime = "Last ON time: " + strTimeStamp;
        if ((ttTimeStamp+thirty_days) < ttNow) {
            strRunMonthlyWU = "Recommend monthly warmup (more than 30 days since last on)";
        } else if ((ttTimeStamp+one_day) < ttNow) {
            strRunMonthlyWU = "Recommend daily warmup (more than 24 hours since last on)";
        } else {
			lInterval = (long)floor(((double)(ttNow - ttTimeStamp) / (double)one_hour));
            //strStatus = "Hours: " + strfn.Format("%d", lInterval);
            if (lInterval < 1) {     //zero hours
				lInterval = (long)floor(((double)(ttNow - ttTimeStamp) / (double)one_minute));
                //strStatus = "Minutes: " + strfn.Format("%d", lInterval);
                if (lInterval < 1) {
					lInterval = (long)floor(((double)(ttNow - ttTimeStamp) / (double)one_second));
                    strInterval = "(" + strfn.Format("%d", lInterval) + "secs.)";
                    //strStatus = "Seconds: " + strfn.Format("%d", lInterval)
                } else {
                    strInterval = "(" + strfn.Format("%d", lInterval) + "mins.)";
                }
            } else {
                strInterval = "(" + strfn.Format("%d", lInterval) + "hrs.)";
            }
            strRunMonthlyWU = "Tube ready to run. " + strInterval;
        }
    }
    strStatus = "Timestamp Record Received\r\n";
}

void CMiniX2Dlg::ProcessFaultRecordMX2Ex(Packet_In PIN, DppStateType DppState)
{
    MiniX2.strMX2AdvancedDisplay = MiniX2.Process_MNX_Fault_Record(PIN);
	//ShowMX2Status("FaultRecord Received\r\n");
}


//'================================================================================
//'===== Send MX2 ASCII Text Commands =============================================
//'================================================================================

void CMiniX2Dlg::SendCommandDataMX2(TRANSMIT_PACKET_TYPE XmtCmd, string strDataIn)
{
	unsigned char DataOut[514];
	int MaxDataLen=514;
	int idxCh;
	int DataLen;
	DataLen = (int)strDataIn.length();
	if (DataLen > 0) {
		for(idxCh=0;idxCh<MaxDataLen;idxCh++) {
			if (idxCh < DataLen) {
				DataOut[idxCh] = (unsigned char)strDataIn[idxCh];
			} else {
				DataOut[idxCh] = 0;
			}
		}
		SendCommandData(XmtCmd, DataOut);
	}
}

//Recommend FaultOverride OFF
void CMiniX2Dlg::SendMX2_FaultOverride(bool bFaultOverride)
{
    if (bFaultOverride) {
        SendCommandDataMX2(XMTPT_TEXT_CONFIGURATION_MX2, "FAOR=ON;");
    } else {
        SendCommandDataMX2(XMTPT_TEXT_CONFIGURATION_MX2, "FAOR=OFF;");
    }
}

//Recommend LimitOverride OFF
void CMiniX2Dlg::SendMX2_LimitOverride(bool bLimitOverride)
{
    if (bLimitOverride) {
        SendCommandDataMX2(XMTPT_TEXT_CONFIGURATION_MX2, "LIOR=ON;");
    } else {
        SendCommandDataMX2(XMTPT_TEXT_CONFIGURATION_MX2, "LIOR=OFF;");
    }
}

//Recommend Volume ON
void CMiniX2Dlg::SendMX2_Volume(bool bVolumeOn)
{
    if (bVolumeOn) {
        SendCommandDataMX2(XMTPT_TEXT_CONFIGURATION_MX2, "VOLU=ON;");
    } else {
        SendCommandDataMX2(XMTPT_TEXT_CONFIGURATION_MX2, "VOLU=OFF;");
    }
}

void CMiniX2Dlg::SendMX2_HVandI(string strHV, string strI)
{
	double dblHV;
	double dblI;
    string strCmd;
	stringex strfn;

	dblHV = atof(strHV.c_str());
	dblI = atof(strI.c_str());
    //strCmd = "HVSE=" + Trim(Val(strHV)) + ";CUSE=" + Trim(Val(strI)) + ";";
	strCmd = "HVSE=";
	strCmd += strfn.Format("%0.1f;", dblHV);
	strCmd += "CUSE=";
	strCmd += strfn.Format("%0.1f;", dblI);

    SendCommandDataMX2(XMTPT_TEXT_CONFIGURATION_MX2, strCmd);
}

bool CMiniX2Dlg::ReadbackMX2_HVandI(float *sngHV, float *sngI)
{
    CMSecTimer tmr;
    string strCmd;
//    bool bCfgCmdDone;
    string strHV;
    string strI;
    float sngHV_In;
    float sngI_In;
    bool bReadbackMX2_HVandI;
	float fltTemp;
//    
    bReadbackMX2_HVandI = false;
    strCmd = "HVSE=?;CUSE=?;";
    MiniX2.strMX2CfgIn = "";
    MiniX2.bMX2CfgReady = false;
    SendCommandDataMX2(XMTPT_READ_TEXT_CONFIGURATION_MX2, strCmd);
    tmr.msTimer(100);
    //strStatus = "bMX2CfgReady: " + CStr(bMX2CfgReady)
    if (MiniX2.bMX2CfgReady) {                      //1. get decoded message values
        strHV = GetCmdData("HVSE", MiniX2.strMX2CfgIn);
        strI = GetCmdData("CUSE", MiniX2.strMX2CfgIn);
    }
    sngHV_In = *sngHV;
    sngI_In = *sngI;                             //2. compare to input values
	fltTemp = StringToFloat(strHV);
    if (sngHV_In != fltTemp) {      //3. if different set flag and replace values
        bReadbackMX2_HVandI = true;
        *sngHV = fltTemp;
    }
	fltTemp = StringToFloat(strI);
    if (sngI_In != fltTemp) {
        bReadbackMX2_HVandI = true;
        *sngI = fltTemp;
    }
	return(bReadbackMX2_HVandI);
}


//void CvcDP5Dlg::cmdMX2Advanced_Click(int Index)
void CMiniX2Dlg::cmdMX2Advanced_Click(int Index)
{
	CString strIndex="";
	strIndex.Format("%d",Index);
	//AfxMessageBox("cmdMX2Advanced_Click("+strIndex+")");
	//MiniX2.strMX2AdvancedDisplay = "cmdMX2Advanced_Click("+strIndex+")";
    switch (Index) {
        case 0:
            bAdvancedStatus = true;
            SendCommand(XMTPT_SEND_STATUS);
			break;
        case 1:
            bAdvancedInterlock = true;
            SendCommand(XMTPT_SEND_TUBE_ILOCK_TABLE_MX2);
			break;
        case 2:
            SendCommand(XMTPT_SEND_WARMUP_TABLE_MX2);
			break;
        case 3:
            bAdvancedTimeStamp = true;
            SendCommand(XMTPT_SEND_TIMESTAMP_RECORD_MX2);
			break;
        case 4:
            SendCommand(XMTPT_SEND_FAULT_RECORD_MX2);
			break;
    }
}

void CMiniX2Dlg::cmdWarmUpSequence_Click(int Index)
{
    switch (Index) {
        case 0:      //Daily Warmup
            SendCommand(XMTPT_INITIATE_WARMUP_DAILY_SEQUENCE_MX2);
			break;
        case 1:      //Monthly Warmup
            SendCommand(XMTPT_INITIATE_WARMUP_MONTHLY_SEQUENCE_MX2);
 			break;
       case 2:      //Stop Warmup
            SetHvAndI_Off();
			break;
	}
}

//Turns tube on/off, sets high voltage and current
//defaults to tube off if no bTubeOn is specified
//reads values to send from user HV (txtHv) and uA (txtCurrent)
//also resets user values if out of range
//HV and UA min/max values were read from the tube table
void CMiniX2Dlg::SetHvAndI_Off()
{
    CMSecTimer tmr;
    float sngHV;
    float sngI;
    string strHV;
    string strI;
    bool bValueChanged;
    //string strStatus;
   
	sngHV = 0.0;
	sngI = 0.0;
	strHV = "0.0";
	strI = "0.0";
    SendMX2_HVandI(strHV, strI);
    tmr.msTimer(100);
    bValueChanged = ReadbackMX2_HVandI(&sngHV, &sngI);
  //  if (bValueChanged) {
  //      strHV = FloatToString(sngHV, 1);
		//DisplayDouble(IDC_SETHIGHVOLTAGECONTROLEDIT, sngHV, 1);
  //      strI = FloatToString(sngI, 1);
		//DisplayDouble(IDC_SETCURRENTCONTROLEDIT, sngI, 1);
  //  }
}

void CMiniX2Dlg::MX2UpdateTimestampAndWarmupInd()
{
    CMSecTimer tmr;
    bAdvancedTimeStamp = false;
    SendCommand(XMTPT_SEND_TIMESTAMP_RECORD_MX2);
    tmr.msTimer(100);
    SendCommand(XMTPT_WRITE_TIMESTAMP_MX2); // update the date/time in the Mini-X
}

//Calls Advanced ASCII Commands, toggles state when called
void CMiniX2Dlg::cmdMX2AscCmd_Click(int iIndex)
{
    switch (iIndex) {
        case 0:      //Limit Override
            SendMX2_LimitOverride (!MiniX2.STATUS_MNX.DISABLE_LIMIT_CHECKS);
			break;
        case 1:      //Fault Override
            SendMX2_FaultOverride (!MiniX2.STATUS_MNX.DISABLE_FAULT_CHECKS);
			break;
        case 2:      //Volume
            SendMX2_Volume (!MiniX2.STATUS_MNX.SPEAKER_EN);
			break;
    }
}

void CMiniX2Dlg::DisplayDouble(int DlgItem, double dblValue, int dpDigits=0)
{
    string strValue;
    string strFormat;
	stringex strfn;
    strFormat = strfn.Format("%%.%df",dpDigits);           // get decimal point precision
    strValue = strfn.Format(strFormat.c_str(),dblValue);          // format data
    GetDlgItem(DlgItem)->SetWindowText(strValue.c_str());  // display in control
}

string CMiniX2Dlg::FloatToString(float fltValue, int dpDigits=0)
{
    string strValue;
    string strFormat;
	stringex strfn;
    strFormat = strfn.Format("%%.%df",dpDigits);           // get decimal point precision
    strValue = strfn.Format(strFormat.c_str(),fltValue);          // format data
	return(strValue);
}

float CMiniX2Dlg::StringToFloat(string strValue)
{
    float fltValue;
    fltValue = (float)atof(strValue.c_str());
    return fltValue;
}

//void CvcDP5Dlg::DisplayStatus(byte StatusCode)
//{
//    string strStatus("");
//    //cstrStatus = GetMiniXStatusString(StatusCode);              // get status string
//    GetDlgItem(IDC_STATUS_EDIT)->SetWindowText(strStatus.c_str());     // display status
//}
//
//float CvcDP5Dlg::GetWindowFloat(int nID)
//{
//    string strValue;
//    float fltValue;
//	char chValue[20];
//	stringex strfn;
//    GetDlgItem(nID)->GetWindowText(chValue,20);
//	strValue = strfn.Format("%s",chValue);
//    fltValue = (float)atof(strValue.c_str());
//    return fltValue;
//}
//
//double CvcDP5Dlg::GetWindowDouble(int nID)
//{
//    string strValue;
//    double dblValue;
//	char chValue[20];
//	stringex strfn;
//    GetDlgItem(nID)->GetWindowText(chValue,20);
//	strValue = strfn.Format("%s",chValue);
//    dblValue = atof(strValue.c_str());
//    return dblValue;
//}

void CMiniX2Dlg::SetFlashWindow(DWORD dwWinFunc)
{
	FlashTbInfo.cbSize = sizeof(FlashTbInfo);
	FlashTbInfo.hwnd = this->m_hWnd;
	FlashTbInfo.dwFlags = dwWinFunc;
	FlashTbInfo.dwTimeout = 0;
	FlashTbInfo.uCount = 0;
	::FlashWindowEx(&FlashTbInfo);
}
