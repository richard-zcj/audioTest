#ifndef _TEST_DATA_H_
#define _TEST_DATA_H_

#include <map>

class CTestData
{
public:
	CTestData(LPCTSTR lpConfig);
	~CTestData();

	vector<double> m_vAllPoints;		// 所有频点
	vector<devParam> m_vInputDevs;		// 输入通道设备
	vector<devParam> m_vOutputDevs;		// 输出通道设备
	vector<stimParam> m_vStimParam;		// 激励数据
	vector<prParam> m_vSpkPlayRecord;	// SPK模式播放和采集
	vector<prParam> m_vMicPlayRecord;	// MIC模式播放和采集
	vector<slotParam> m_vSlotParam;		// 测试参数数据

	int CfgReadInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault);
	bool CfgReadBool(LPCTSTR lpSection, LPCTSTR lpKey, bool bDefault);
	double CfgReadDouble(LPCTSTR lpSection, LPCTSTR lpKey, double nDefault);
	CString CfgReadStr(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault);
	bool CfgWriteStr(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpValue);

	int ScpReadInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault);
	bool ScpReadBool(LPCTSTR lpSection, LPCTSTR lpKey, bool bDefault);
	double ScpReadDouble(LPCTSTR lpSection, LPCTSTR lpKey, double nDefault);
	CString ScpReadStr(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault);
	bool ScpWriteStr(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpValue);

	CString GetWorkPath();
	CString GetConfigFile();
	CString GetScriptFile();
	CString GetFileFromConfig(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault);
	CString GetPathFromConfig(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault);

	void CalcStimulusData(int nPRIndex, bool bSpk);

	int GetDeviceIndex(LPCTSTR lpName, bool bInput);
	bool AddDeviceData(LPCTSTR lpName, bool bInput);
	bool EditDeviceData(LPCTSTR lpName, LPCTSTR lpValue, int nIndex, bool bInput);

	int GetStimulusIndex(LPCTSTR lpName);
	bool AddStimulusParam(LPCTSTR lpName);
	bool EditStimulusParam(LPCTSTR lpItem, LPCTSTR lpValue, int nIndex);
	bool DeleteStimulusParam(int nIndex);
	
	bool AddPlayRecordItem(const prParam* pPR, bool bSpkMode);
	bool EditPlayRecordItem(LPCTSTR lpName, LPCTSTR lpValue, int nIndex, bool bSpkMode);
	bool DeletePlayRecordItem(int nIndex, bool bSpkMode);

	bool SetTestSlotItem(LPCTSTR lpItem, LPCTSTR lpValue, int nIndex);
	bool AddTestItemData(bool bResult, vector<double>& vItemData, LPCTSTR lpItemName, bool bSpkMode, int nIndex);
	bool ClearAllTestItemData();
	testItem* GetTestItemParam(LPCTSTR lpItem, bool bSpkMode, int nIndex);
	itemLimit* GetPointLimitParam(LPCTSTR lpItem, bool bSpkMode, double nPoint, int nIndex);

private:
	CString m_sAppPath;
	CString m_sConfigFile;
	CString m_sScriptFile;

	map<double, double> m_mEQValue;

	// 根据通道索引获取通道名
	CString GetChannelName(int nIndex);
	// 根据通道名获取通道索引
	int GetChannelIndex(LPCTSTR lpName);
	// 加载测试项参数
	void GetTestItems(slotParam* pSlot, bool bSpk);
	// 加载测试项limit
	void GetItemLimit(LPCTSTR lpSection, testItem* pItem);
	// 从文件中加载EQ值
	void GetEQValues(const prParam* pPlayRecord);

	// 加载所有频点
	void LoadAllFrequencyPoints();

	// 加载设备数据
	void LoadDeviceData();

	// 加载激励数据
	void LoadStimulusParam();

	// 加载播放和采集
	void LoadPlayAndRecord();

	// 加载测试参数
	void LoadTestSlotParam();


	double FindFrPointPos(double nVal, int& nPos);
	void GetFrPoints(prParam* pPlayRecord);
	//testItem* GetItemIndex(slotParam* pTest, LPCTSTR lpItemName);
};

#endif  // _TEST_DATA_H_

