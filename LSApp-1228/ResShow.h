#pragma once


// CResShow 对话框

class CResShow : public CDialogEx
{
	DECLARE_DYNAMIC(CResShow)

public:
	CResShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CResShow();

	void setitemName(CString s);
	void setuint(CString s);
	void setval(double value);
	void setval(CString value);
	void setTitle(CString title);

	void IsSucess(bool bsucess);


// 对话框数据
	enum { IDD = IDD_DlgResShow };


private:

	CString m_itemName;
	CString m_uint;
	double m_val;
	bool m_truncolor;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOK();
};
