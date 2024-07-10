/** CWinusbNotify CWinusbNotify */
#pragma once
#include "DppWinUSB.h"

class CWinusbNotify
{
public:
	CWinusbNotify(void);
	~CWinusbNotify(void);
	HDEVNOTIFY DeviceNotificationHandle;
	void InitDeviceInfo();
	HWND m_hWndOwner;
	void SetNotificationWindow(HWND m_hWnd);
	void RegDeviceNotification();
	BOOL GetDevNotifyInfo(DWORD_PTR dwData, char devicePath[]);
	void RemoveDeviceNotification();
	BOOL UnregDeviceNotification(HDEVNOTIFY Handle);

};
