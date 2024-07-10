//#include "stdafx.h"
#include "MiniX2.h"
#include "stringex.h"
#include <time.h>

CMiniX2::CMiniX2(void)
{
}

CMiniX2::~CMiniX2(void)
{
}

string CMiniX2::MX2TubeTypeEnumToString(MX2TubeTypeEnum mx2ttTubeType)
{
	string strMX2TubeTypeEnum("");
    strMX2TubeTypeEnum = "Tube Type Unknown";
	switch(mx2ttTubeType) {
		case mx2tubetype50kV4W:
            strMX2TubeTypeEnum = "50kV4W";
			break;
		case mx2tubetype50kV10W:
            strMX2TubeTypeEnum = "50kV10W";
			break;
		case mx2tubetype70kV10W:
            strMX2TubeTypeEnum = "70kV10W";
			break;
	}
	return(strMX2TubeTypeEnum);
}

void CMiniX2::Process_MNX_Status(Stat_MNX *STATUS_MNX)
{
		 //called by ParsePacket
    float sngTemp=0.0;
    
    isMiniX2 = true;
    
    STATUS_MNX->FW = STATUS_MNX->RAW[4];
    STATUS_MNX->Build = STATUS_MNX->RAW[5] & 15;
	STATUS_MNX->RAW[3] = STATUS_MNX->RAW[3] & 0x7F;		// uses only 7 bits
//    STATUS_MNX->SN = (long)(STATUS_MNX->RAW[3] & 0x7F) * (2 ^ 24) + (long)(STATUS_MNX->RAW[2]) * (2 ^ 16) + (long)(STATUS_MNX->RAW[1]) * 256 + (long)(STATUS_MNX->RAW[0])
	STATUS_MNX->SN = (long)DppUtil.LongWordToDouble(0, STATUS_MNX->RAW);

    sngTemp = (float)((STATUS_MNX->RAW[15] & 15)) * (float)(256.0) + (float)(STATUS_MNX->RAW[14]);
    STATUS_MNX->PWR_IN = (float)(sngTemp / 1000.0 * 3.26);
    
    sngTemp = (float)((STATUS_MNX->RAW[13] & 15)) * (float)(256.0) + (float)(STATUS_MNX->RAW[12]);
    STATUS_MNX->PWR_XRAY = (float)(sngTemp / 1000.0 * 3.26);

    sngTemp = (float)((STATUS_MNX->RAW[11] & 15)) * (float)(256.0) + (float)(STATUS_MNX->RAW[10]);
    STATUS_MNX->LAMP_I = (float)(sngTemp / 1000.0 * 12.44);    // in mA
    
    sngTemp = (float)((STATUS_MNX->RAW[9] & 15)) * (float)(256.0) + (float)(STATUS_MNX->RAW[8]);
    STATUS_MNX->I_MON_RAW = (float)(sngTemp / 1000.0);                                 // monitor, in V

    sngTemp = (float)((STATUS_MNX->RAW[7] & 15)) * (float)(256.0) + (float)(STATUS_MNX->RAW[6]);
    STATUS_MNX->HV_MON_RAW = (float)(sngTemp / 1000.0);                                 // monitor, in V

    STATUS_MNX->HV_EN = DppUtil.ByteToBool(STATUS_MNX->RAW[16] & 0x80);          // D7
    STATUS_MNX->PWR_XRAY_FLAG = DppUtil.ByteToBool(STATUS_MNX->RAW[16] & 0x20);  // D5
    STATUS_MNX->INTERLOCK_STATE = (STATUS_MNX->RAW[16] & 15);  // D3-D0
    
    sngTemp = (float)(STATUS_MNX->RAW[17]);
	if (sngTemp > 127) { // 8-bit signed
        sngTemp = sngTemp - 256;
	}
    STATUS_MNX->Temp = sngTemp;
    
    STATUS_MNX->LAST_FAULT = (STATUS_MNX->RAW[18] & 15);							    // D3-D0
    STATUS_MNX->SPEAKER_EN = DppUtil.ByteToBool(STATUS_MNX->RAW[18] & 128);			    // D7
    STATUS_MNX->DISABLE_FAULT_CHECKS = DppUtil.ByteToBool(STATUS_MNX->RAW[18] & 64);    // D6
    STATUS_MNX->DISABLE_LIMIT_CHECKS = DppUtil.ByteToBool(STATUS_MNX->RAW[18] & 32);    // D5
    STATUS_MNX->ULTRAMINI_PRESENT = DppUtil.ByteToBool(STATUS_MNX->RAW[18] & 16);       // D4

    STATUS_MNX->WARMUP_STEP = STATUS_MNX->RAW[19];
    STATUS_MNX->WARMUP_TIME = STATUS_MNX->RAW[20] + (STATUS_MNX->RAW[21] & 0x7F) * 256;   // avoid overflow
    
	STATUS_MNX->RAW[25] = STATUS_MNX->RAW[25] & 0x7F;  //uses only 7 bits
//    STATUS_MNX->RUNTIME = (long)(STATUS_MNX->RAW[25] & 0x7F) * (2 ^ 24) + (long)(STATUS_MNX->RAW[24]) * (2 ^ 16) + (long)(STATUS_MNX->RAW[23]) * 256 + (long)(STATUS_MNX->RAW[22]);
    STATUS_MNX->RUNTIME = (long)DppUtil.LongWordToDouble(22, STATUS_MNX->RAW);
    
    //STATUS_MNX->HVScale = Format((STATUS_MNX->RAW[26] * 256 + STATUS_MNX->RAW[27]) / 256, "##0.000")
	STATUS_MNX->HVScale = float((STATUS_MNX->RAW[26] * 256.0 + STATUS_MNX->RAW[27]) / 256.0);
    STATUS_MNX->HV_MON = STATUS_MNX->HV_MON_RAW * STATUS_MNX->HVScale;    // in kV
    
    //STATUS_MNX->IScale = Format((STATUS_MNX->RAW[28] * 256 + STATUS_MNX->RAW[29]) / 256, "##0.000")
    STATUS_MNX->IScale = float((STATUS_MNX->RAW[28] * 256.0 + STATUS_MNX->RAW[29]) / 256.0);
    STATUS_MNX->I_MON = STATUS_MNX->I_MON_RAW * STATUS_MNX->IScale;    // in uA

		// Tube P/N, S/N and Description are in tube table
		// Mini-X S/N 
}

