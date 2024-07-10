// ConnectPage.cpp : implementation file
//
#include "stdafx.h"
#include "resource.h"
#include "WizardDialog.h"
#include "ConnectPage.h"
#include "MiniX2Dlg.h"
#include ".\connectpage.h"
#include "DppWinUSB.h"
#include "DebugUtils.h" // MINE - ALLOWS PrintToOutputWindow 

//#include "SProfile.h"
#define HIDD_MX2_CONNECT                        0x20191

/////////////////////////////////////////////////////////////////////////////
//  CConnectPage dialog

CConnectPage::CConnectPage(CWnd* pParent /*=NULL*/)
	: CWizardPage(CConnectPage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CHardwarePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	PrintToOutputWindow("CConnectPage - ConnectPage.cpp\n");
}


//IDD_MX2_CONNECT

//IMPLEMENT_DYNAMIC(CConnectPage, CDialog)
//CConnectPage::CConnectPage(CWnd* pParent /*=NULL*/)
//	: CDialog(CConnectPage::IDD, pParent)

//CConnectPage::CConnectPage(CWnd* pParent /*=NULL*/)
//	: CDialog(_T("IDD_MX2_CONNECT"), pParent)
//CConnectPage::CConnectPage(CWnd* pParent /*=NULL*/)
//	: CDialog("IDD_MX2_CONNECT")

//CConnectPage::CConnectPage() : CDialog(_T("IDD_MX2_CONNECT"))
//{
//	//
//}

//CConnectPage::~CConnectPage()
//{
//}

BEGIN_MESSAGE_MAP(CConnectPage, CDialog)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_RADIO_COMMTYPE2, OnBnClickedRadioCommtype2)
	ON_BN_CLICKED(IDC_BUTTON_FIND_USB_DEVICE, OnBnClickedButtonFindUsbDevice)
	ON_BN_CLICKED(IDC_CONNECTOK, OnBnClickedConnectOK)
	//ON_BN_CLICKED(IDOK, OnOK)

	ON_BN_CLICKED(IDC_BUTTON_COUNT_DEVICES, OnBnClickedButtonCountDevices)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_SELECT_DEVICE, OnDeltaposSpinSelectDevice)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE_DEVICE, OnBnClickedButtonCloseDevice)
	//ON_WM_HELPINFO()
	ON_BN_CLICKED(IDCANCEL, OnBnClickedCancel)
END_MESSAGE_MAP()

void CConnectPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSpectrumDlg)
	DDX_Control(pDX, IDC_STATIC_FOUND2, lblDppFound[1]);
	DDX_Control(pDX, IDC_SPIN_SELECT_DEVICE, m_SpinSelectDevice);
	//}}AFX_DATA_MAP
}

