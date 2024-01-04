#pragma once


// CCardDlg 对话框

class CCardDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCardDlg)

public:
	CCardDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCardDlg();

// 对话框数据
	enum { IDD = IDD_DLG_CARDSETING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
