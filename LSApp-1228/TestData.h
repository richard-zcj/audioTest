#ifndef _TEST_DATA_H_
#define _TEST_DATA_H_

#include <map>

class CTestData
{
public:
	CTestData(LPCTSTR lpConfig);
	~CTestData();

	vector<double> m_vAllPoints;		// ����Ƶ��
	vector<devParam> m_vInputDevs;		// ����ͨ���豸
	vector<devParam> m_vOutputDevs;		// ���ͨ���豸
	vector<stimParam> m_vStimParam;		// ��������
	vector<prParam> m_vSpkPlayRecord;	// SPKģʽ���źͲɼ�
	vector<prParam> m_vMicPlayRecord;	// MICģʽ���źͲɼ�
	vector<slotParam> m_vSlotParam;		// ���Բ�������

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

	// ����ͨ��������ȡͨ����
	CString GetChannelName(int nIndex);
	// ����ͨ������ȡͨ������
	int GetChannelIndex(LPCTSTR lpName);
	// ���ز��������
	void GetTestItems(slotParam* pSlot, bool bSpk);
	// ���ز�����limit
	void GetItemLimit(LPCTSTR lpSection, testItem* pItem);
	// ���ļ��м���EQֵ
	void GetEQValues(const prParam* pPlayRecord);

	// ��������Ƶ��
	void LoadAllFrequencyPoints();

	// �����豸����
	void LoadDeviceData();

	// ���ؼ�������
	void LoadStimulusParam();

	// ���ز��źͲɼ�
	void LoadPlayAndRecord();

	// ���ز��Բ���
	void LoadTestSlotParam();


	double FindFrPointPos(double nVal, int& nPos);
	void GetFrPoints(prParam* pPlayRecord);
	//testItem* GetItemIndex(slotParam* pTest, LPCTSTR lpItemName);
};

#endif  // _TEST_DATA_H_