// create packet for existing functions to decode
void CMiniX2::MakeFaultMX2Packet(FaultTableMX2PacketType mx2Packet, Packet_In *MX2PIN, unsigned char PIN_buffer[], long idxStart, long idxEnd)
{
	const unsigned char pktPID1_RCV_STATUS = 0x80;
	const unsigned char pktPID1_RCV_SCOPE_MISC = 0x0D;
	const unsigned char pktRCVPT_MX2_STATUS = 0x02;
	const unsigned char pktRCVPT_MX2_TUBE_ILOCK_TABLE = 0x0D;
	const unsigned char pktRCVPT_MX2_WARMUP_TABLE = 0x0E;
	const unsigned char pktRCVPT_MX2_TIMESTAMP_RECORD = 0x0F;
	long idxDataIn;
	long idxDataOut=0;
	
	for(idxDataIn=idxStart;idxDataIn<=idxEnd;idxDataIn++)
	{
		MX2PIN->DATA[idxDataOut] = PIN_buffer[idxDataIn];
		idxDataOut++;
	}

	switch(mx2Packet) {
		case ftptTimestamp:
			MX2PIN->PID1 = pktPID1_RCV_SCOPE_MISC;
			MX2PIN->PID2 = pktRCVPT_MX2_TIMESTAMP_RECORD;
			MX2PIN->LEN = 0x0F;
			break;
		case ftptStatus:
			MX2PIN->PID1 = pktPID1_RCV_STATUS;
			MX2PIN->PID2 = pktRCVPT_MX2_STATUS;
			MX2PIN->LEN = 0x40;
			break;
		case ftptTube:
			MX2PIN->PID1 = pktPID1_RCV_SCOPE_MISC;
			MX2PIN->PID2 = pktRCVPT_MX2_TUBE_ILOCK_TABLE;
			MX2PIN->LEN = 0x5E;
			break;
		case ftptWarmup:
			MX2PIN->PID1 = pktPID1_RCV_SCOPE_MISC;
			MX2PIN->PID2 = pktRCVPT_MX2_WARMUP_TABLE;
			MX2PIN->LEN = 0x30;
			break;
	}
}

string CMiniX2::Process_MNX_Fault_Record(Packet_In PIN)
{
    string strFault("");
	unsigned char PIN_buffer[520];
    long idxData;
	Packet_In MX2PIN;
	Stat_MNX STATUS_MNX2;
	TubeInterlockTableType TubeInterlockTableMX2;
	MiniX2WarmUpTable WarmUpTableMX2;
	string strNowDTS("");
	float flTemp=0.0;
	stringex strfn;

    strFault = "======= Fault Record ===========\r\n";
    strFault += "======= Unit Info ===========\r\n";
	strNowDTS = GetNowTimeString();
    strFault += "Fault Record read at: " + strNowDTS + "\r\n";

	//-----------------------------------------------------------------
	//---- Save the data to local storage -----------------------------
	//-----------------------------------------------------------------
	memset(PIN_buffer,0,sizeof(PIN_buffer));
	for(idxData=0; idxData<PIN.LEN; idxData++) {
        PIN_buffer[idxData] = PIN.DATA[idxData];	// copy the fault record packet
	}

	//-----------------------------------------------------------------
	//---- Get the timestamp table ------------------------------------
	//-----------------------------------------------------------------
	memset(MX2PIN.DATA,0,sizeof(MX2PIN.DATA));
	MakeFaultMX2Packet(ftptTimestamp, &MX2PIN, PIN_buffer, 1, 15);
	time_t ttTimeStamp;		// holds tm for calcs, not used here
	strFault += "Fault occurred at: ";
	strFault += Process_MNX_Timestamp(MX2PIN, &ttTimeStamp);
	strFault += "\r\n";

	//-----------------------------------------------------------------
	//---- Get the status ---------------------------------------------
	//-----------------------------------------------------------------
	//memset(MX2PIN.DATA,0,sizeof(MX2PIN.DATA));
	MakeFaultMX2Packet(ftptStatus, &MX2PIN, PIN_buffer, 18, 81);
	for(idxData=0; idxData<MX2PIN.LEN; idxData++) {		// load the raw data into the status packet
        STATUS_MNX2.RAW[idxData] = MX2PIN.DATA[idxData];	// copy the fault record packet
	}
	Process_MNX_Status(&STATUS_MNX2);
	strFault += MiniX2_StatusToString(STATUS_MNX2);

	//////////-----------------------------------------------------------------
	//////////---- Get the Tube Table -----------------------------------------
	//////////-----------------------------------------------------------------
    //strFault += "======= Tube Table ===========\r\n";
	memset(MX2PIN.DATA,0,sizeof(MX2PIN.DATA));
	MakeFaultMX2Packet(ftptTube, &MX2PIN, PIN_buffer, 82, 175);
	strFault += Process_MNX_Tube_Table(MX2PIN, &TubeInterlockTableMX2);

	//-----------------------------------------------------------------
	//---- Get the Warmup Table ---------------------------------------
	//-----------------------------------------------------------------
    //strFault += "======= Warmup Table ===========\r\n";
	memset(MX2PIN.DATA,0,sizeof(MX2PIN.DATA));
	MakeFaultMX2Packet(ftptWarmup, &MX2PIN, PIN_buffer, 176, 223);
	strFault += Process_MNX_Warmup_Table(MX2PIN, &WarmUpTableMX2, WarmUpTableTypeMX2);

    strFault += "======= Additional Fault Info ===========\r\n";

    flTemp = (float)((((float)(PIN_buffer[226] & 127) * 256.0) + (float)PIN_buffer[227]) / 256.0);
	strFault += "Tube HV Setpoint: " + strfn.Format("%0.2fkV\r\n",flTemp);

    flTemp = (float)((((float)(PIN_buffer[228] & 127) * 256.0) + (float)PIN_buffer[229]) / 256.0);
    strFault += "Tube Current Setpoint: " + strfn.Format("%0.2fuA\r\n", flTemp);

	return(strFault);
}

