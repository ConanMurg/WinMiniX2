// vcMiniX2.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "vcMiniX2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


void PrintToOutputWindow(const std::string& message) {
	OutputDebugStringA(message.c_str());
}


/////////////////////////////////////////////////////////////////////////////
// CWizApp

BEGIN_MESSAGE_MAP(CWizApp, CWinApp)
	//{{AFX_MSG_MAP(CWizApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWizApp construction

CWizApp::CWizApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CWizApp object

CWizApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CWizApp initialization

BOOL CWizApp::InitInstance()
{
	PrintToOutputWindow("InitInstance - vcMiniX2.cpp\n");

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.
	SetRegistryKey(_T("Amptek"));

	CMiniX2Dlg Dlg(NULL);
	m_pMainWnd = &Dlg;

	//PrintToOutputWindow("InitInstance2 - vcMiniX2.cpp\n");
	
	//CHardwarePage HardwarePage;
	//CSystemPage SystemPage;
	//CPerfDataPage PerfDataPage;
	//
	//Dlg.AddPage(&HardwarePage, CHardwarePage::IDD);
	//Dlg.AddPage(&SystemPage, CSystemPage::IDD);
	//Dlg.AddPage(&PerfDataPage, CPerfDataPage::IDD);

	CConnectPage ConnectPage;
	CMiniXPage MiniXPage;
	CAdvancedPage AdvancedPage;
	
	PrintToOutputWindow("Test - vcMiniX2.cpp\n");
	
	Dlg.AddPage(&ConnectPage, CConnectPage::IDD);
	Dlg.AddPage(&MiniXPage, CMiniXPage::IDD);
	Dlg.AddPage(&AdvancedPage, CAdvancedPage::IDD);

	PrintToOutputWindow("Test2 - vcMiniX2.cpp\n");


	if (Dlg.DoModal() == ID_WIZFINISH)
	{
		AfxMessageBox("Finished Wizard");
	}
	else
	{
		AfxMessageBox("Cancelled Wizard");
	}

	




	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
