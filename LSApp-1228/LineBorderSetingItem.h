#pragma once


// LineBorderSetingItem 对话框

class LineBorderSetingItem : public CDialogEx
{
	DECLARE_DYNAMIC(LineBorderSetingItem)

public:
	LineBorderSetingItem(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~LineBorderSetingItem();


	void setTitles(CString title);

// 对话框数据
	enum { IDD = IDD_limitsItem };

private:

	CString m_title;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
