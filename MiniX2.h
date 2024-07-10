#pragma once
#ifndef MiniX2_H
#define MiniX2_H

#include <stdio.h>
#include <stdarg.h>
#include <cstdarg>
#include <vector>
#include <string>
using namespace std;
#include "DppUtilities.h"
#include "DP5Protocol.h"

// ==============================================
// MiniX_Command_Button_States
// >Multiple controls can be enabled in one state
// ==============================================
// Control			mxmEnabledCmds Bit
// indStartMiniX		0x01
// indHVOn				0x02
// indHVOff				0x04
// indSetHVandCurrent	0x08
// indExit				0x10
// indMonitor			0x20
// =========================================
const unsigned char mxcsMX2Ready = 0x11;
const unsigned char mxcsMX2Started = 0x36;
const unsigned char mxcsMX2Waiting = 0x30;
const unsigned char mxcsMX2HVOn = 0x3C;
const unsigned char mxcsMX2HVOff = 0x36;
const unsigned char mxcsMX2Exiting = 0x20;

const long DailyWarmupIndexStart = 0;
const long MonthlyWarmupIndexStart = 6;

//==== Warmup Table ==================================
enum MX2WarmupTableType {
	mx2tbltyp50kV4W,
	mx2tbltyp50kV10W,
	mx2tbltyp70kV10W,
	mx2tbltypCustom
};

enum MX2TubeTypeEnum {
	mx2tubetype50kV4W,
	mx2tubetype50kV10W,
	mx2tubetype70kV10W,
	mx2tubetypeNONE
};

// commands that can be sent with SendMiniXCommand
// ALSO
// command enable masks for mxmEnabledCmds
enum MiniX_Commands {
	mxcDisabled = 0,             // null command
	mxcStartMiniX = 1,           // start minix controller
	mxcHVOn = 2,                 // turn high voltage on
	mxcHVOff = 4,                // turn high voltage off
	mxcSetHVandCurrent = 8,      // set high voltage and current
	mxcExit = 16,                // exit controller
	mxcMonitor = 32              // monitor on
};

//minix controller status codes
enum MiniX_Status {
	mxstNoStatus,                    // no status available
	mxstDriversNotLoaded,            // drivers were not found, install drivers
	mxstMiniXApplicationReady,       // application is ready to connect to minix
	mxstPortCLOSED,                  // minix detected, port closed, will attempt connect
	mxstNoDevicesAttached,           // minix is not connected or is not powered
	mxstMiniXControllerSelected,     // minix has been found
	mxstMiniXControllerReady,        // minix connected and ready for first command
	mxstMiniXControllerFailedToOpen, // minix detected, but failed to open
	mxstNoDeviceSelected,            // could not select minix device
	mxstRequestedVoltageOutOfRange,  // hv was selected out of range,api will set in range
	mxstRequestedCurrentOutOfRange,  // uA was selected out of range,api will set in range
	mxstConnectingToMiniX,           // api busy attempting to connect to minix
	mxstUpdatingSettings,            // api busy updating settings
	mxstMiniXReady                   // ready for next operation
};

//==== Fault Table Packet Type =======================
enum FaultTableMX2PacketType {
	ftptTimestamp,
	ftptStatus,
	ftptTube,
	ftptWarmup
};

enum InterlockViewState {
    ilvsCLOSED,
    ilvsOPEN,
    ilvsSHORTED,
    ilvsHIDE
};

//typedef struct _Packet_In {
//	unsigned char PID1;
//	unsigned char PID2;
//	unsigned short LEN;  // signed, but data payload always less than 32768
//	unsigned char STATUS;
//	unsigned char Data[32768];
//	long CheckSum;
//} Packet_In;

typedef struct _MiniX2WarmUpTable
{
	float fltWarmUpVoltage[12];
	float fltWarmUpCurrent[12];
	float fltWarmUpTime[12];
	MX2WarmupTableType WarmUpTableType;
	long DailyWarmupTotalTime;
	long MonthlyWarmupTotalTime;
} MiniX2WarmUpTable, *PMiniX2WarmUpTable;

typedef struct _TubeInterlockTableType
{
	string PartNumber;
	string SerialNumber;
	float HVmin_kV;
	float HVmax_kV;
	float Imin_uA;
	float Imax_uA;
	float Pmax_Anode_W;
	
	float HVscale_kVperV;
	float Iscale_uAperV;

	float Interlock_Voltage_V;
	float Interlock_min_I_mA;
	float Interlock_max_I_mA;
	
	float Tube_Supply_Min_V;
	float Tube_Supply_Max_V;

	string Description;
} TubeInterlockTableType, *PTubeInterlockTableType;

//holds Mini-X Fixed Constant values for Display Setup
typedef struct _MiniX_Constant
{ 
	long lTubeTypeID;						// Tube Type Identifier Index
	double dblHighVoltageConversionFactor;	// High Voltage Conversion Factor
	double dblHighVoltageMin;				// High Voltage Min
	double dblHighVoltageMax;				// High Voltage Max
	double dblDefaultdblHighVoltage;		// Default High Voltage kV
	double dblCurrentMin;					// Current Min
	double dblCurrentMax;					// Current Max
	double dblDefaultCurrent;				// Default Current
	double dblWattageMax;					// Wattage Max
	string strTubeType;						// Tube Type Name
	string strControllerType;				// Controller Type Name
	string strCtrlTypeID;					// Controller Type Short Name
} MiniX_Constant, *PMiniX_Constant;


typedef struct _DppConfigType
{
	bool CfgReadBack;       //load dpp config into config editor
	bool DisplayCfg;        //show dpp config in msgbox dialog
	bool bSpectrumCfg;      //create dpp mca file config
} DppConfigType, *PDppConfigType;

