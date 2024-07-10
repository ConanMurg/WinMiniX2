#include "StdAfx.h"
#include "SProfile.h"
#include <time.h>
#include <math.h>

CSProfile::CSProfile(void)
{
	// seed random number generator
	srand((unsigned)time(NULL));	//Seed the random-number generator with current time
}

CSProfile::~CSProfile(void)
{
}

CString CSProfile::Get(LPCTSTR profile, LPCTSTR entry, LPCTSTR def)
{
	//return(theApp.GetProfileString(profile, entry, def));
	return("");
}
    
int CSProfile::Get(LPCTSTR profile, LPCTSTR entry, int def)
{
	//return(theApp.GetProfileInt(profile, entry, def));
	return(1);
}
    
BOOL CSProfile::Put(LPCTSTR profile, LPCTSTR entry, LPCTSTR def)
{
	//return(theApp.WriteProfileString(profile, entry, def));
	return(TRUE);
}
    
BOOL CSProfile::Put(LPCTSTR profile, LPCTSTR entry, int def)
{
	//return(theApp.WriteProfileInt(profile, entry, def));
	return(TRUE);
}

void CSProfile::LoadProfile()
{
	// plot type
	//GETP1(regDP5Comm, isScaleLog, FALSE);	//TRUE=log, FALSE=linear, default=FALSE
	//GETP1(regDP5Comm, isPlotLine, FALSE);	//TRUE=line, FALSE=solid, default=FALSE

}

void CSProfile::SaveProfile()
{
	// plot type
	//PUTP1(regDP5Comm, isScaleLog);
	//PUTP1(regDP5Comm, isPlotLine);
}

