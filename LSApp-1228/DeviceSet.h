#pragma once
#include "afxwin.h"


// CDeviceSet 对话框

class CDeviceSet : public CDialogEx
{
	DECLARE_DYNAMIC(CDeviceSet)

public:
	CDeviceSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDeviceSet();

	double getsen1();
	double getsen2();
	void setsen1(double sen);
	void setsen2(double sen);

	void upudatedata();

// 对话框数据
	enum { IDD = IDD_DLG_DEVICESET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()


private:
	CString getExcutePath();
	wchar_t* getLPWSTR(int len);
	char* getcharFromLPWSTR(wchar_t* wide, int len);




	
public:
	CEdit m_editsen1;
	CEdit m_editsen2;
	double sen1;
	double sen2;
};
