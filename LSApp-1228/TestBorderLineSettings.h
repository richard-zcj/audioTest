#pragma once
#include "LineBorderSetingItem.h"
#include "afxwin.h"
#include <vector>
#include "afxcmn.h"
using namespace std;
// CTestBorderLineSettings 对话框

class Mutidata
{
public:
	Mutidata(double x1,double y1,double y2);
	~Mutidata();

	void getdata(double& x1, double& y1, double& y2);

private:
	double x;
	double y_down;
	double y_up;

};




class CTestBorderLineSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CTestBorderLineSettings)

public:
	CTestBorderLineSettings(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestBorderLineSettings();
	virtual BOOL OnInitDialog();

	void showItem(int index,CWnd* wnd);
	void DrawThreadBitMap();

	void setData(vector<Mutidata*>& data);
	void setShowData(const vector<Mutidata*>& data);

	void deleData(vector<Mutidata*>& data);

// 对话框数据
	enum { IDD = IDD_DlgBorderLineSettings };

private:

	LineBorderSetingItem* senstivityItem;
	LineBorderSetingItem* FrItem;
	LineBorderSetingItem* ThdItem;
	LineBorderSetingItem* Thd_NItem;
	LineBorderSetingItem* PhaseItem;

	
	

	double m_hz;
	double m_ydown;
	double m_yup;
	double m_ydown1;
	double m_yup1;

	bool  m_singleOrmuti;

	BOOL m_singleup;
	BOOL m_singledown;
	BOOL m_mutiup;
	BOOL m_mutidown;

	int m_rowsList;

	int dengfenshu;


	void sortLineData(vector<Mutidata*>& data);
	void drawGroupBox(CWnd* wnd, CPaintDC* dc);




public:
	vector<Mutidata*> mutidata;//多点数据
	vector<Mutidata*> mutidata2;//单点数据

	//
	vector<Mutidata*> fr_borderLine;
	vector<Mutidata*> thd_borderLine;
	vector<Mutidata*> spk_fr_borderLine;
	vector<Mutidata*> spk_thd_borderLine;
	vector<Mutidata*> mic_fr_borderLine;
	vector<Mutidata*> mic_thd_borderLine;
	vector<Mutidata*> thdN_borderLine;
	vector<Mutidata*> rb_borderLine;
	vector<Mutidata*> Balance_borderLine;
	vector<Mutidata*> NoiseReduction_borderLine;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	CComboBox m_uints;
	CComboBox m_cmbmuti;
	CComboBox m_cmbsingle;
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonmuti();
	afx_msg void OnBnClickedButtonsingle();
	afx_msg void OnBnClickedButtonClear();
	afx_msg void OnBnClickedButtonSaveLineBorederdata();
	afx_msg void OnCbnSelchangelimitscmbtestitemmuti();
	CListCtrl m_listlinedata;
	afx_msg void OnNMDblclkList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnOK();
};




