#pragma once
#include "afxwin.h"
#include <vector>
#include "MyListBox.h"

#define RESITEMSSHOW WM_USER+1000


// CTestResManagement 对话框



class CTestResManagement : public CDialogEx
{
	DECLARE_DYNAMIC(CTestResManagement)

public:
	CTestResManagement(CWnd* pParent = NULL);   // 标准构造函数
	CTestResManagement(int id,CWnd* pParent = NULL); 
	virtual ~CTestResManagement();
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	

	void setItem(const std::vector<CString>& dataItem);
	void clearItem();
	void getShowItems(std::vector<int>& dataItem);
	void setHwnd(HWND wnd, HWND wnd2);
	void setWindowText(CString title);
	int getSlotFrameID(CString name);
	void setRes(bool* res);



// 对话框数据
	enum { IDD = IDD_DlgTestResManage };

private:

	std::vector<int> m_showItems;
	HWND m_hwnd, m_hwnd2;
	CRect rc;
	CToolTipCtrl m_tip;

	int m_id;

	bool* m_res;
	bool bpass;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	MyListBox m_list;
	afx_msg void OnLbnSetfocusList1();
	afx_msg void OnLbnSelchangeList1testres();
	afx_msg void OnLbnDblclkList1testres();
	afx_msg void OnNMThemeChangedList1testres(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
};
