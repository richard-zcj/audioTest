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



// CRbSettings �Ի���

class CRbSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CRbSettings)

public:
	CRbSettings(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRbSettings();
	virtual BOOL OnInitDialog();


	RBdata getdata();

// �Ի�������
	enum { IDD = IDD_dlgRBsettings };


private:

	BOOL m_bEndofHamnics;

	//г����ʼ
	int hamnicStart;
	int hamnicEnd;

	//radio
	BOOL m_radioHamnicIEC;
	BOOL m_radioHamnicIEEE;
	CMainFrame* mainf;

	/**************˽�к���****************/
	void drawGroupBox(CWnd* wnd, CPaintDC* dc);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheckendofhamnics();
	CEdit m_bEndofHamnicsRB;
	afx_msg void OnBnClickedButtonSave();

	RBdata rbData;
	afx_msg void OnPaint();
	afx_msg void OnOK();
};