string CMiniX2::MiniX2FaultToString(unsigned char idxFault)
{
    string strFault("Unknown");
	switch(idxFault) {
		case 0:
			strFault = "Interlock: CLOSED";
			break;
		case 1:
			strFault = "Interlock: OPEN";
			break;
		case 2:
			strFault = "Interlock: SHORTED";
			break;
		case 3:
			strFault = "Supply: VIN UV";
			break;
		case 4:
			strFault = "Supply: VIN OV";
			break;
		case 5:
			strFault = "Tube: HVMON LOW";
			break;
		case 6:
			strFault = "Tube: HVMON HIGH";
			break;
		case 7:
			strFault = "Tube: IMON LOW";
			break;
		case 8:
			strFault = "Tube: IMON HIGH";
			break;
		case 9:
			strFault = "USB: Disconnected";
			break;
		case 10:
			strFault = "USB: Stopped Talking";
			break;
		case 11:
			strFault = "Warmup: Done";
			break;
		case 12:
			strFault = "TBD";
			break;
		case 13:
			strFault = "TBD";
			break;
		case 14:
			strFault = "TBD";
			break;
		case 15:
			strFault = "TBD";
			break;
	}
	return(strFault);
}

string CMiniX2::MiniX2_StatusToString(Stat_MNX STATUS_MNX)
{
    string strStatus("");
	long H;
	long m;
	long S;
	stringex strfn;

    if (STATUS_MNX.SN == 0) { 
		return(strStatus);
	}
	
    strStatus += "======= Mini-X2 Status ===========\r\n";
	strStatus += "Mini-X2 Serial Number: " + strfn.Format("%04d\r\n", STATUS_MNX.SN);
    strStatus += "Firmware Version: " + strfn.Format("%d.",STATUS_MNX.FW / 16) + strfn.Format("%02d.",STATUS_MNX.FW & 15) + strfn.Format("%02d\r\n", STATUS_MNX.Build);
    strStatus += "Supply Voltage (Mini-X): " + strfn.Format("%0.2fV\r\n", STATUS_MNX.PWR_IN);
    strStatus += "Supply Voltage (to tube): " + strfn.Format("%0.2fV\r\n", STATUS_MNX.PWR_XRAY);
    strStatus += "Tube HV Mon: " + strfn.Format("%0.2fkV\r\n", STATUS_MNX.HV_MON);
    strStatus += "Tube HV Mon (raw): " + strfn.Format("%0.3fV\r\n", STATUS_MNX.HV_MON_RAW);
    strStatus += "Tube Current Mon: " + strfn.Format("%0.2fuA\r\n", STATUS_MNX.I_MON);
    strStatus += "Tube Current Mon (raw): " + strfn.Format("%0.3fV\r\n", STATUS_MNX.I_MON_RAW);
    strStatus += "Interlock Current: " + strfn.Format("%0.2fmA\r\n", STATUS_MNX.LAMP_I);
    strStatus += "Board Temp: " + strfn.Format("%0.0f°C\r\n", STATUS_MNX.Temp);
    if (STATUS_MNX.HV_EN) {
        strStatus += "HV_EN State: Enabled\r\n";
    } else {
        strStatus += "HV_EN State: Disabled\r\n";
    }
    strStatus += "Current Fault: " + MiniX2FaultToString(STATUS_MNX.INTERLOCK_STATE) + "\r\n";
    if (STATUS_MNX.LAST_FAULT > 0) {
        strStatus += "Previous Fault: " + MiniX2FaultToString(STATUS_MNX.LAST_FAULT) + "\r\n";
    } else {
        strStatus += "Previous Fault: none\r\n";
    }
    
    H = STATUS_MNX.RUNTIME / 3600;
    m = (STATUS_MNX.RUNTIME - H * 3600) / 60;
    S = STATUS_MNX.RUNTIME - H * 3600 - m * 60;
    strStatus += "Tube Runtime: " + strfn.Format("%dh", H) + " " + strfn.Format("%02dm", m) + " " + strfn.Format("%02ds\r\n", S);
    
    if (STATUS_MNX.DISABLE_LIMIT_CHECKS) {
        strStatus += "Command Limit Checks: Disabled\r\n";
    } else {
        strStatus += "Command Limit Checks: Enabled\r\n";
    }
    
    if (STATUS_MNX.DISABLE_FAULT_CHECKS) {
        strStatus += "Fault Checks: Disabled\r\n";
    } else {
        strStatus += "Fault Checks: Enabled\r\n";
    }
    
    if (STATUS_MNX.SPEAKER_EN) {
        strStatus += "Speaker: Enabled\r\n";
    } else {
        strStatus += "Speaker: Disabled\r\n";
    }
    
    if (STATUS_MNX.WARMUP_STEP > 127) {    // MSB is set if warmup is running
        strStatus += "Warmup Step: " + strfn.Format("%d\r\n", STATUS_MNX.WARMUP_STEP & 15);
        strStatus += "Warmup step time: " + strfn.Format("%d\r\n", STATUS_MNX.WARMUP_TIME);
    } else {
        strStatus += "Warmup: not running\r\n";
    }
	return(strStatus);
}

