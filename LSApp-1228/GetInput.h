#pragma once
#include "afxwin.h"


// CGetInput �Ի���

class CGetInput : public CDialogEx
{
	DECLARE_DYNAMIC(CGetInput)

public:
	CGetInput(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGetInput();

	void getjiliName(CString& name);

// �Ի�������
	enum { IDD = IDD_DlgGetInput };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

private:

	CString m_jiliname;
public:
	CComboBox m_cmb;
};
