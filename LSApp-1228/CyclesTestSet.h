#pragma once
#include "afxwin.h"
#include "resource.h"

class CyclesSetData
{//如果私有变量含有指针，需要重写C++的拷贝构造函数
public:
	CyclesSetData();
	~CyclesSetData();

	void setCyclesTimes(int Times);
	int getCyclesTimes();

	void setCyclesPeriod(int Times);
	int getCyclesPeriod();

	void setCyclesColr(COLORREF clo);
	COLORREF getCyclesColr();

	void setmainTestColr(COLORREF clo);
	COLORREF getmainTestColr();

	void operator = (const CyclesSetData& data);

private:

	int cyclesTimes;
	int cyclesPeriod;
	COLORREF clor;
	COLORREF clor_mainTest;

};
// CCyclesTestSet 对话框

class CCyclesTestSet : public CDialogEx
{
	DECLARE_DYNAMIC(CCyclesTestSet)

public:
	CCyclesTestSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCyclesTestSet();
	virtual BOOL OnInitDialog();

	int getCurnetItemsIndex();

	CyclesSetData getData();
	void setData(CyclesSetData& data);

// 对话框数据
	enum { IDD = IDD_dlgCyclesTestSet };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cyclesItmes;
	afx_msg void OnBnClickedButtonSave();

private:

	int cyclesTimes;
	int cyclesPeriod;

	CyclesSetData data;
public:
	afx_msg void OnBnClickedButtonSetColor();
	afx_msg void OnBnClickedButtonMainTestFrmColorSet();
};