////===================================================================================
//// Here's the tube table structure from the Mini-X embedded code
////struct MNX_TT {                     // 64-byte tube and interlock table
////    unsigned char PN[20];           // 0-19: 20-character P/N
////    unsigned char SN[12];           // 20-31: 12-character S/N
////    unsigned char HVMIN;            // 32
////    unsigned char HVMAX;            // 33
////    unsigned char IMIN;             // 34
////    unsigned int  IMAX;             // 35-36
////    unsigned char PMAX;             // 37
////    unsigned char HVMIN_alt;        // 38
////    unsigned char HVMAX_alt;        // 39
////    unsigned char IMIN_alt;         // 40
////    unsigned int  IMAX_alt;         // 41-42
////    unsigned char PMAX_alt;         // 43
////    unsigned int  HVSCALE;          // 44-45
////    unsigned int  ISCALE;           // 46-47
////    unsigned char MNX_Interlock_V;      // 48: 19.5mV/LSB
////    unsigned int  MNX_Interlock_MinI;   // 49-50: 199uA/LSB
////    unsigned int  MNX_Interlock_MaxI;   // 51-52: 199uA/LSB
////    unsigned char MISC[9];          // 53-61
////    unsigned int  CHKSUM;           // 62-63
////    };
////===================================================================================
string CMiniX2::Process_MNX_Tube_Table(Packet_In PIN, TubeInterlockTableType *TubeInterlockTable)
{
//// This is called by Form1.ParsePacket
//    Dim L As Long
    long idxData;
    string strTubeTable("");
	string strTemp("");
	stringex strfn;
    strTubeTable += "======= Tube Table ===========\r\n";

	//-----------------------------------------------------------------
	//---- Tube Part Number -------------------------------------------
	//-----------------------------------------------------------------
    strTemp = "";
	for(idxData=0;idxData<=19;idxData++) {
		if (PIN.DATA[idxData] > 0) {
			strTemp += strfn.Format("%c",PIN.DATA[idxData]);
		} else {
			break;
		}
	}
    TubeInterlockTable->PartNumber = strTemp;
	strTubeTable += "Tube Part Number: " + strTemp;
	strTubeTable += "\r\n";
	//-----------------------------------------------------------------
	//---- Tube Serial Number -----------------------------------------
	//-----------------------------------------------------------------
    strTemp = "";
	for(idxData=20;idxData<=31;idxData++) {
		if (PIN.DATA[idxData] > 0) {
			strTemp += strfn.Format("%c",PIN.DATA[idxData]);
		} else {
			break;
		}
	}
	TubeInterlockTable->SerialNumber = strTemp;
	strTubeTable += "Tube Serial Number: " + TubeInterlockTable->SerialNumber;
	strTubeTable += "\r\n";
	//-----------------------------------------------------------------
	//---- Tube Description -------------------------------------------
	//-----------------------------------------------------------------
    strTemp = "";
	for(idxData=62;idxData<=93;idxData++) {
		if (PIN.DATA[idxData] > 0) {
			strTemp += strfn.Format("%c",PIN.DATA[idxData]);
		} else {
			break;
		}
	}
    TubeInterlockTable->Description = strTemp;
    strTubeTable += "Tube Description: " + strTemp;
	strTubeTable += "\r\n";
	//-----------------------------------------------------------------
	TubeInterlockTable->HVmin_kV = (float)PIN.DATA[32];		// 8-bit HVMin
	strTubeTable += strfn.Format("HVmin: %0.0fkV\r\n", TubeInterlockTable->HVmin_kV);
	//-----------------------------------------------------------------
    TubeInterlockTable->HVmax_kV = (float)PIN.DATA[33];		// 8-bit HVMax                      
	strTubeTable += strfn.Format("HVmax: %0.0fkV\r\n", TubeInterlockTable->HVmax_kV);
	//-----------------------------------------------------------------
    TubeInterlockTable->Imin_uA = (float)PIN.DATA[34];      // 8-bit Imin                
	strTubeTable += strfn.Format("Imin: %0.0fuA\r\n", TubeInterlockTable->Imin_uA);
	//-----------------------------------------------------------------
    TubeInterlockTable->Imax_uA = (float)(PIN.DATA[35] * 256 + PIN.DATA[36]);   // 16 bit Imax, 35 MSB, 36 LSB
	strTubeTable += strfn.Format("Imax: %0.0fuA\r\n", TubeInterlockTable->Imax_uA);
	//-----------------------------------------------------------------
    TubeInterlockTable->Pmax_Anode_W = (float)((float)PIN.DATA[37] / 4.0);	// 8-bit 6.2 fixed point Pmax
	strTubeTable += strfn.Format("Pmax anode: %0.2fW\r\n", TubeInterlockTable->Pmax_Anode_W);
	//-----------------------------------------------------------------
    TubeInterlockTable->HVscale_kVperV = (float)(((float)PIN.DATA[44] * 256.0 + (float)PIN.DATA[45]) / 256.0);	// 16 bit 8.8 fixed point, 44 MSB, 45 LSB
	strTubeTable += strfn.Format("HVscale: %0.2fkV/V\r\n", TubeInterlockTable->HVscale_kVperV);
	//-----------------------------------------------------------------
    TubeInterlockTable->Iscale_uAperV = (float)(((float)PIN.DATA[46] * 256.0 + (float)PIN.DATA[47]) / 256.0);	// 16 bit 8.8 fixed point, 46 MSB, 47 LSB
	strTubeTable += strfn.Format("Iscale: %0.2fuA/V\r\n", TubeInterlockTable->Iscale_uAperV);
	//-----------------------------------------------------------------

	strTubeTable += "======= Interlock Table ===========\r\n";

	//-----------------------------------------------------------------
	TubeInterlockTable->Interlock_Voltage_V = (float)((float)((float)PIN.DATA[48]) * 0.02);	// 20.0mV/LSB, 8 bit
	strTubeTable += strfn.Format("Interlock Voltage: %0.2fV\r\n", TubeInterlockTable->Interlock_Voltage_V);
	//-----------------------------------------------------------------
    TubeInterlockTable->Interlock_min_I_mA = (float)(((float)PIN.DATA[49] * 256.0 + (float)PIN.DATA[50]) * 0.01244);	// 12.44uA/LSB, 12 bit, 49 MSB, 50 LSB
	strTubeTable += strfn.Format("Interlock Current Min: %0.2fmA\r\n", TubeInterlockTable->Interlock_min_I_mA);
	//-----------------------------------------------------------------
    TubeInterlockTable->Interlock_max_I_mA = (float)(((float)PIN.DATA[51] * 256.0 + (float)PIN.DATA[52]) * 0.01244);	// 12.44uA/LSB, 12 bit, 51 MSB, 52 LSB
	strTubeTable += strfn.Format("Interlock Current Max: %0.2fmA\r\n", TubeInterlockTable->Interlock_max_I_mA);
	//-----------------------------------------------------------------
    TubeInterlockTable->Tube_Supply_Min_V = (float)(((float)PIN.DATA[53]) / 16.0);	// 8-bit 4.4 fixed point Tube supply min
	strTubeTable += strfn.Format("Tube Supply Min: %0.2fV\r\n", TubeInterlockTable->Tube_Supply_Min_V);
	//-----------------------------------------------------------------
    TubeInterlockTable->Tube_Supply_Max_V = (float)(((float)PIN.DATA[54]) / 16.0);	// 8-bit 4.4 fixed point Tube supply max
	strTubeTable += strfn.Format("Tube Supply Max: %0.2fV\r\n", TubeInterlockTable->Tube_Supply_Max_V);
    // 55-61: misc (unused)
	//-----------------------------------------------------------------
	return(strTubeTable);
}

