#pragma once
#include "afxwin.h"
#include <vector>
#include "MainFrm.h"

using namespace std;
class RBdata
{
public:
	RBdata();
	~RBdata();

	void setdHamnics(int hamnics);
	void setIECorIEEE(bool alogriType);
	void cleardata();
	bool getAlogrithm();
	vector<int>* getHamnics();

private:

	vector<int> m_hamincs;
	bool m_isIECorIEEE;
	

private:

};



// CRbSettings 对话框

class CRbSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CRbSettings)

public:
	CRbSettings(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRbSettings();
	virtual BOOL OnInitDialog();


	RBdata getdata();

// 对话框数据
	enum { IDD = IDD_dlgRBsettings };


private:

	BOOL m_bEndofHamnics;

	//谐波开始
	int hamnicStart;
	int hamnicEnd;

	//radio
	BOOL m_radioHamnicIEC;
	BOOL m_radioHamnicIEEE;
	CMainFrame* mainf;

	/**************私有函数****************/
	void drawGroupBox(CWnd* wnd, CPaintDC* dc);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckendofhamnics();
	CEdit m_bEndofHamnicsRB;
	afx_msg void OnBnClickedButtonSave();

	RBdata rbData;
	afx_msg void OnPaint();
	afx_msg void OnOK();
};
