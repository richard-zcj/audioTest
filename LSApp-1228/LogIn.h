#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "HttpClient.h"


// CLogIn 对话框

class CLogIn : public CDialogEx
{
	DECLARE_DYNAMIC(CLogIn)

public:
	CLogIn(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLogIn();
	virtual BOOL OnInitDialog();


	bool GetDalgRes();
	bool IsOffLine();

// 对话框数据
	enum { IDD = IDD_DlgLogin };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();


private:

	bool m_resDialg;
	bool m_isOffline;

	CHttpClient* m_pHttp;
	CString m_sSoftVer;
public:
	CNetAddressCtrl edtHttpAddr;
	CComboBox edtUser;
	CEdit edtPwd;
	CComboBox edtStation;
	CComboBox edtWorkGroup;
};