string CMiniX2::Process_MNX_Warmup_Table(Packet_In PIN, MiniX2WarmUpTable *WarmUpTable, MX2WarmupTableType WarmUpTableType)
{
    long idxWUEntry=0;
    string strTable("");
	long lWup;
    string strErrorHelp("");
	stringex strfn;

    strErrorHelp = "Checking WarmUpTable Struct";
    //the table values are stored in variant arrays to allow quick array init
    strErrorHelp = "WarmUpTable Struct needs init";
    switch(WarmUpTableType) {
        case mx2tbltyp50kV10W:
            strErrorHelp = "Create50kV10W_WarmUpTable WarmUpTable";
            Create50kV10W_WarmUpTable(WarmUpTable);
			break;
        case mx2tbltyp70kV10W:
            strErrorHelp = "Create70kV10W_WarmUpTable WarmUpTable";
            Create70kV10W_WarmUpTable(WarmUpTable);
			break;
        case mx2tbltyp50kV4W:
            strErrorHelp = "Create50kV4W_WarmUpTable WarmUpTable";
            Create50kV4W_WarmUpTable(WarmUpTable);
			break;
        case mx2tbltypCustom:
            strErrorHelp = "Create50kV10W_WarmUpTable WarmUpTable";
            Create50kV10W_WarmUpTable(WarmUpTable);
			break;
        default:
            strErrorHelp = "Not initialized";
			break;
    }
    strErrorHelp = "WarmUpTable Struct init done";
//    
    strErrorHelp = "Loading WarmUpTable data from PIN";
	for(idxWUEntry = 0; idxWUEntry < 12; idxWUEntry++) {
        WarmUpTable->fltWarmUpVoltage[idxWUEntry] = (float)((long)(PIN.DATA[idxWUEntry * 4]));
        WarmUpTable->fltWarmUpCurrent[idxWUEntry] = (float)((long)(PIN.DATA[idxWUEntry * 4 + 1]));
        WarmUpTable->fltWarmUpTime[idxWUEntry] = (float)((long)(PIN.DATA[idxWUEntry * 4 + 2]) * 256 + (long)(PIN.DATA[idxWUEntry * 4 + 3]));
    }
    strErrorHelp = "Set Warmup type (if any)";
    WarmUpTable->WarmUpTableType = WarmUpTableType;
    strErrorHelp = "Calc daily warmup total";
    WarmUpTable->DailyWarmupTotalTime = CalcWarmupTotalTime(WarmUpTable->fltWarmUpTime, DailyWarmupIndexStart);
    strErrorHelp = "Calc monthly warmup total";
    WarmUpTable->MonthlyWarmupTotalTime = CalcWarmupTotalTime(WarmUpTable->fltWarmUpTime, MonthlyWarmupIndexStart);

    strTable += "======= Warmup Table ===========\r\n";
    strErrorHelp = "Creating Warmup Table Display";
	for(idxWUEntry = 0; idxWUEntry < 12; idxWUEntry++) {
        if (idxWUEntry < 6) {
            strTable += "Daily Warmup Step #" + strfn.Format("%01d: ", idxWUEntry + 1);
        } else {
            strTable += "Monthly Warmup Step #" + strfn.Format("%01d: ", idxWUEntry - 5);
        }
        strTable += strfn.Format("%0.0fkV, ", WarmUpTable->fltWarmUpVoltage[idxWUEntry]);
        strTable += strfn.Format("%0.0fuA, ", WarmUpTable->fltWarmUpCurrent[idxWUEntry]);
        strTable += strfn.Format("%0.0fsec\r\n", WarmUpTable->fltWarmUpTime[idxWUEntry]);
        if (idxWUEntry == 5) {
            lWup = (long)(WarmUpTable->DailyWarmupTotalTime);
            //MsgBox lWup
            strTable += "Daily Warmup Total time = " + strfn.Format("%01dm ", lWup / 60) + strfn.Format("%02ds\r\n", lWup % 60);
        } else if (idxWUEntry == 11) {
            lWup = (long)(WarmUpTable->MonthlyWarmupTotalTime);
            //MsgBox lWup
            strTable += "Monthly Warmup Total time = " + strfn.Format("%01dm ", lWup / 60) + strfn.Format("%02ds\r\n", lWup % 60);
        }
    }
	return(strTable);
}

