#pragma once

#include "afxwin.h"
#include "afxcmn.h"
#include "FrTest.h"
#include "Thd.h"
#include "ThdN.h"
#include <vector>
#include "RubZ.h"
#include "Phase.h"
#include "Sensitivity.h"
#include "TestRME.h"
#include "BlueComm.h"
#include "MainFrm.h"
#include "DataShow.h"
#include "TestBorderLineSettings.h"
#include "AudioAlgorithm.h"
#include "ResShow.h"
#include "TestResManagement.h"
#include "WaveShow.h"
#include "DeviceManage.h"
#include "PlayRecordSettings.h"
#include "Public.h"
#include "Scanlot.h"
#include "HttpClient.h"

#define SIMNICK 1
#define ALGOFFSET 1

using namespace std;
// CBlueEreTest 对话框

class CBlueEreTest : public CDialogEx
{
	DECLARE_DYNAMIC(CBlueEreTest)

public:
	CBlueEreTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlueEreTest();
	void setRunCycles(bool brun);

	void setTestCycleTimes(int CycleTimes);
	int getTestCycleTimes();

	void setTestCyclePeriod(int CycleTimes);
	int getTestCyclePeriod();

	void setTestCyclecolor(COLORREF color);
	COLORREF getTestCyclecolor();

	void SetIsOffline(bool onlineSta);

	void HideProcess(bool bhide);
	void destroyRes();
	void setOffline(bool bstatus);
	void setHTTPClinet(CHttpClient* pHttp, CString staton);
	void MesDataPost(CString sTestItem, bool rdbFail, CString ErrCode, CString ErrItem, CString sSN);

// 对话框数据
	enum { IDD = IDD_DlgBlueEre };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	
	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonRecord();
	bool FrChecked(double hz, double down, double up, vector<double>* data, vector<double>* res, const vector<Mutidata*>& borderLine);

	double getPhaseMinVal(vector<double>& phase_data,bool& bmove);

	void ShowRes();
	CTestResManagement* GetResManagement(int nIndex);
	

private:
	


	bool m_boffline;
	bool m_IsOffline;
	bool m_connBuleIsFial;

	int m_time;

	CString m_testID;

	int testCycleTimes;
	int currentTestTimes;
	bool m_isrunCycles;
	int cyclesPeriod;
	bool m_bTesting;
	bool m_bloadDriver;
	COLORREF m_color;
	

	CFrTest mfr;
	CThd    mthd;
	CThdN   mthdn;
	CRubZ   mrubz;
	CPhase  mphase;
	CSensitivity msen;

	CBlueComm bluecomm;

	BOOL m_slectall;
	BOOL m_bfr;
	BOOL m_bthd;
	BOOL m_bthdn;
	BOOL m_bRub_Buzz;
	BOOL m_bPhase;
	BOOL m_bSensitivity_spk;
	BOOL m_bSensitivity_mic;
	BOOL M_Polarity;
	BOOL m_Balance;
	BOOL m_NoiseReduction;
	BOOL m_snr;//信噪比测试项
	//定义6个测试项的结果集
	vector<double> datafr;
	vector<double> datathd;
	vector<double> datathdn;
	vector<double> datarub;
	vector<double> dataphase;
	vector<double> datasen;
	double dut_sen1;
	double dut_sen2;
	double dut_sen3;
	double dut_sen4;

	vector<double> dataBalance1;
	vector<double> dataBalance2;

	vector<double> dataNoiseReduction1;
	vector<double> dataNoiseReduction2;
	vector<int*> asioRecordData;

	double m_snr1;//输入电压0v空测
	double m_snr2;//带电压测试
	bool m_btestSnr;

	double m_show_min;
	double m_show_min_thd;
	double m_show_min_rb;


	CMainFrame* mainf;

	TestRME testRme;

	CDataShow* datashow;
	CDataShow* datashow2;
	CDataShow* datashow3;
	CDataShow* datashow4;

	CDataShow* datashowthd;
	CDataShow* datashowthd2;
	CDataShow* datashowthd3;
	CDataShow* datashowthd4;

	CDataShow* datashowthd_N;

	CDataShow* datashowRB;
	CDataShow* datashowRB2;
	CDataShow* datashowRB3;
	CDataShow* datashowRB4;

	CDataShow* datashowPhase;
	CDataShow* resshowBalance;
	CDataShow* dataNoiseReduction;

	CResShow* resshowSensitivity;
	CResShow* resshowPolarity;
	CResShow* resshowSNR;

	CScanlot scanlot;




	//vector<CDataShow*> micres;//fr thd sen  *4=12                4fr   4thd   4sen
	map<int, CDataShow*> micres;
	//vector<CDataShow*> spkres;//fr thd rb  *4=12   +=24          ----
	map<int, CDataShow*> spkres;

	
	TestASIUO testASIUO;
	CHttpClient* m_pHttp;

