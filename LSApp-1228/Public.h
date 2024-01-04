#ifndef _PUBLIC_DATA_H_
#define _PUBLIC_DATA_H_

#include <vector>
#include "SerialPort.h"

using namespace std;

// 设备数据
typedef struct ST_DEVICE_PARAMETER
{
	CString sName;				// 输入或输出通道名
	CString sDriver;			// 驱动
	CString sDevice;			// 设备名
	int nChannel;				// 通道，1-Mono（单通道），2-Stereo（双通道），0-未配置/无
	double nVp;					// Vp值
	double nSamplingRate;		// 采样率
	int nBitDepth;				// 量化位数
	double nSensitivity;		// MIC灵敏度
	double nGain;				// 功放增益
	int nDevIndex;				// 设备索引

	ST_DEVICE_PARAMETER()
		: sName(_T(""))
		, sDriver(_T(""))
		, sDevice(_T(""))
		, nChannel(0)
		, nVp(11)
		, nSamplingRate(44100)
		, nBitDepth(24)
		, nSensitivity(50)
		, nGain(1)
		, nDevIndex(-1)
	{}
}devParam;

// 激励数据
typedef struct ST_STIMULUS_PARAMETER
{
	CString sName;				// 激励名
	double nVoltage;			// 电压
	double nFrequencyStart;		// 开始频率
	double nFrequencyStop;		// 停止频率
	double nSamplingRate;		// 采样率
	int nOctave;				// 倍频程，0-1/3, 1-1/6, 2-1/12, 3-1/24
	int nType;					// 激励类型，0-扫频, 1-定频
	int nCycle;					// 信号步长，周期
	int nDuration;				// 信号步长，时长
	BOOL bEQEnable;				// 是否启用EQ，false-不启用，true-启用
	CString sEQPath;			// EQ路径

	ST_STIMULUS_PARAMETER()
		: sName(_T(""))
		, nVoltage(0.1)
		, nFrequencyStart(100)
		, nFrequencyStop(8000)
		, nSamplingRate(44100)
		, nOctave(0)
		, nType(0)
		, nCycle(10)
		, nDuration(10)
		, bEQEnable(false)
		, sEQPath(_T(""))
	{}
}stimParam;

// 测试项上下限
typedef struct ST_TEST_ITEM_LIMIT
{
	double nFrequency;			// 频率点
	double nLower;				// 下限
	double nUpper;				// 上限
	bool bPoint;					// 是否写频点头
	bool bLower;				// 是否写下限头
	bool bUpper;				// 是否写上限头
	bool bData;					// 是否写数据
}itemLimit;

typedef struct ST_PLAY_RECORD_PARAMETER
{
	int nMode;					// 0-SPK, 1-MIC
	bool bEnable;				// 是否测试
	CString sPlaysDev;			// 播放通道名
	CString sRecordDev;			// 采集通道名
	CString sStimName;			// 激励名
	int nPlayIndex;				// 播放通道对应索引
	int nRecordIndex;			// 采集通道对应索引
	int nStimIndex;				// 激励名对应索引
	double nTime;				// 激励播放时间
	vector<double> vPoints;		// 测试频点
	vector<int> vPointPos;		// 频点位置
	vector<int> vDatas;			// 激励数据

	ST_PLAY_RECORD_PARAMETER()
		: nMode(0)
		, sPlaysDev(_T(""))
		, sRecordDev(_T(""))
		, sStimName(_T(""))
		, nPlayIndex(-1)
		, nRecordIndex(-1)
		, nStimIndex(-1)
		, nTime(0)
	{}
}prParam;

// 测试项参数
typedef struct ST_TEST_ITEM_PARAMETER
{
	int nMode;					// 0-SPK, 1-MIC
	CString sAliasName;			// 测试项别名
	CString sItemName;			// 测试项名，如FR, THD, Sensitivity
	vector<int> vHarmonic;		// 谐波阶次
	int nMethod;				// 算法标准，0-IEC标准，1-IEEE标准
	vector<itemLimit> vLimit;	// 上下限列表
	int nWaveFormID;			// 波形窗口ID，无波形测试项忽略
	BOOL bEnable;				// 是否启用
	int nTestResult;			// 测试结果状态，0-成功，1-失败，2-未测试
	vector<double> vDatas;		// 测试结果数据

	ST_TEST_ITEM_PARAMETER()
		: nMode(0)
		, sItemName(_T(""))
		, nWaveFormID(-1)
		, bEnable(FALSE)
		, nTestResult(2)
	{}
}testItem;

typedef struct ST_DUT_PORT_PARAM
{
	CString sPort;				// 端口号
	int nBaud;					// 波特率

	ST_DUT_PORT_PARAM()
		: sPort(_T(""))
		, nBaud(921600)
	{}
}dutPort;

