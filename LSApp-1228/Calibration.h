#pragma once
#include "afxcmn.h"
#include "CalibInput.h"
#include "CalibrOutput.h"


// CCalibration �Ի���

class CCalibration : public CDialogEx
{
	DECLARE_DYNAMIC(CCalibration)

public:
	CCalibration(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCalibration();
	virtual BOOL OnInitDialog();


	double getinputSens();

// �Ի�������
	enum { IDD = IDD_dlgcalibration };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
