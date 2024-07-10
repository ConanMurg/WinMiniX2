#pragma once
#include "stdafx.h"
#include "X11Color.h"

/////////////////////////////////////////////////////////////////////////////
// NetFinder Data

typedef struct NETDISPLAY_ENTRY {
	unsigned char alert_level;
	
	unsigned int event1_days;
	unsigned char event1_hours;
	unsigned char event1_minutes;
	
	unsigned int event2_days;
	unsigned char event2_hours;
	unsigned char event2_minutes;

	unsigned char event1_seconds;
	unsigned char event2_seconds;

	unsigned char mac[6];
	unsigned char ip[4];
	unsigned short port; // Get port from UDP header
	unsigned char subnet[4];
	unsigned char gateway[4];
	
	CString str_type;
	CString str_description;
	CString str_ev1;
	CString str_ev2;

	CString str_display;
	COLORREF ccConnectRGB;

	BOOL HasData;

	unsigned long SockAddr;

} NETDISPLAY_ENTRY;

const int NO_ENTRIES = -1;
const int MAX_ENTRIES = 10;

class CNetFinder
{
public:
	CNetFinder(void);
	~CNetFinder(void);

	int m_rand;
	NETDISPLAY_ENTRY DppEntries[MAX_ENTRIES];	// 20140430 changed hardcode to const var
	int LastEntry;

	unsigned long DppSockAddr;					// socket address of selected DPP
	int DppEntryIdx;					// entry index of selected DPP
	BOOL DppFound;						// selected DPP found

	CString EntryToStr(NETDISPLAY_ENTRY *pEntry);
	CString EntryToStrRS232(NETDISPLAY_ENTRY *pEntry, CString strPort);
	CString EntryToStrUSB(NETDISPLAY_ENTRY *pEntry, CString strPort);
	COLORREF AlertEntryToCOLORREF(NETDISPLAY_ENTRY *pEntry);
	COLORREF AlertByteToCOLORREF(unsigned char alert_level);
	
	int FindEntry(unsigned long SockAddr);

	CString SockAddr_DlgToStr(CDialog *dlg, int id1, int id2, int id3, int id4);
	unsigned int SockPort_DlgToUInt(CDialog *dlg, int id1);

	in_addr SockAddr_ByteToInAddr(unsigned char *ip);
	unsigned long SockAddr_ByteToULong(unsigned char *ip);
	CString SockAddr_ByteToStr(unsigned char *ip);

	in_addr SockAddr_StrToInAddr(CString strSockAddr);	// Bytes in_addr.S_un.S_un_b.s_b* 
	unsigned long SockAddr_StrToULong(CString strSockAddr);

	in_addr SockAddr_ULongToInAddr(unsigned long SockAddr);
	CString SockAddr_ULongToStr(unsigned long SockAddr);

	CString SockAddr_InAddrToStr(in_addr inSockAddr);

	void InitEntry(NETDISPLAY_ENTRY *pEntry);
	void InitAllEntries();
	void AddEntry(NETDISPLAY_ENTRY *pEntry, unsigned char *buffer, unsigned int destPort);
	
};