BOOL CConnectPage::OnInitDialog()
{
	PrintToOutputWindow("OnInitDialog - ConnectPage.cpp\n");

	CDialog::OnInitDialog();

	bMiniX2Connected = false;
	GetDlgItem(IDC_CONNECTOK)->ShowWindow(SW_HIDE);
	DppWinUSB.NumDevices = 0;
	GetDlgItem(IDC_STATIC_NUM_DEVICES)->SetWindowText("0");
	DppWinUSB.CurrentDevice = 1;
	m_SpinSelectDevice.SetRange(1,1);
	m_SpinSelectDevice.SetPos(1);
	GetDlgItem(IDC_STATIC_NUM_DEVICES2)->SetWindowText("1");

	lblDppFound[USB]
		.SetFontName("Arial")
		.SetFontSize(14)
		.SetTextColor(colorLightSlateGray)
		.SetBkColor(colorLightSteelBlue)
		.SetFontBold(TRUE);

	if (DppWinUSB.devInfo.deviceConnected) {
		lblDppFound[USB].ShowWindow(SW_SHOW);
	} else {
		lblDppFound[USB].ShowWindow(SW_HIDE);
	}
	CheckRadioButton(IDC_RADIO_COMMTYPE2, IDC_RADIO_COMMTYPE2, IDC_RADIO_COMMTYPE2);
	EnableCommFunctions(USB);	// initialize interface selection
	OnBnClickedButtonCountDevices();
	OnBnClickedButtonFindUsbDevice();
	//CenterWindow();
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CConnectPage::OnOK()
{
	return;		// there is no OnOk Button, disable return key exit
	//CWizardPage::OnOK();
}

BOOL CConnectPage::DestroyWindow()
{
	return CDialog::DestroyWindow();
}

void CConnectPage::OnSetActive()
{
	m_pParent->SetTitle("Select Mini-X2 Communications Port");
}

void CConnectPage::OnBnClickedRadioCommtype2()
{
	EnableCommFunctions(USB);
	OnBnClickedButtonCountDevices();
	OnBnClickedButtonFindUsbDevice();
}

void CConnectPage::OnBnClickedButtonFindUsbDevice()
{
	PrintToOutputWindow("\n\nOnBnClickedButtonFindUsbDevice - ConnectPage.cpp\n");

	TRANSMIT_PACKET_TYPE XmtCmd;
    BOOL HaveBuffer;
    BOOL SentPkt;

	string strUSBStatus;
	DPP_USB_STATUS USBStatus;	//0=do nothing, 1=new connect, 2=cannot connect
	CString cstrDeviceType="";

	long pktLen;
	BYTE buffer[4096] = {0};
	int iPos=0;

	GetDlgItem(IDC_CONNECTOK)->ShowWindow(SW_HIDE);
	OnBnClickedButtonCloseDevice();

	USBStatus = DppWinUSB.FindUSBDevice(DppWinUSB.CurrentDevice - 1);
	if (USBStatus != dusbStatNoAction) {
		strUSBStatus = DppWinUSB.USBStatusString(USBStatus);
		GetDlgItem(IDC_EDIT_STATUS2)->SetWindowText(strUSBStatus.c_str());
		if (USBStatus == dusbStatNewConnect) {
			// new usb connection
		}
	}
	if (DppWinUSB.devInfo.deviceConnected) {
		lblDppFound[USB].ShowWindow(SW_SHOW);
		DppWinUSB.isDppFound = TRUE;
		XmtCmd = XMTPT_SEND_NETFINDER_PACKET;
		HaveBuffer = SndCmd.DP5_CMD(DP5Proto.BufferOUT, XmtCmd);
		if (HaveBuffer) {
			SentPkt = DppWinUSB.SendPacketUSB(DP5Proto.BufferOUT, DppWinUSB.devInfo, DP5Proto.PacketIn);
		}
		if (SentPkt) {
			FindDppUSB.InitAllEntries();
			pktLen = (long)((DP5Proto.PacketIn[4] * 256) + DP5Proto.PacketIn[5] + 8);   // temporarily save the length field
			for (int i=0; i< pktLen;i++) {
				buffer[i] = DP5Proto.PacketIn[i+6];
			}
			FindDppUSB.AddEntry(&FindDppUSB.DppEntries[0], buffer, 0);
			CString cstrStatus;
			CString cstrDeviceInfo;
			cstrDeviceInfo.Format("USB Device %d of %d", DppWinUSB.CurrentDevice, DppWinUSB.NumDevices);
			cstrStatus = FindDppUSB.EntryToStrUSB(&FindDppUSB.DppEntries[0], cstrDeviceInfo);
			GetDlgItem(IDC_EDIT_STATUS2)->SetWindowText(cstrStatus);
			iPos = cstrStatus.Find(" - S/N");
			cstrDeviceType = cstrStatus.Left(iPos);
			if (cstrDeviceType.CompareNoCase("Amptek Mini-X2")==0) {
				GetDlgItem(IDC_STATIC_CONNECTION_STATUS)->SetWindowText("Mini-X2");
				GetDlgItem(IDC_CONNECTOK)->ShowWindow(SW_SHOW);
				bMiniX2Connected = true;
			} else {
				GetDlgItem(IDC_CONNECTOK)->ShowWindow(SW_HIDE);
				GetDlgItem(IDC_STATIC_CONNECTION_STATUS)->SetWindowText(cstrDeviceType);
				bMiniX2Connected = false;
			}
		}
	} else {
		GetDlgItem(IDC_CONNECTOK)->ShowWindow(SW_HIDE);
		lblDppFound[USB].ShowWindow(SW_HIDE);
		DppWinUSB.isDppFound = FALSE;
		GetDlgItem(IDC_STATIC_CONNECTION_STATUS)->SetWindowText("Device not connected");
		bMiniX2Connected = false;
	}
}

void CConnectPage::EnableCommFunctions(int ControlGroup)
{
	PrintToOutputWindow("EnableCommFunctions - ConnectPage.cpp\n");
	BOOL isDUSBEnabled;
	int ShowDUSBEnabled;
	isDUSBEnabled = FALSE;
	ShowDUSBEnabled = SW_HIDE;

	switch (ControlGroup) {
		case USB:
			isDUSBEnabled = TRUE;
			ShowDUSBEnabled = isDUSBEnabled * SW_SHOW;
			break;
	}

	// WinUsb
	GetDlgItem(IDC_BUTTON_FIND_USB_DEVICE)->EnableWindow(isDUSBEnabled);
	GetDlgItem(IDC_EDIT_STATUS2)->EnableWindow(isDUSBEnabled);

	GetDlgItem(IDC_BUTTON_COUNT_DEVICES)->EnableWindow(isDUSBEnabled);
	GetDlgItem(IDC_STATIC_NUM_DEVICES)->EnableWindow(isDUSBEnabled);
	GetDlgItem(IDC_STATIC_SELECT_USB_DEVICE_LABEL)->EnableWindow(isDUSBEnabled);
	GetDlgItem(IDC_SPIN_SELECT_DEVICE)->EnableWindow(isDUSBEnabled);
	GetDlgItem(IDC_STATIC_NUM_DEVICES2)->EnableWindow(isDUSBEnabled);
	GetDlgItem(IDC_BUTTON_CLOSE_DEVICE)->EnableWindow(isDUSBEnabled);

	GetDlgItem(IDC_EDIT_STATUS2)->ShowWindow(ShowDUSBEnabled);

	if (ControlGroup != USB) {
		lblDppFound[USB].SetNullColor();
	} else {
		lblDppFound[USB].SetLastColor();
	}
	GetDlgItem(IDC_RADIO_COMMTYPE2)->EnableWindow(TRUE);	// WinUsb

}

//void CConnectPage::OnBnClickedOk()
//{
//	// TODO: Add your control notification handler code here
//	OnOK();
//}

void CConnectPage::OnBnClickedButtonCountDevices()
{
	PrintToOutputWindow("\n\nOnBnClickedButtonCountDevices - ConnectPage.cpp\n");

	CDppWinUSB DppWinUSB1;
	CString cstrNumDevices;
	DppWinUSB.NumDevices = DppWinUSB1.CountDP5WinusbDevices();
	if (DppWinUSB.CurrentDevice > DppWinUSB.NumDevices) {
		DppWinUSB.CurrentDevice = 1;
		m_SpinSelectDevice.SetPos(1);
		GetDlgItem(IDC_STATIC_NUM_DEVICES)->SetWindowText("1");
	}
	if (DppWinUSB.NumDevices > 1) {
		m_SpinSelectDevice.SetRange(1,DppWinUSB.NumDevices);
		m_SpinSelectDevice.EnableWindow(TRUE);
	} else {
		DppWinUSB.CurrentDevice = 1;
		m_SpinSelectDevice.SetPos(1);
		m_SpinSelectDevice.SetRange(1,1);
		GetDlgItem(IDC_STATIC_NUM_DEVICES)->SetWindowText("1");
		m_SpinSelectDevice.EnableWindow(FALSE);
	}
	cstrNumDevices.Format("%d",DppWinUSB.NumDevices);
	GetDlgItem(IDC_STATIC_NUM_DEVICES)->SetWindowText(cstrNumDevices);
	UpdateData(FALSE);	// DDEX from var to cntrl, ie sends var value to control
}

void CConnectPage::OnDeltaposSpinSelectDevice(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	CString cstrPos;
	int iMax = 1;		
	int iMin = 1;
	int iVal = 1;		// old value
	int iPos = 1;		// new value (if updated)
	int iDelta = 0;		// direction of change
	int iNewPos = 1;	// final value
	m_SpinSelectDevice.GetRange(iMin, iMax);
	if ((iMin != 1) || (iMax < 1)) { return; }	// max/min value errors
	if (iMax == 1) {							// 0 or 1 devices
		iNewPos = 1;	
	} else {									// max must be greater than 1
		GetDlgItem(IDC_STATIC_NUM_DEVICES2)->GetWindowText(cstrPos);
		iVal = atoi(cstrPos);
		iPos = pNMUpDown->iPos;
		iDelta = pNMUpDown->iDelta;
		if (iVal != iPos) {						// value was updated 
			iNewPos = iPos;
		} else {								// control just enabled, has delta but pos not updated
			iNewPos = iPos + iDelta;			// update value
			if (iNewPos > iMax) {				// do wrap if needed
				iNewPos = iMin;
			} else if (iNewPos < iMin) {
				iNewPos = iMax;
			}
		}
	}
	//cstrPos.Format("iVal %d iPos %d iDelta %d iNewPos %d", iVal, iPos, iDelta, iNewPos);
	cstrPos.Format("%d", iNewPos);
	//AfxMessageBox(cstrPos);
	DppWinUSB.CurrentDevice = iNewPos;
	OnBnClickedButtonFindUsbDevice();
	GetDlgItem(IDC_STATIC_NUM_DEVICES2)->SetWindowText(cstrPos);
	*pResult = 0;
}

void CConnectPage::OnBnClickedButtonCloseDevice()
{
	PrintToOutputWindow("\n\nOnBnClickedButtonCloseDevice- ConnectPage.cpp\n");
	DppWinUSB.devInfo.deviceConnected = FALSE;
	DppWinUSB.CloseWinUsbDevice();
	WinusbNotify.RemoveDeviceNotification();
	GetDlgItem(IDC_EDIT_STATUS2)->SetWindowText("Device Connection Closed");
	lblDppFound[USB].ShowWindow(SW_HIDE);
	DppWinUSB.isDppFound = FALSE;
}

//
//void CConnectPage::OnOK()
//{
//	//DISABLE DEFAULT 
//}
//
//BOOL CConnectPage::OnHelpInfo(HELPINFO* pHelpInfo) 
//{
//	CString cstrHelp;
//	if (pHelpInfo->iContextType == HELPINFO_WINDOW) {
//		if (pHelpInfo->dwContextId == 0) {
//			::HtmlHelp(this->GetSafeHwnd(), theApp.m_pszHelpFilePath, HH_HELP_CONTEXT, HIDD_MX2_CONNECT);
//		} else {
//			//
//		}
//	}
//	return true;
//}
//

void CConnectPage::OnBnClickedConnectOK()
{
	PrintToOutputWindow("\n\nOnBnClickedConnectOK - ConnectPage.cpp\n");
	OnBnClickedButtonFindUsbDevice();
	if (bMiniX2Connected) {
		CWizardDialog* pSheet = (CWizardDialog*)GetParent();
		int iPage = pSheet->GetPageIndex(this);
		pSheet->SetActivePage(iPage + 1);
	} else {		// refresh display to show errors
		OnBnClickedButtonCountDevices();
	}
}

void CConnectPage::OnBnClickedCancel()
{
	PrintToOutputWindow("\n\nOnBnClickedCancel - ConnectPage.cpp\n");
	m_pParent->OnWizardFinishEx();
}

