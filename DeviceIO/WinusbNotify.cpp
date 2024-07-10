#include "StdAfx.h"
#include "WinusbNotify.h"

CWinusbNotify::CWinusbNotify(void)
{
	m_hWndOwner = NULL;
	DeviceNotificationHandle = NULL;
}

CWinusbNotify::~CWinusbNotify(void)
{
}

void CWinusbNotify::SetNotificationWindow(HWND m_hWnd)
{
	m_hWndOwner = m_hWnd;
}

void CWinusbNotify::InitDeviceInfo()
{
	DeviceNotificationHandle = NULL;
}

void CWinusbNotify::RegDeviceNotification()
{
	DEV_BROADCAST_DEVICEINTERFACE DevBroadcastDeviceInterface;
	DevBroadcastDeviceInterface.dbcc_size = sizeof(DevBroadcastDeviceInterface);
	DevBroadcastDeviceInterface.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;
	DevBroadcastDeviceInterface.dbcc_classguid = GUID_DP5_INTERFACE_CLASS;
	DeviceNotificationHandle = RegisterDeviceNotification(m_hWndOwner, 
									&DevBroadcastDeviceInterface, 
									DEVICE_NOTIFY_WINDOW_HANDLE);
}

BOOL CWinusbNotify::GetDevNotifyInfo(DWORD_PTR dwData, char devicePath[])
{
	CString DeviceNameString;
	//CString strDevicepath("%s", devicePath);
	PDEV_BROADCAST_HDR lDBH = (PDEV_BROADCAST_HDR)dwData;
	if (lDBH->dbch_devicetype == DBT_DEVTYP_DEVICEINTERFACE)
	{
		PDEV_BROADCAST_DEVICEINTERFACE lDBDI = (PDEV_BROADCAST_DEVICEINTERFACE)dwData;
		DeviceNameString = lDBDI->dbcc_name;
		//AfxMessageBox(DeviceNameString);
		if 	((DeviceNameString.CompareNoCase(devicePath)) == 0) {
			//AfxMessageBox("The names match.");
			return true;
		} else {
			//AfxMessageBox("It’s a different device.");
			return false;
		}
	}
	return false;
}

void CWinusbNotify::RemoveDeviceNotification()
{
	if (DeviceNotificationHandle != NULL) {
		UnregisterDeviceNotification(DeviceNotificationHandle);
	}
}

BOOL CWinusbNotify::UnregDeviceNotification(HDEVNOTIFY Handle)
{
	return UnregisterDeviceNotification(Handle);
}

