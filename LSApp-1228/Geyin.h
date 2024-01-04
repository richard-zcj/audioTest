#pragma once

#include <vector>
#include "RuiSenCard.h"
#include "afxwin.h"





// CGeyin �Ի���

class CGeyin : public CDialogEx
{
	DECLARE_DYNAMIC(CGeyin)

public:
	CGeyin(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGeyin();





// �Ի�������
	enum { IDD = IDD_DLG_GEYIN };
	std::vector<double>* data;              //�ź����ݼ���
	std::vector<double>* data2;              //�ź����ݼ���,
	CRuiSenCard card;

private:

	CDC*dc;
	CWnd* pictrue;
	CBrush mBrush;
	bool bExit = false;
	bool OpenDrawThread();
	void DrawThreadBitMap();
	void drawTestkx(CPaintDC dc);

	void OpenDrawThreadDrawRes();
	void DrawThreadRes();

	void recoradthread();

	bool jiaozhunMic(double& sen);

	double getAp(int* data,int len );

	double* caculateRes(double* data, int len);

	//out �ⲿmic   out2�ڲ�mic�ɼ��ź�
	void play(std::vector<double>* data, double*out, double*out2);

	char* getcharp(CString str);

	CString getExcutePath();
	wchar_t* getLPWSTR(int len);
	char* getcharFromLPWSTR(wchar_t* wide, int len);

	void threadjiaozhuan();
	void threadstopCard();

	std::vector<double> dataRes;
	
	RecordPartmeter m_RecordParameter;

	double m_fs;//У׼����Ƶ��

	double bili = 1000000000.0;

	double m_sen1;
	double m_sen2;

	bool isstopjiaozhun;
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonTest();
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonUpdate();
	void OnDraw();
	afx_msg void OnBnClickedButton6();
	CStatic m_playing;


	afx_msg void OnBnClickedButtonMic1();
	afx_msg void OnBnClickedButtonMic2();
	afx_msg void OnBnClickedButtonShengYaJiaoZhun();
	CComboBox m_cmbpinlv;
	CComboBox m_cmbshengya;
	CEdit m_mubiaovalue;
	CEdit m_initvalote;
	CButton m_btnMic1;
	CButton m_btnMic2;
	CButton m_btnDb;
	CButton m_btnTest;
	CEdit m_editValote;
	afx_msg void OnBnClickedBtnstopjiaozhun();
};