typedef struct _Stat_MNX
{
	unsigned char RAW[64];
	long SN;
	unsigned char FW;
	unsigned char Build;
	float PWR_IN;
	float PWR_XRAY;
	float HV_MON;
	float HV_MON_RAW;
	float I_MON;
	float I_MON_RAW;
	float LAMP_I;
	float Temp;
	bool HV_EN;
	bool PWR_XRAY_FLAG;
	unsigned char INTERLOCK_STATE;
	unsigned char LAST_FAULT;
	bool SPEAKER_EN;
	bool DISABLE_FAULT_CHECKS;
	bool DISABLE_LIMIT_CHECKS;
	unsigned char WARMUP_STEP;
	int WARMUP_TIME;
	long RUNTIME;
	bool ULTRAMINI_PRESENT;
	float HVScale;
	float IScale;
	bool bUSBError;
} Stat_MNX, *PStat_MNX;

//holds monitored values from ReadMiniXMonitor
typedef struct _MiniX_Monitor
{
	double mxmHighVoltage_kV;		// high voltage monitor
	double mxmCurrent_uA;			// current monitor
	double mxmPower_mW;				// power in milliwatts
	double mxmTemperatureC;			// temperature in degrees C
	unsigned char mxmRefreshed;		// monitor data refresh ok
	unsigned char mxmInterLock;		// 0=Open,1=Restored(Closed)
	unsigned char mxmEnabledCmds;	// command button enables
	unsigned char mxmStatusInd;		// minix status indicator
	unsigned char mxmOutOfRange;	// wattage value out of range
	unsigned char mxmHVOn;			// high voltage on indicator
	double mxmReserved;				// reserved, should be 123.456
} MiniX_Monitor, *PMiniX_Monitor;

//holds corrected settings from ReadMiniXSettings
typedef struct _MiniX_Settings
{
	double HighVoltage_kV;    // high voltage setting
	double Current_uA;        // current setting
} MiniX_Settings, *PMiniX_Settings;

class CMiniX2
{
public:
	CMiniX2(void);
	~CMiniX2(void);

	string strMX2AdvancedDisplay;
	bool bRequestSerNumDisplay;
	string strSerNumDisplay;		// display controller serial number

	MiniX2WarmUpTable WarmUpTable;

	//==== Display ==================================


	MiniX_Constant MxDevice;

	InterlockViewState indInterlockStatus;
	bool indDisableMonitorCmds;

	string HwCfgDP5;
	string cstrRawCfgIn;
	bool HwCfgReady;
	bool HwCfgExReady;
	vector<string> Dp5CmdList;		// already included in app
	string HwCfgDP5Out;
	string HwRdBkDP5Out;
	
	// Shared Communications Interface Settings
	bool isDppConnected;            // CurrentInterface selected and device found
	unsigned char CurrentInterface;             // selected communications interface (0=RS232/1=USB/2=ETHERNET)
	long ComPort;                      // Serial Port Settings
	
	
	DppConfigType DppConfig;
	

	//==== Tube Interlock ==================================


	
	TubeInterlockTableType TubeInterlockTable;





	Stat_MNX STATUS_MNX;   // 64 byte status packet for Mini-X
	//vector<string> varMiniX2_Faults;		// return string for code instead
	//vector<string> Fault; //(16);				// return string for code instead
	bool isMiniX2;
	bool bHaveTubeType;
	bool bRequestTubeType;
	MX2TubeTypeEnum mx2ttTubeType;
	MX2WarmupTableType WarmUpTableTypeMX2;

	string strMX2CfgIn;
	bool bMX2CfgReady;

	CDppUtilities DppUtil;

	string MX2TubeTypeEnumToString(MX2TubeTypeEnum mx2ttTubeType);
	void Process_MNX_Status(Stat_MNX *STATUS_MNX);

	void MakeFaultMX2Packet(FaultTableMX2PacketType mx2Packet, Packet_In *MX2PIN, unsigned char PIN_buffer[], long idxStart, long idxEnd);
	string Process_MNX_Fault_Record(Packet_In PIN);

	string MiniX2FaultToString(unsigned char idxFault);
	string MiniX2_StatusToString(Stat_MNX STATUS_MNX);

	string Process_MNX_Tube_Table(Packet_In PIN, TubeInterlockTableType *TubeInterlockTable);
	string Process_MNX_Warmup_Table(Packet_In PIN, MiniX2WarmUpTable *WarmUpTable, MX2WarmupTableType WarmUpTableType = mx2tbltyp50kV10W);

	string GetNowTimeString();
	string Process_MNX_Timestamp(Packet_In PIN, time_t *ttTimeStamp);
	string GetMiniXStatusString(unsigned char monitorMiniXStatus);
	void ReadMiniXSetup50kv4W(MiniX_Constant *MiniXConstant);
	void ReadMiniXSetup50kv10W(MiniX_Constant *MiniXConstant);
	void ReadMiniXSetup70kv10W(MiniX_Constant *MiniXConstant);

	string MxDblDispFormat(double dblValue);

	long CalcWarmupTotalTime(float fltWarmUpTable[], long WarmupIndexStart, long NumberWarmupIndexes=6);
	void Create50kV10W_WarmUpTable(MiniX2WarmUpTable *WarmUpTable);
	void Create50kV4W_WarmUpTable(MiniX2WarmUpTable *WarmUpTable);
	void Create70kV10W_WarmUpTable(MiniX2WarmUpTable *WarmUpTable);
};

#endif
