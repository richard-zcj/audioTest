#pragma once
#include "afxwin.h"
#include <vector>

using namespace std;
// CBlueComm �Ի���

class CBlueComm : public CDialogEx
{
	DECLARE_DYNAMIC(CBlueComm)

public:
	CBlueComm(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlueComm();

// �Ի�������
	enum { IDD = IDD_DlgCom };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnopen();


	//�ⲿ���ú���
	bool opencomm(CString portName);//�򿪴�������
	void connectRre(char* id);//������������
	char* searchEre();//���������豸
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
