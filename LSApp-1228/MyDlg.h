#pragma once
#include "resource.h"
#include "afxwin.h"


// MyDlg �Ի���

class MyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MyDlg)

public:
	MyDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MyDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

	CComboBox m_cmbInput;
	CComboBox m_inputInside;


	bool SaveData();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeCmbInputinside();
	afx_msg void OnCbnSelchangeCmbInput();
	CComboBox m_cmbYinxiang;
	afx_msg void OnClose();
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	CComboBox m_cmbyinxiang2;
};