string CMiniX2::GetNowTimeString()
{
	char buffer[256];
	time_t tNow;
    struct tm tmNow;
	string strFormat("%m/%d/%Y %I:%M:%S %p"); 
	stringex strfn;
	string strDTS("");

    tNow = time(NULL);
    tmNow = *localtime(&tNow);
	strftime(buffer, sizeof(buffer), strFormat.c_str(), &tmNow);
	strDTS = strfn.Format("%s",buffer);
	return(strDTS);
}

string CMiniX2::Process_MNX_Timestamp(Packet_In PIN, time_t *ttTimeStamp)
{
	string strMNX_Timestamp("");
	unsigned char Timestamp_flags;
	unsigned char LastOnTime[4];
	long Timetag_lag = 0;
	//====== time Processing ===================
	time_t ttNow;							// World Time
    struct tm tmLastOnTime;
	time_t ttTimeWithLag;
	char buffer[256];
	struct tm tmLastOnTime2;
	string strFormat("%m/%d/%Y %I:%M:%S %p"); 
	stringex strfn;

	//====== time Processing ===================
	ttNow = time(NULL);						// Get World Time
	tmLastOnTime = *localtime(&ttNow);		// init with current local time
    Timestamp_flags = PIN.DATA[0];
    if (Timestamp_flags < 128) {
		// set to the LastOnTime
		tmLastOnTime.tm_year = PIN.DATA[3] + 100;	// tm year to starts at 1900
		tmLastOnTime.tm_mon = PIN.DATA[1] - 1;		// tm month to starts at 0
		tmLastOnTime.tm_mday = PIN.DATA[2];
		tmLastOnTime.tm_hour = PIN.DATA[4];
		tmLastOnTime.tm_min = PIN.DATA[5];
		tmLastOnTime.tm_sec = PIN.DATA[6];
        //Timetag_lag = (long)(PIN.DATA[7] & 0x7F) * (2 ^ 24) + (long)(PIN.DATA[8]) * (2 ^ 16) + (long)(PIN.DATA[9]) * 256 + (long)(PIN.DATA[10]);
		PIN.DATA[7] = PIN.DATA[7] & 0x7F;	// msb uses only 7 bits
		LastOnTime[3] = PIN.DATA[7];
		LastOnTime[2] = PIN.DATA[8];
		LastOnTime[1] = PIN.DATA[9];
		LastOnTime[0] = PIN.DATA[10];
		Timetag_lag = (long)DppUtil.LongWordToDouble(0, LastOnTime);
		ttTimeWithLag = mktime(&tmLastOnTime) + Timetag_lag;
		tmLastOnTime2 = *localtime(&ttTimeWithLag);
		*ttTimeStamp = ttTimeWithLag;
		//*tmTimeStamp = *localtime(&ttTimeWithLag);

		//tmTimeStamp = &tmLastOnTime2;
		strftime(buffer, sizeof(buffer), strFormat.c_str(), &tmLastOnTime2);
		strMNX_Timestamp = strfn.Format("%s",buffer);
		//printf("%s\n",buffer);
    }
	return(strMNX_Timestamp);
}

