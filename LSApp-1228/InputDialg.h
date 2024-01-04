#pragma once
#include "afxwin.h"


// CInputDialg �Ի���

class CInputDialg : public CDialogEx
{
	DECLARE_DYNAMIC(CInputDialg)

public:
	//CInputDialg(CWnd* pParent = NULL);   // ��׼���캯��
	CInputDialg(bool isStr=false, CWnd* pParent = NULL);
	virtual ~CInputDialg();

	double getV();

	void setCaption(CString cap);

	void setunit(CString unit);
	CString getV2();

	

// �Ի�������
	enum { IDD = IDD_DlgInput };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();




	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);

private:

	double v;
	CString v2;
	CString m_caption;
	CString m_uit;

	bool m_isStr;

	/***********************˽�к���**************************/
	void LoadCmb(bool isinput);

public:
	CComboBox m_cmb;
};
