#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include <vector>
#include "MainFrm.h"

using namespace std;
class ThdData
{
public:
	ThdData();
	~ThdData();
	void operator=(const ThdData& data);

	void setdHamnics(int hamnics);
	void setIECorIEEE(bool alogriType);
	void cleardata();

	vector<int>* getHamnics() ;
	bool getAlogrithm() ;

private:

	vector<int> m_hamincs;

	bool m_isIECorIEEE;

	

};



// CThdSettings 对话框

class CThdSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CThdSettings)

public:
	CThdSettings(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CThdSettings();
	virtual BOOL OnInitDialog();

	void setMincycles();

	ThdData getdata();

// 对话框数据
	enum { IDD = IDD_dlgTHDsettings };

private:

	BOOL m_bEndofHamnics;

	//16个check
	BOOL m_checkHamnic1;
	BOOL m_checkHamnic2;
	BOOL m_checkHamnic3;
	BOOL m_checkHamnic4;
	BOOL m_checkHamnic5;
	BOOL m_checkHamnic6;
	BOOL m_checkHamnic7;
	BOOL m_checkHamnic8;
	BOOL m_checkHamnic9;
	BOOL m_checkHamnic10;
	BOOL m_checkHamnic11;
	BOOL m_checkHamnic12;
	BOOL m_checkHamnic13;
	BOOL m_checkHamnic14;
	BOOL m_checkHamnic15;
	BOOL m_checkHamnic16;

	//radio
	BOOL m_radioHamnicIEC;
	BOOL m_radioHamnicIEEE;


	//谐波开始
	int hamnicStart;
	int hamnicEnd;


	//主窗口类指针
	CMainFrame* mainf;


	/****************私有函数**************************/
	void drawGroupBox(CWnd* wnd, CPaintDC* dc);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck17();
	CEdit m_btnEndofHamics;
	CSpinButtonCtrl m_alogriTHDSetMinCycles;
	CEdit m_startxiebo;

	ThdData thdData;
	afx_msg void OnBnClickedButtonSaveAlogrithmThdData();
	afx_msg void OnPaint();
	afx_msg void OnOK();
};
