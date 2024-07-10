#pragma once
#include "stdafx.h"
//#include "vcDP5.h"
#include "string.h"

// Get/Save a double format value
#define GETPD(p1, i1, data) str = Get(p1, #i1, data); i1 = atof(str)
#define PUTPD(p1, i1) str.Format("%g", i1); Put(p1, #i1, str)

// Get/Save a parameter value
#define GETP1(p1, i1, data) i1 = Get(p1, #i1, data)
#define PUTP1(p1, i1) Put(p1, #i1, i1)

const CString regDP5Comm = "DP5_Communication";
const CString regDP5Device = "DP5_Device";
const CString regDP5Config = "DP5_Configuration";

class CSProfile
{
public:
	CSProfile(void);
	~CSProfile(void);

	// Save/Load Registry Settings Support functions
    CString Get(LPCTSTR profile, LPCTSTR entry, LPCTSTR def);
    int Get(LPCTSTR profile, LPCTSTR entry, int def);
    BOOL Put(LPCTSTR profile, LPCTSTR entry, LPCTSTR def);
    BOOL Put(LPCTSTR profile, LPCTSTR entry, int def);
	void LoadProfile();
	void SaveProfile();
	
	// save plot type
	BOOL isScaleLog;	//TRUE=log, FALSE=linear, default=FALSE
	BOOL isPlotLine;	//TRUE=line, FALSE=solid, default=FALSE

	CString profile;
	int device_type;	// DPP Device Type
	int data_refresh;	// usb data refresh rate in milli seconds
	//string strDevice; // Holds the live device type name for display // DPP5 20071010_1245
};

extern CSProfile Profile;

















