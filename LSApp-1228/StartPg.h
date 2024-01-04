#pragma once


// CStartPg 对话框

class CStartPg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartPg)

public:
	CStartPg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStartPg();
	virtual BOOL OnInitDialog();

	void SetStartInfo(CString info);

// 对话框数据
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


private:
	CBrush brush;

	CString m_shwstr;
};
