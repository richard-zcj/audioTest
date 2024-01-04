#pragma once

#include "resource.h"
// CFrSettings 对话框

class CFrSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CFrSettings)

public:
	CFrSettings(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFrSettings();

// 对话框数据
	enum { IDD = IDD_dlgFrsettings };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
