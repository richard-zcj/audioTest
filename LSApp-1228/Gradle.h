#pragma once


// CGradle 对话框

class CGradle : public CDialogEx
{
	DECLARE_DYNAMIC(CGradle)

public:
	CGradle(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CGradle();

// 对话框数据
	enum { IDD = IDD_DlgGradle };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
