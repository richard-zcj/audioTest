#pragma once
#include "afxwin.h"


// CGetInput 对话框

class CGetInput : public CDialogEx
{
	DECLARE_DYNAMIC(CGetInput)

public:
	CGetInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGetInput();

	void getjiliName(CString& name);

// 对话框数据
	enum { IDD = IDD_DlgGetInput };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();

private:

	CString m_jiliname;
public:
	CComboBox m_cmb;
};
