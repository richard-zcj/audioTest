#pragma once
#include "afxwin.h"
#include "resource.h"

class CyclesSetData
{//���˽�б�������ָ�룬��Ҫ��дC++�Ŀ������캯��
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
// CCyclesTestSet �Ի���

class CCyclesTestSet : public CDialogEx
{
	DECLARE_DYNAMIC(CCyclesTestSet)

public:
	CCyclesTestSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCyclesTestSet();
	virtual BOOL OnInitDialog();

	int getCurnetItemsIndex();

	CyclesSetData getData();
	void setData(CyclesSetData& data);

// �Ի�������
	enum { IDD = IDD_dlgCyclesTestSet };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