// 测试数据
typedef struct ST_TEST_SLOT_PARAMETER
{
	BOOL bEnable;				// 是否启用
	CString sBarcode;			// DUT SN码
	CString sBuleId;            // DUT蓝牙ID
	int nTestResult;			// 测试结果状态，-1-未启用测试，0-成功，1-失败，2-停止测试
	CString sFailMsg;			// 失败信息
	int nSlotIndex;				// 测试工位
	int nSpkPlayRecord;			// SPK播放和采集
	int nMicPlayRecord;			// MIC播放和采集
	bool bEnableBT;				// 是否连接蓝牙
	bool bBTConnected;			// 蓝牙是否连接成功
	CSerialPortDriver* pSerial;	// 串口对象
	vector<testItem> vSpkItems;	// SPK测试项
	vector<testItem> vMicItems;	// MIC测试项

	ST_TEST_SLOT_PARAMETER()
		: bEnable(FALSE)
		, sBarcode(_T(""))
		, nSlotIndex(0)
		, nSpkPlayRecord(0)
		, nMicPlayRecord(0)
		, nTestResult(-1)
		, sFailMsg(_T(""))
		, bEnableBT(true)
		, bBTConnected(false)
		, pSerial(nullptr)
	{}	
}slotParam;

//typedef struct ST_SPEAKER_CALIBRATION
//{
//
//};

extern CString IntToStr(int nValue);
extern char* WCharToChar(LPCTSTR lpData, char* pBuff, int nSize);
extern bool FileExists(LPCTSTR pFileName);
extern void ForceCreateDirectory(LPCTSTR pPath);
extern bool Split(LPCTSTR lpSrcStr, LPCTSTR lpPattern, vector<CString>& vList);
extern CString GetFilePath(LPCTSTR lpFile);
extern CString GetFileVersion(LPCTSTR lpFileName, int nNum);

extern int ReadInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault, LPCTSTR lpIniFile);
extern bool ReadBool(LPCTSTR lpSection, LPCTSTR lpKey, bool bDefault, LPCTSTR lpIniFile);
extern double ReadDouble(LPCTSTR lpSection, LPCTSTR lpKey, double nDefault, LPCTSTR lpIniFile);
extern CString ReadStr(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault, LPCTSTR lpIniFile);

class CTestData;
extern CTestData* InitTestData(LPCTSTR lpConfig);
extern void DestroyTestData();

extern CString GetAppPath();
extern CString GetConfigIniFile();
extern CString GetScriptIniFile();

extern CString GetFileFromCfg(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault);
extern CString GetPathFromCfg(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault);

extern int GetAllFrPointCount();
extern double GetIndexOfAllFrPoint(int nIndex);
extern void CalcStimData(int nPlayRecordIndex, bool bSpk);

// 设备管理
extern int GetDevCount(bool bInput);
extern int GetDevIndex(LPCTSTR lpName, bool bInput);
extern devParam* GetDevParam(int nIndex, bool bInput);
extern bool AddChannelDev(LPCTSTR lpName, bool bInput);
extern bool EditChannelDev(LPCTSTR lpItem, LPCTSTR lpValue, int nIndex, bool bInput);

// 激励
extern int GetStimCount();
extern int GetStimIndex(LPCTSTR lpName);
extern stimParam* GetStimParam(int nIndex);
extern bool AddStimParam(LPCTSTR lpName);
extern bool EditStimParam(LPCTSTR lpItem, LPCTSTR lpValue, int nIndex);
extern bool DeleteStimParam(int nIndex);

// 播放和采集
extern int GetPlayRecordCount(bool bSpkMode);
extern prParam* GetPlayRecord(int nIndex, bool bSpkMode);
extern bool AddPlayRecord(const prParam* pPR, bool bSpkMode);
extern bool EditPlayRecord(LPCTSTR lpName, LPCTSTR lpValue, int nIndex, bool bSpkMode);
extern bool DeletePlayRecord(int nIndex, bool bSpkMode);
extern int GetStimDataCount(int nIndex, bool bSpkMode);

// 测试参数
extern int GetSlotCount();
extern slotParam* GetSlotParam(int nIndex);
extern testItem* GetItemParam(LPCTSTR lpItem, bool bSpkMode = true, int nIndex = 0);
extern bool SetSlotEnable(BOOL bValue, int nIndex);
extern bool SetSlotBTConnect(BOOL bValue, int nIndex);
extern bool SetSlotItem(LPCTSTR lpItem, LPCTSTR lpValue, int nIndex);
extern bool AddSlotItemData(bool bResult, vector<double>& vItemData, LPCTSTR lpItemName, bool bSpkMode = true, int nIndex = 0);
extern bool ClearAllSlotItemData();
extern itemLimit* GetLimitParam(LPCTSTR lpItem, bool bSpkMode, double nPoint, int nIndex = 0);

#endif  // _PUBLIC_DATA_H_

