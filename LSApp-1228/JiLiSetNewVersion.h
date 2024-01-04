#pragma once
#include <thread>
#include <vector>
#include <map>
#include "afxwin.h"
using namespace std;
// JiLiSetNewVersion 对话框

class JiLiSetNewVersion : public CDialogEx
{
	DECLARE_DYNAMIC(JiLiSetNewVersion)

public:
	JiLiSetNewVersion(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~JiLiSetNewVersion();


	double getPlayTimes();


	void DrawThreadBitMap();

	void drawWave(CDC* dc);

	void setVoltage(double v);
	double getVoltage();
	vector<CString> getJiliname();

	void GenerateV(double v,double f);
	vector<double>* getsenofSpk(int index, const vector<double>& testFs);
	vector<double>* getsenofMic(int index, const vector<double>& testFs);
	void LoadCsv(vector<double>* data, CString path1);
	void LoadSenoFSpk();//mic模式
	void LoadSenoFMic();//spk模式
	vector<double>* GetOffsetFs(const vector<double> data, int nslot, bool bSpk = true);
// 对话框数据
	enum { IDD = IDD_jilisetnew };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnDelJili();
	afx_msg void OnshowJili();
	afx_msg void OnplayJili();

private:

	void getpindian(int pos, vector<double>& pindiandata);

	double findpos(double inval, int& pos);
	void getsteppindian(int startstep, int step, int stoppos, vector<double>& stepdata);
	void drawGroupBox(CWnd* wnd, CPaintDC* dc);
	void LoadTxt(vector<double>* data, CString path1);
	

	void onDbClicklist();
	void onRClicklist();

	void precudeplayWav();

	

private:
	
	
	

	double voltage;
	double startfs;
	double stopfs;
	double fs;
	double vp;
	BOOL m_eq;
	CString m_eqpath;
	double maxeq;

	int cycle;
	int mintime;

	double playtime;

	CRect m_rc;//绘图区域
	vector<double> m_listEQ;

	map<CString,int*> mapData;

	//mic模式
	vector<double> m_senofSpk;
	vector<double> m_senofSpk2;
	vector<double> m_senofSpk3;
	vector<double> m_senofSpk4;

	//spk模式
	vector<double> m_senofMic;
	vector<double> m_senofMic2;
	vector<double> m_senofMic3;
	vector<double> m_senofMic4;

	//offset_fs
	vector<double> offsetFs_spk;//spk
	vector<double> offsetFs_mic;//mic
	
	bool m_bInitFinish;

	void GetEQValues(stimParam* pStim, map<double, double>& mEQValue);
	void loadMicOffsetval(string line, int i);
	void loadSpkOffsetval(string line, int i);
	void loadOffsetFs(string line, vector<double>& vFsList);

public:
	map<double, int> pos;
	vector<int> data;//这里的数据类型是否对声卡的输出由影响
	vector<double> pindians; //初始化的频点集合
	vector<double> inuse_pindians; //初始化的频点集合

public:
	afx_msg void OnBnClickedButtonUpdate();
	afx_msg void OnBnClickedButtonSave();
	afx_msg void OnBnClickedButtonDeljili();
	afx_msg void OnOK();
	afx_msg void OnBnClickedButtonImportEq();
	CListBox m_listJili;
	CComboBox m_cmbPlayChs;
	afx_msg void OnBnClickedButtonplayJili();
	afx_msg void OnSelchangeList2();
	afx_msg void OnChangeEditjilisetnewv();
	afx_msg void OnChangeEditjilisetnewstart();
	afx_msg void OnChangeEditjilisetnewstop();
	afx_msg void OnCbnSelchangeCombfs();
	afx_msg void OnCbnSelchangeCombbeipin();
	afx_msg void OnCbnSelchangeCombjilitype();
	afx_msg void OnEnChangejilinewcycle();
	afx_msg void OnEnChangejilinewmintime();
	afx_msg void OnBnClickedjilisetcheckeq();
	afx_msg void OnEnChangejilieqpath();
	afx_msg void OnBnClickedButton_CalSPK();
	afx_msg void SpkCalibration();
};
