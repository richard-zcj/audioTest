#pragma once
#include "afxwin.h"
#include "TestASIUO.h"


// TestRME 对话框
void calldraw();

class TestRME : public CDialogEx
{
	DECLARE_DYNAMIC(TestRME)

public:
	TestRME(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~TestRME();

// 对话框数据
	enum { IDD = IDD_DIALOGRME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedPlay();
	afx_msg void OnBnClickedRecord();



	void drawrmeRecordData();

	void playAndRecordThread();

	void setIndex(int num);

	void play(CString filename, int playtime);
	void record(int recordtime);

	bool wdmIsRecordDone();
	void setwdmIsRecordDone(bool b);

	


private:

	int recordtime;
	TestASIUO test;
	CWnd* pictrue;
public:
	CComboBox m_comfilename;
	afx_msg void OnBnClickedButtonStopRecord();
	afx_msg void OnBnClickedButtonShowData();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonPlayAndRecord();
	afx_msg void OnBnClickedButtonStartRME();
	afx_msg void OnBnClickedButtonStopRME();
	afx_msg void OnBnClickedButtonRMEinit();
};
