#pragma once
#include "afxwin.h"
//#include "daqusb402x.h"
#include <vector>


// CBlueTestD �Ի���

class CBlueTestD : public CDialogEx
{
	DECLARE_DYNAMIC(CBlueTestD)

public:
	CBlueTestD(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBlueTestD();

// �Ի�������
	enum { IDD = IDD_DlgBlueTest };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	CComboBox m_cmbtestitem;
	afx_msg void OnBnClickedButtonTest();


private:
//	daqusb402x *daqdevice;
	int thd();
	int thdAndN();
	//void adc_config_channel(daqusb402x* daqdevice);
	//int adc_n_sample_example(daqusb402x* daqdevice);//thd
	//int adc_n_sample_ThdN(daqusb402x* daqdevice);//thd+n

	int xinzaobi();

	int dizao();

	int gelidu();//�����Ÿ����


private:

	int f;
	int fs;
	int m_count;
	int testfs;

	vector<double> m_data;
	vector<double> m_data2;
	int xscale;
	int yscale;

	std::vector<double>* data;              //�ź����ݼ���

	CRect drawrect;
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonFR();
};
