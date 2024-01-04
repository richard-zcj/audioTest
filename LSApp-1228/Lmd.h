#pragma once


// CLmd 对话框

class CLmd : public CDialogEx
{
	DECLARE_DYNAMIC(CLmd)

public:
	CLmd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLmd();

// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
