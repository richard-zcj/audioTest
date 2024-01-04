#pragma once


// CTestProcess 对话框

class CTestProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CTestProcess)

public:
	CTestProcess(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestProcess();
	virtual BOOL OnInitDialog();

	void setProcessMove(int x);

// 对话框数据
	enum { IDD = IDD_DIgTestProcess };

private:
	CRect rc;
	int l;
	int t;
	int r;
	int b;
	int m_rcmove;
	CRect picrc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnClose();
};
