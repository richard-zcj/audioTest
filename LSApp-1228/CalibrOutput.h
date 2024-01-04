#pragma once
#include "afxcmn.h"
#include <vector>
#include "CallBackBase.h"
#include "USB4431.h"
#include "afxwin.h"
using namespace std;


// CCalibrOutput 对话框

class CCalibrOutput : public CDialogEx, public CCallBackBase
{
	DECLARE_DYNAMIC(CCalibrOutput)

public:
	CCalibrOutput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalibrOutput();

	void cal_Vp(CString innerDeviceName,double& vpout);

private:
	int maxdeviceid;
	HTREEITEM hroot, hroot2;
	HTREEITEM hItemdel;
	HTREEITEM hItemdel2;
	

// 对话框数据
	enum { IDD = IDD_calibrationoutput };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonStartCalibration();
	CTreeCtrl m_tree;

private:

	bool isdupname(CString addname);
	void getmaxdeviceId();
	void drawGroupBox(CWnd* wnd, CPaintDC* dc);
	bool iskeysheetf;

	vector<HTREEITEM> deldat;

	CString devicename;

	

	double vpp;
	double v;
	double vp;
	double ap;

	double m_involtage;
	double m_outvoltage;

	double m_involtageAP;
	double m_outvoltageAP;

public:
	afx_msg void OnBnClickedButtonDelInstru();
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnKeydownTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonstopNIplay();
	afx_msg void OnBnClickedButtonUpdateoutCHsVps();
	afx_msg void OnBnClickedButtonUpdateAP();
	afx_msg void OnCbnDropdowncalibraoutputdevices();
	CComboBox m_cmboutDevices;
//	afx_msg void OnPaint();
	afx_msg void OnPaint();
	afx_msg void OnCbnDropdownCombo1();
	CComboBox m_chsOutDevice;
	afx_msg void OnOK();
};
