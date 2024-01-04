#pragma once
#include "afxcmn.h"
#include "CalibInput.h"
#include "CalibrOutput.h"


// CCalibration 对话框

class CCalibration : public CDialogEx
{
	DECLARE_DYNAMIC(CCalibration)

public:
	CCalibration(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalibration();
	virtual BOOL OnInitDialog();


	double getinputSens();

// 对话框数据
	enum { IDD = IDD_dlgcalibration };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CTabCtrl m_tab;


	CCalibInput calibrinput;
	CCalibrOutput calibroutput;
public:
	afx_msg void OnTcnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTcnKeydownTab1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnPaint();
};
