#define PLX_API __declspec(dllexport)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//用户读DMA内存接口
typedef void (*ReadDMA_HANDLER)(DWORD DMASize,unsigned char* MemData);
//用户写DMA内存接口
typedef void (*WriteDMA_HANDLER)(DWORD DMASize,unsigned char* MemData);

typedef struct tagPcieInfo 
{
	BOOL   PcieWriteUsed;
	double PcieWriteSize;
	double PcieWriteSpeed;
	double PcieWriteTime;
	double PcieWriteEmpty;
	double PcieWriteFull;
	double PcieWriteDDRLen;
	////////////////////////
	BOOL   PcieReadUsed;
	double PcieReadSize;
	double PcieReadSpeed;
	double PcieReadTime;
	double PcieReadEmpty;
	double PcieReadFull;
	double PcieReadDDRLen;
}PCIEINFO;


extern "C"
{
	PLX_API   long WCDPcie_GetDeviceInfo(char *InfoStr);
	PLX_API   long WCDPcie_GetDeviceInfoEx(unsigned long *VID,unsigned long *PID,unsigned long *ChainSpeed,unsigned long *ChainWidth,unsigned long *ChainIsOK,char *ChainState);

	PLX_API   long WCDPcie_OpenDevice(DWORD vendorid=0x10ee,DWORD deviceid=0x0007,unsigned long BusID=-1,unsigned long SlotID=-1);   
    PLX_API   long WCDPcie_ReadCtl(DWORD offset,UINT*outdata);
	PLX_API   long WCDPcie_WriteCtl(DWORD offset,UINT indata);	
    PLX_API   long WCDPcie_ReadReg(DWORD offset,UINT*outdata);
	PLX_API   long WCDPcie_WriteReg(DWORD offset,UINT indata);
	PLX_API   long WCDPcie_SetCardSampleFrequent(unsigned long Frequent,unsigned long ClockMode=0);
	PLX_API   long WCDPcie_GetCardSampleFrequent();

	PLX_API   long WCDPcie_Reset_AD_DDR(unsigned long AutoOrMannual=0,unsigned long Rst=0);
	PLX_API   long WCDPcie_Reset_DA_DDR(unsigned long AutoOrMannual=0,unsigned long Rst=0);

	PLX_API   long WCDPcie_StartAD();
	PLX_API   long WCDPcie_StartDA();
	PLX_API   long WCDPcie_StopAD();
	PLX_API   long WCDPcie_StopDA();
	PLX_API   long WCDPcie_CloseDevice();

	PLX_API   unsigned long WCDPcie_GetC0DDRDataSize();
	PLX_API   unsigned long WCDPcie_GetC1DDRDataSize();

	PLX_API   unsigned long WCDPcie_GetADCHNum(unsigned long *C0CHNum=NULL,unsigned long *C1CHNum=NULL);
	PLX_API   unsigned long WCDPcie_GetDACHNum(unsigned long *C0CHNum=NULL,unsigned long *C1CHNum=NULL);

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PLX_API   long WCDPcie_InitReadDMA(UINT DMAID,UINT BlockNum,UINT BlockSize);
	PLX_API   long WCDPcie_InitReadFile(UINT DMAID,UINT BlockNum,UINT BlockSize,__int64 DMASize,char *FileName,ReadDMA_HANDLER  ReadDMAFunHandle);
	PLX_API   long WCDPcie_InitReadMem(UINT DMAID,UINT BlockNum,UINT BlockSize,__int64 DMASize,unsigned char *MemData,ReadDMA_HANDLER  ReadDMAFunHandle);
	PLX_API   long WCDPcie_ReceiveReadDMA(__int64 DMASize,unsigned char *MemData);
	PLX_API   long WCDPcie_CreateReadDMAThread(BOOL CreateThreadForEnd); 
	PLX_API   long WCDPcie_StopReadDMA();
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PLX_API   long WCDPcie_InitWriteFile(UINT DMAID,UINT BlockNum,UINT BlockSize,__int64 DMASize,char *FileName,WriteDMA_HANDLER WriteDMAFunHandle);
	PLX_API   long WCDPcie_InitWriteMem(UINT DMAID,UINT BlockNum,UINT BlockSize,__int64 DMASize,unsigned char *MemData);
	PLX_API   long WCDPcie_InitWriteDMA(UINT DMAID,UINT BlockNum,UINT BlockSize);//x
	PLX_API   long WCDPcie_SendWriteDMA(__int64 DMASize,unsigned char *MemData);//x
	PLX_API   long WCDPcie_CreateWriteDMAThread(BOOL CreateThreadForEnd);
	PLX_API   long WCDPcie_StopWriteDMA();
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	PLX_API   long WCDPcie_SetC0C1Ctl(unsigned long C0_DARDEn,unsigned long C1_ADRDEn,unsigned long NowRD_C0OrC1);
	PLX_API   long WCDPcie_SetADSampleLen(unsigned long ADSampleLen);
	PLX_API   long WCDPcie_SoftTrigger();
	PLX_API   long WCDPcie_SetADParamter(unsigned long TriggerType,unsigned long PreFifoLen,unsigned long TriggerDataMax,unsigned long TriggerDataMin,unsigned long ContinueLen,unsigned long ContinueStep,unsigned long ContinueTimes);
	PLX_API   long WCDPcie_GetADStatus(unsigned long &TriggerOK,unsigned long &SoftTriggerOK,unsigned long &CHTriggerOK,unsigned long &OutTriggerOK);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PLX_API   long WCDPcieDA_InitDAParameter(unsigned long DAWorkMode=1,unsigned long StartPlayDDRSize=1500000000,unsigned __int64 RealPlaySize=0);//Max=2G

	PLX_API   PVOID* WCDPcieDA_GetWrbuffer();
	PLX_API   long  WCDPcieDA_GetCurrentWrPoint();
	PLX_API   long  WCDPcieDA_GetPcieInfo(double *PcieWriteSpeed,double *PcieWriteSize,double* PcieWriteTime);
	PLX_API   long  WCDPcieDA_GetDAMemInfo(unsigned long *WriteEmptyError,unsigned long *WriteFullError,double *WriteDDRLen);

	PLX_API   long  WCDPcieAD_GetPcieInfo(double *PcieReadSpeed,double *PcieReadSize,double* PcieReadTime);
	PLX_API   long  WCDPcieAD_GetDAMemInfo(unsigned long *ReadEmptyError,unsigned long *ReadFullError,double *ReadDDRLen);


 	PLX_API   long  WCDPcieDA_WriteSPI(unsigned int ChipID,unsigned int  Address,unsigned long Data);
	PLX_API   unsigned long  WCDPcieDA_ReadSPI(unsigned int  ChipID,unsigned int  Address);
 	PLX_API   long  WCDPcieDA_NCOSet(unsigned int ChipID,long NCOEn,unsigned long  NCOFrequent,unsigned long HLSideImage,long IQEnable,long InverseSinCEn);//0x27
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 	PLX_API   long  WCDPcieDA_SetDAMode0_FpgaDDS(long DAMode,double CH0Feq,double CH1Feq,double CH2Feq,double CH3Feq,double CH4Feq,double CH5Feq,double CH6Feq,double CH7Feq);
 	PLX_API   long  WCDPcieDA_SetDAMode1_PCData(long DAMode);
 	PLX_API   long  WCDPcieDA_SetDAMode2_FpgaConstant(long DAMode,double CH0Volt,double CH1Volt,double CH2Volt,double CH3Volt,double CH4Volt,double CH5Volt,double CH6Volt,double CH7Volt);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 	PLX_API   long  WCDPcieDA_ConfigureBegin(unsigned int ChipID);
	PLX_API   long  WCDPcieDA_ConfigureParameter(unsigned int ChipID);
 	PLX_API   long  WCDPcieDA_ConfigureEnd(unsigned int ChipID);
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	PLX_API   PVOID* WCDPcieAD_GetRdbuffer();
	PLX_API   long   WCDPcieAD_GetCurrentRdPoint();

	PLX_API   long WCDPcie_GetPcieInfo(PCIEINFO *PcieInfo);


}
//#endif