//converts minix controller status codes (mxmStatusInd) to text strings
string CMiniX2::GetMiniXStatusString(unsigned char monitorMiniXStatus)
{
	string strGetMiniXStatus("");
	switch(monitorMiniXStatus) {
		case mxstNoStatus:
            strGetMiniXStatus = "No Status";
			break;
        case mxstDriversNotLoaded:
            strGetMiniXStatus = "Drivers not loaded, please install MiniX drivers.";
			break;
        case mxstMiniXApplicationReady:
            strGetMiniXStatus = "MiniX application ready.";
			break;
        case mxstPortCLOSED:
            strGetMiniXStatus = "Port CLOSED";
			break;
        case mxstNoDevicesAttached:
            strGetMiniXStatus = "No devices attached";
			break;
        case mxstMiniXControllerSelected:
            strGetMiniXStatus = "MiniX Controller selected";
			break;
        case mxstMiniXControllerReady:
            strGetMiniXStatus = "MiniX Controller ready";
			break;
        case mxstMiniXControllerFailedToOpen:
            strGetMiniXStatus = "MiniX Controller failed to open";
			break;
        case mxstNoDeviceSelected:
            strGetMiniXStatus = "No device selected";
			break;
        case mxstRequestedVoltageOutOfRange:
            strGetMiniXStatus = "Requested voltage out of range.  Please enter voltage in range.";
			break;
        case mxstRequestedCurrentOutOfRange:
            strGetMiniXStatus = "Requested current out of range.  Please enter current in range.";
			break;
        case mxstConnectingToMiniX:
            strGetMiniXStatus = "Connecting to MiniX. Please wait...";
			break;
        case mxstUpdatingSettings:
            strGetMiniXStatus = "Updating settings. Please wait...";
			break;
        case mxstMiniXReady:
            strGetMiniXStatus = "Mini-X Ready";
			break;
		default:
            strGetMiniXStatus = "Status Unknown";
			break;
	}
	return (strGetMiniXStatus);
}

void CMiniX2::ReadMiniXSetup50kv4W(MiniX_Constant *MiniXConstant)
{
    MiniXConstant->strTubeType = "Mini-X2 50kV 4W";             // Tube Type Name
    MiniXConstant->lTubeTypeID = 1;                                // Tube Type Identifier Index
    MiniXConstant->strControllerType = "Mini-X2 Controller";      // Controller Type Name
    MiniXConstant->strCtrlTypeID = "Mini-X2";                         // Controller Type Short Name
    MiniXConstant->dblHighVoltageConversionFactor = 12.5;          // High Voltage Conversion Factor
    MiniXConstant->dblHighVoltageMin = 10.0;                        // High Voltage Min
    MiniXConstant->dblHighVoltageMax = 50.0;                        // High Voltage Max
    MiniXConstant->dblDefaultdblHighVoltage = 15.0;                 // Default High Voltage kV
    MiniXConstant->dblCurrentMin = 5.0;                             // Current Min
    MiniXConstant->dblCurrentMax = 200.0;                           // Current Max
    MiniXConstant->dblDefaultCurrent = 15.0;                        // Default Current
    MiniXConstant->dblWattageMax = 4.0;                             // Wattage Max
}

void CMiniX2::ReadMiniXSetup50kv10W(MiniX_Constant *MiniXConstant)
{
    MiniXConstant->strTubeType = "Mini-X2 50kV 10W";            // Tube Type Name
    MiniXConstant->lTubeTypeID = 2;                                // Tube Type Identifier Index
    MiniXConstant->strControllerType = "Mini-X2 Controller";   // Controller Type Name
    MiniXConstant->strCtrlTypeID = "Mini-X2";                      // Controller Type Short Name
    MiniXConstant->dblHighVoltageConversionFactor = 12.5;          // High Voltage Conversion Factor
    MiniXConstant->dblHighVoltageMin = 10.0;                        // High Voltage Min
    MiniXConstant->dblHighVoltageMax = 50.0;                        // High Voltage Max
    MiniXConstant->dblDefaultdblHighVoltage = 15.0;                 // Default High Voltage kV
    MiniXConstant->dblCurrentMin = 5.0;                             // Current Min
    MiniXConstant->dblCurrentMax = 200.0;                           // Current Max
    MiniXConstant->dblDefaultCurrent = 15.0;                        // Default Current
    MiniXConstant->dblWattageMax = 10.0;                            // Wattage Max
}

void CMiniX2::ReadMiniXSetup70kv10W(MiniX_Constant *MiniXConstant)
{
    MiniXConstant->strTubeType = "Mini-X2 70kV 10W";            // Tube Type Name
    MiniXConstant->lTubeTypeID = 0;                                // Tube Type Identifier Index
    MiniXConstant->strControllerType = "Mini-X2 Controller";      // Controller Type Name
    MiniXConstant->strCtrlTypeID = "Mini-X2";                         // Controller Type Short Name
    MiniXConstant->dblHighVoltageConversionFactor = 20.0;           // High Voltage Conversion Factor
    MiniXConstant->dblHighVoltageMin = 35.0;                        // High Voltage Min
    MiniXConstant->dblHighVoltageMax = 70.0;                        // High Voltage Max
    MiniXConstant->dblDefaultdblHighVoltage = 45.0;                 // Default High Voltage kV
    MiniXConstant->dblCurrentMin = 10.0;                            // Current Min
    MiniXConstant->dblCurrentMax = 143.0;                           // Current Max
    MiniXConstant->dblDefaultCurrent = 15.0;                        // Default Current
    MiniXConstant->dblWattageMax = 10.0;                            // Wattage Max
}

