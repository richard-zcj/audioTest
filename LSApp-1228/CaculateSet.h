#pragma once


// CCaculateSet 对话框

class CCaculateSet : public CDialogEx
{
	DECLARE_DYNAMIC(CCaculateSet)

public:
	CCaculateSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCaculateSet();
	virtual BOOL OnInitDialog();

	int getDotNum();
	void setDotNum(int num);

// 对话框数据
	enum { IDD = IDD_dlgCaculateSet };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

private:

	int m_dotNum;
	
public:
	afx_msg void OnBnClickedButtonSave();
};
