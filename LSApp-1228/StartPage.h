#pragma once

#include "resource.h"
// CStartPage 对话框

class CStartPage : public CDialogEx
{
	DECLARE_DYNAMIC(CStartPage)

public:
	CStartPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStartPage();

// 对话框数据
	enum { IDD = IDD_StartPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