string CMiniX2::MxDblDispFormat(double dblValue)
{
	stringex strfn;
	string strDisplay("");
	strDisplay = strfn.Format("%.0f",dblValue);
	return(strDisplay);
}

long CMiniX2::CalcWarmupTotalTime(float fltWarmUpTable[], long WarmupIndexStart, long NumberWarmupIndexes)
{
    long idxWarmup = 0;
    float sngTotal = 0.0;
	for(idxWarmup = WarmupIndexStart; idxWarmup < (WarmupIndexStart + NumberWarmupIndexes); idxWarmup++)
	{
        sngTotal = sngTotal + fltWarmUpTable[idxWarmup];
    }
	return((long)sngTotal);
}

// 50kV10W Daily warm-up 0-5,Monthly warm-up 6-11
void CMiniX2::Create50kV10W_WarmUpTable(MiniX2WarmUpTable *WarmUpTable)
{
	float fltWarmUpVoltage[12] = {25.0, 30.0, 40.0, 40.0, 50.0, 20.0, 25.0, 30.0, 40.0, 40.0, 50.0, 20.0};
	float fltWarmUpCurrent[12] = {20.0, 50.0, 50.0, 100.0, 200.0, 200.0, 20.0, 50.0, 50.0, 100.0, 200.0, 200.0};
	float fltWarmUpTime[12] = {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 300.0, 300.0, 300.0, 600.0, 600.0, 600.0};
	for(int i=0;i<12;i++){
		WarmUpTable->fltWarmUpVoltage[i] = fltWarmUpVoltage[i];
		WarmUpTable->fltWarmUpCurrent[i] = fltWarmUpCurrent[i];
		WarmUpTable->fltWarmUpTime[i] = fltWarmUpTime[i];
	}
    WarmUpTable->WarmUpTableType = mx2tbltyp50kV10W;
    WarmUpTable->DailyWarmupTotalTime = CalcWarmupTotalTime(WarmUpTable->fltWarmUpTime, DailyWarmupIndexStart);
    WarmUpTable->MonthlyWarmupTotalTime = CalcWarmupTotalTime(WarmUpTable->fltWarmUpTime, MonthlyWarmupIndexStart);
}

// 50kV4W Daily warm-up 0-5,Monthly warm-up 6-11
void CMiniX2::Create50kV4W_WarmUpTable(MiniX2WarmUpTable *WarmUpTable)
{
	float fltWarmUpVoltage[12] = {25.0, 30.0, 40.0, 40.0, 50.0, 20.0, 25.0, 30.0, 40.0, 40.0, 50.0, 20.0};
    float fltWarmUpCurrent[12] = {20.0, 50.0, 50.0, 100.0, 80.0, 200.0, 20.0, 50.0, 50.0, 100.0, 80.0, 200.0};
    float fltWarmUpTime[12] = {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 300.0, 300.0, 300.0, 600.0, 600.0, 600.0};
	for(int i=0;i<12;i++){
		WarmUpTable->fltWarmUpVoltage[i] = fltWarmUpVoltage[i];
		WarmUpTable->fltWarmUpCurrent[i] = fltWarmUpCurrent[i];
		WarmUpTable->fltWarmUpTime[i] = fltWarmUpTime[i];
	}
    WarmUpTable->WarmUpTableType = mx2tbltyp50kV4W;
    WarmUpTable->DailyWarmupTotalTime = CalcWarmupTotalTime(WarmUpTable->fltWarmUpTime, DailyWarmupIndexStart);
    WarmUpTable->MonthlyWarmupTotalTime = CalcWarmupTotalTime(WarmUpTable->fltWarmUpTime, MonthlyWarmupIndexStart);
}

// 70kV10W Daily warm-up 0-5,Monthly warm-up 6-11
void CMiniX2::Create70kV10W_WarmUpTable(MiniX2WarmUpTable *WarmUpTable)
{
    float fltWarmUpVoltage[12] = {10.0, 35.0, 50.0, 50.0, 60.0, 70.0, 10.0, 25.0, 40.0, 50.0, 50.0, 60.0};
    float fltWarmUpCurrent[12] = {20.0, 20.0, 100.0, 200.0, 166.0, 143.0, 50.0, 50.0, 50.0, 100.0, 200.0, 266.0};
    float fltWarmUpTime[12] = {5.0, 5.0, 5.0, 5.0, 5.0, 5.0, 300.0, 300.0, 300.0, 300.0, 300.0, 300.0};
	for(int i=0;i<12;i++){
		WarmUpTable->fltWarmUpVoltage[i] = fltWarmUpVoltage[i];
		WarmUpTable->fltWarmUpCurrent[i] = fltWarmUpCurrent[i];
		WarmUpTable->fltWarmUpTime[i] = fltWarmUpTime[i];
	}
    WarmUpTable->WarmUpTableType = mx2tbltyp70kV10W;
    WarmUpTable->DailyWarmupTotalTime = CalcWarmupTotalTime(WarmUpTable->fltWarmUpTime, DailyWarmupIndexStart);
    WarmUpTable->MonthlyWarmupTotalTime = CalcWarmupTotalTime(WarmUpTable->fltWarmUpTime, MonthlyWarmupIndexStart);
}











