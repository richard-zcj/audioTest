#pragma once
#include "afxwin.h"
#include <vector>

using namespace std;
// CBlueComm 对话框

class CBlueComm : public CDialogEx
{
	DECLARE_DYNAMIC(CBlueComm)

public:
	CBlueComm(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CBlueComm();

// 对话框数据
	enum { IDD = IDD_DlgCom };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnopen();


	//外部调用函数
	bool opencomm(CString portName);//打开串口连接
	void connectRre(char* id);//连接蓝牙耳机
	char* searchEre();//搜索蓝牙设备
	void disconnect();
	void switchmode(CString btnstr);
	void getDeviceMode(CString& str);
	void setMode(CString s);
	void clearcms();

	void setIndex(int num);
	int index;


private:
	/*HANDLE hcom;*/

	CBrush brush;
	CString m_mode;
	
	vector<HANDLE> list_coms;
	
	void getDeviceStatus();
	

public:
	afx_msg void OnBnClickedBtnclose();
	CListBox mlistBox;
	afx_msg void OnBnClickedButtonDisconn();
	afx_msg void OnBnClickedButtonConn();
	afx_msg void OnBnClickedButtonSerchBlue();
	afx_msg void OnBnClickedButtonStopSerch();
	afx_msg void OnBnClickedButtonClearScreen();
	CComboBox m_port;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonSetVol();
	afx_msg void OnBnClickedButtonModeSwatch();
	afx_msg void OnBnClickedButtonDeviceStatus();
	afx_msg void OnBnClickedBtnsend();
};
