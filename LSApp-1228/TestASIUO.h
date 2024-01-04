#pragma once
#include "asio.h"
#include "a4aInterface.h"
//ASIOBool init (void* sysRef);
#include <Windows.h>
#include <MMSystem.h>
#include "asiosmpl.h"
#include <vector>
#include "MainFrm.h"
#include <map>
#include <mutex>

using namespace std;
#define RUNMODE 0 
#define RUNTESTASIOMODE 0 

typedef ASIOBool(*pinit)(void *sysRef);


#define SAMPLENUMS 48000*10*4
#define SAMPLENUMS2 44100*10*4
#define CHUNK_SIZE 48000*10*4
#define CHUNK_SIZE2 44100*10*4
#define SAMPLES 48000

struct MMERecordParams
{
	double time;
	int chs;
	int bit;
	DWORD fs;
	int index;
	int mode;	// 0-MIC, 1-SPK
};


typedef void(*CallBackDraw)();

class TestASIUO
{
public:
	TestASIUO();
	~TestASIUO();

	void LoadDll();

	void LoadASIO4ALL();
	void LoadASIO4ALLFunc();

	void LoadWinMM();

	void playThread();
	void recordThread();
	void openRecordThread(double time,int index);//wdm录音


	void play(CString, double, int index,CString palyChsName);

	void record(int,void*);

	void recordMME(double recordTimes, void* obj,int chs,int bits,DWORD fs,int i);
	void startMM();
	//void writeWaveFile(const MMERecordParams& mmesparam, int nIndex, char* pData);
	//void MMEinit(int n, double time, const MMERecordParams& mmesparam, int nIndex);
	void MMEinit(int n, double time, const vector<MMERecordParams>& mmesparam, int nIndex = -1);
	void createMMEbuffer(int n);
	void delMMEbuffer();
	void createMMEbuffer();
	void submitPlayData(DWORD len);
	void playMME(int recordTimes, void* obj);

	void MMEFree(double nTime, bool bSpk, int nIndex = -1);

	char* getcharp(CString str);

	void stopRe();
	void playAndRecord(int, CString);
	void wdmplayAndRecord(int time, int index_in, CString playfilename, int index_out,CString inChsName,double fs);//从该函数获取参数
	void wdmplayAndRecordthread();

	void mmeplayAndRecord(double fs, int bits, int recordmmeIndex, int playmmeIndex,double outvp);

	void mmelayAndRecordthread();

	void LoadWaveData();

	bool StartRME();
	void StopRME();

	void SetCallBackFunc(CallBackDraw call);

	bool initRMECard();

	bool wdmisRecordDone();
	void setwdmisRecordDone(bool b);


	bool LoadWDM(vector<wstring>& driver, vector<int>& ch, bool isinput);
	bool LoadASIO(vector<wstring>& driver, vector<int>& ch, bool isinput);


	void LoadRSASIO(vector<wstring>& driver, vector<string>& ch, bool binput, int type);
	void LoadRSASIO2(vector<wstring>& driver, vector<string>& ch, bool binput, int type);

	void LoadYamahaASIODriever();


	int createASIOBuffer(double device_fs);
	int createASIOBuffer2(double device_fs);


	void playASIO(int* index, int**data, int count, double device_fs);
	void recordASIO(int* index, int***data, int count, double device_fs, double recordtime);

	void micCalibrationRecordASIO(int* index, int***data, int count, double device_fs, int recordtime);

	void setplaydata(int**pldata, int count,int* playchs);

	HRESULT SetDefaultAudio(PCWSTR devId, int setType);


	void setWDMplaych(int index, PCWSTR Name,bool isInput);

	void setwdmindex_out(CString name);
	void setwdmName_in(CString name);

	void setMainf(CMainFrame* mf);


	int getPlayCount();

	int getWdmChsIndex(bool isInput, CString chsName);

	void setASIOStop(bool bstop);
	void cal_vpByMME(double fs, int chs, int bit,int dev_id,double outvp);
	void saveMMEFile(double fs, int chs, int bit, double time);

	void playMME(int* data, double fs, int chs, int bit, int dev_id, double outvp);
	
	CString GetLastErrMsg();

	void setIsNoPlay(bool bplay);
	


private:
	HMODULE hModuleDll;

	pinit init;

	
	double* indata;
	double* outdata;

	int DeviceIndex;
	int InterfaceIndex;

	int re_time;
	
	int wdmindex_out;//wdm  输出通道
	int wdmindex_in;//wdm  输出通道
	CString wdmName_out;
	CString wdmName_in;

	CString palyname;

	
	HWAVEIN hWaveIn = NULL;//录音设备初始化
	WAVEFORMATEX waveform;
	IASIO* samp = NULL;

	int initcardtimes1;
	int initcardtimes2;
	int initcardtimes3;
	int initcardtimes4;

	bool bstop;

	CallBackDraw calldaw;

	bool initRME;
	
	HANDLE hThreadId;

	bool bfirstrun;

	double valote;

	bool  wdmrecorddone;
	bool  wdmplaydone;
	

	CMainFrame* mainf;
	double m_fs;


	//mme param

	DWORD play_len;
	int* playdatamme;
	int paly_count;


	bool m_StopASIO_Record;

	map<int, IASIO*> m_ASIODriverList;

	CString m_sLastErrMsg;


	bool m_IsNoPlay;

	

	

};


