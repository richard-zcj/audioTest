#ifndef _PUBLIC_DATA_H_
#define _PUBLIC_DATA_H_

#include <vector>
#include "SerialPort.h"

using namespace std;

// �豸����
typedef struct ST_DEVICE_PARAMETER
{
	CString sName;				// ��������ͨ����
	CString sDriver;			// ����
	CString sDevice;			// �豸��
	int nChannel;				// ͨ����1-Mono����ͨ������2-Stereo��˫ͨ������0-δ����/��
	double nVp;					// Vpֵ
	double nSamplingRate;		// ������
	int nBitDepth;				// ����λ��
	double nSensitivity;		// MIC������
	double nGain;				// ��������
	int nDevIndex;				// �豸����

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

// ��������
typedef struct ST_STIMULUS_PARAMETER
{
	CString sName;				// ������
	double nVoltage;			// ��ѹ
	double nFrequencyStart;		// ��ʼƵ��
	double nFrequencyStop;		// ֹͣƵ��
	double nSamplingRate;		// ������
	int nOctave;				// ��Ƶ�̣�0-1/3, 1-1/6, 2-1/12, 3-1/24
	int nType;					// �������ͣ�0-ɨƵ, 1-��Ƶ
	int nCycle;					// �źŲ���������
	int nDuration;				// �źŲ�����ʱ��
	BOOL bEQEnable;				// �Ƿ�����EQ��false-�����ã�true-����
	CString sEQPath;			// EQ·��

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

// ������������
typedef struct ST_TEST_ITEM_LIMIT
{
	double nFrequency;			// Ƶ�ʵ�
	double nLower;				// ����
	double nUpper;				// ����
	bool bPoint;					// �Ƿ�дƵ��ͷ
	bool bLower;				// �Ƿ�д����ͷ
	bool bUpper;				// �Ƿ�д����ͷ
	bool bData;					// �Ƿ�д����
}itemLimit;

typedef struct ST_PLAY_RECORD_PARAMETER
{
	int nMode;					// 0-SPK, 1-MIC
	bool bEnable;				// �Ƿ����
	CString sPlaysDev;			// ����ͨ����
	CString sRecordDev;			// �ɼ�ͨ����
	CString sStimName;			// ������
	int nPlayIndex;				// ����ͨ����Ӧ����
	int nRecordIndex;			// �ɼ�ͨ����Ӧ����
	int nStimIndex;				// ��������Ӧ����
	double nTime;				// ��������ʱ��
	vector<double> vPoints;		// ����Ƶ��
	vector<int> vPointPos;		// Ƶ��λ��
	vector<int> vDatas;			// ��������

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

// ���������
typedef struct ST_TEST_ITEM_PARAMETER
{
	int nMode;					// 0-SPK, 1-MIC
	CString sAliasName;			// ���������
	CString sItemName;			// ������������FR, THD, Sensitivity
	vector<int> vHarmonic;		// г���״�
	int nMethod;				// �㷨��׼��0-IEC��׼��1-IEEE��׼
	vector<itemLimit> vLimit;	// �������б�
	int nWaveFormID;			// ���δ���ID���޲��β��������
	BOOL bEnable;				// �Ƿ�����
	int nTestResult;			// ���Խ��״̬��0-�ɹ���1-ʧ�ܣ�2-δ����
	vector<double> vDatas;		// ���Խ������

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
	CString sPort;				// �˿ں�
	int nBaud;					// ������

	ST_DUT_PORT_PARAM()
		: sPort(_T(""))
		, nBaud(921600)
	{}
}dutPort;

// ��������
typedef struct ST_TEST_SLOT_PARAMETER
{
	BOOL bEnable;				// �Ƿ�����
	CString sBarcode;			// DUT SN��
	CString sBuleId;            // DUT����ID
	int nTestResult;			// ���Խ��״̬��-1-δ���ò��ԣ�0-�ɹ���1-ʧ�ܣ�2-ֹͣ����
	CString sFailMsg;			// ʧ����Ϣ
	int nSlotIndex;				// ���Թ�λ
	int nSpkPlayRecord;			// SPK���źͲɼ�
	int nMicPlayRecord;			// MIC���źͲɼ�
	bool bEnableBT;				// �Ƿ���������
	bool bBTConnected;			// �����Ƿ����ӳɹ�
	CSerialPortDriver* pSerial;	// ���ڶ���
	vector<testItem> vSpkItems;	// SPK������
	vector<testItem> vMicItems;	// MIC������

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

// �豸����
extern int GetDevCount(bool bInput);
extern int GetDevIndex(LPCTSTR lpName, bool bInput);
extern devParam* GetDevParam(int nIndex, bool bInput);
extern bool AddChannelDev(LPCTSTR lpName, bool bInput);
extern bool EditChannelDev(LPCTSTR lpItem, LPCTSTR lpValue, int nIndex, bool bInput);

// ����
extern int GetStimCount();
extern int GetStimIndex(LPCTSTR lpName);
extern stimParam* GetStimParam(int nIndex);
extern bool AddStimParam(LPCTSTR lpName);
extern bool EditStimParam(LPCTSTR lpItem, LPCTSTR lpValue, int nIndex);
extern bool DeleteStimParam(int nIndex);

// ���źͲɼ�
extern int GetPlayRecordCount(bool bSpkMode);
extern prParam* GetPlayRecord(int nIndex, bool bSpkMode);
extern bool AddPlayRecord(const prParam* pPR, bool bSpkMode);
extern bool EditPlayRecord(LPCTSTR lpName, LPCTSTR lpValue, int nIndex, bool bSpkMode);
extern bool DeletePlayRecord(int nIndex, bool bSpkMode);
extern int GetStimDataCount(int nIndex, bool bSpkMode);

// ���Բ���
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