	CString m_station;
	CString m_sn;


	CAudioAlgorithm audiotest;
	CTestResManagement testResManage;
	CTestResManagement testResManage2;
	CTestResManagement testResManage3;
	CTestResManagement testResManage4;
	CWaveShow          waveShow;
	CBrush brush1;

	/********编辑框是否聚焦********/
	bool focus_num;
	/*********是否测试SPK********************/
	bool m_btestSPK;

	/**********测试结果***************/
	bool bIsSucess;

	/*******************私有函数***********************/
	void play(char* data);
	void recordTest();
	void cyclesTest();
	void drawGroupBox(CWnd* wnd, CPaintDC* dc);
	void updateanalysisStatus_spk(const vector<int>* analysisdata, int cycle, char* name);
	void updateanalysisStatus_mic(const vector<int>* analysisdata, int cycle, char* name);
	void CloseResWindow();

	void fendaTestOnePlayOneRecord();

	double GetTestParamData(vector<PlayRecordDialogData*>* prdata, vector<MMERecordParams> &mmeparams);

	/*double FindPos(double nVal, int& nPos);
	void GetFrPoints(int nStimIndex);
	void CalcStimulusData(slotParam* pSlot, bool bSpk);*/

	bool InitASIODriver(const CString& sDevName, bool bInput);
	bool SingleRecord(double nTime, devParam* pDev, int*** pData);
	double MultiRecord();
	bool ConnectBTSwitch(int nSlot, const char* pBtID, bool bSpkMode = true);
	void DisonnectBT(int nSlot);

	bool CheckFRLimit(vector<Mutidata*>& frLimit, vector<double>& borderX,
		vector<double>& downY, vector<double>& upY, prParam* pPlayRecord,vector<double>& checkData);
	bool CheckTHDLimit(vector<Mutidata*>& thdLimit, vector<double>& borderX,
		vector<double>& downY, vector<double>& upY, prParam* pPlayRecord, vector<double>& checkData);

	bool ConnectAllBT(CScanlot* scallot);
	void DisconnectAllBT();

	bool SpkModeTest(bool& bEnable,bool bInfo=true);
	bool MicModeTest(bool& bEnable);
	bool ConnFenDaWatch(const char* pBtID, int nSlot);
	bool SendBTDataAndCheck(const slotParam* pSlot, const char* pSend, const char* pRes);

	void movephase(vector<double>& movedata,double moveVal);
	void revermovephase(vector<double>& movedata, double moveVal);

	void GetTestItems(CString& testItems,CString itemName);

	bool GetErrCode(CString& errcode, CString& erritem,bool* res,int nslot);

	void DelDir(CString path);

	void PlaySharkJili();
	double GetSensitivity(const vector<double>& data, prParam* pPlayRecord);
	void WriteDoubleData(CArchive& ar, double dbData, int nNum = 0);
	void WriteStringData(CArchive& ar, const char* pData);
	void SavePointHead(LPCTSTR lpItemName, bool bSpkMode, CArchive& ar, prParam* pPR);
	void SaveLimitHead(LPCTSTR lpItemName, bool bSpkMode, CArchive& ar, prParam* pPR, bool bLower = true);
	void WriteItemData(LPCTSTR lpItemName, bool bSpkMode, CArchive& ar, int nIndex, const char* pTestItem);
	void SaveDataToCsv();
	CString GetSaveFileName();

public:
	afx_msg void OnBnClickedButtonRecordOnly();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CComboBox m_cmbTestItem;
	afx_msg void OnBnClickedRadio1();
	CTabCtrl m_tab;
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnBnClickedButtonTest2();
	afx_msg void OnBnClickedButtonTest3();
	afx_msg void OnBnClickedButtonTest4();
	afx_msg void OnBnClickedButtonRecordTest();
	afx_msg void OnBnClickedButtonAutoConnEre();
	afx_msg void OnBnClickedButtonDisconnectEre();
	CComboBox mInchsnames;
	CComboBox mOutchsnames;
	afx_msg void OnCbnDropdowneretestincom();
	afx_msg void OnCbnDropdowneretestoutcom();
	CStatic m_staticPlayTimes;
	
	int playtimes;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButtonTestCycles();
	afx_msg void OnBnClickedButtonStopTest();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonSwitchBlueMode();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnOK();
	afx_msg void OnEnSetfocusEdittestid();
	afx_msg void OnEnKillfocusEdittestid();
	CEdit m_EditTextNum;
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	afx_msg LRESULT OnResItemShow(WPARAM wParam,LPARAM lParam);
	CButton m_startTest;
	CButton m_stopTest;
};
