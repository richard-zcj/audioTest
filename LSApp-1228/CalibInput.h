#pragma once
#include "afxcmn.h"
#include "ViewTree.h"
#include <vector>
#include "CallBackBase.h"
#include "USB4431.h"
#include "afxwin.h"
using namespace std;


// CCalibInput 对话框
//extern CUSB4431 * pDevUSB4431;
class CCalibInput : public CDialogEx, public CCallBackBase
{
	DECLARE_DYNAMIC(CCalibInput)

public:
	CCalibInput(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CCalibInput();
	virtual BOOL OnInitDialog();

	double getInDeviceSensitivity();

	void cal_Vp(int fs, CString decname,double& vp,int playchs,int recordchs,double outvp);
	double cal_sensitivity(CString decname,double vp, double fs);
	

// 对话框数据
	enum { IDD = IDD_calibrationinput };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	void RecieveUSB4431Data(double * readArrary, UINT32 dataLength);

	DECLARE_MESSAGE_MAP()
public:
	CTreeCtrl m_tree;
	CViewTree m_treeview;
	afx_msg void OnBnClickedButtonStartCalibration();

private:
	int maxdeviceid;
	HTREEITEM hroot, hroot2;
	HTREEITEM hItemdel;
	HTREEITEM hItemdel2;

	vector<HTREEITEM> deldat;

	CString devicename;

	double inputval;

	double calibrfs;
	double invrms;
	double inVP;
public:
	afx_msg void OnBnClickedButtonDelInstru();
	afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusTree1(NMHDR *pNMHDR, LRESULT *pResult);

private:

	bool isdupname(CString addname);
	void getmaxdeviceId();
	bool iskeysheetf;
	char* getcharp(CString str);
	
	template<typename T1>
	double getAp(T1* data, int len, int nfs = 44100, double mubiaofs = 1000);//为了兼容NI采集卡引入类型参数化
	double getApzcj(int* data, int len, int nfs = 44100, double mubiaofs = 1000);
	double getApzcjdouble(double* data, int len, int nfs = 44100, double mubiaofs = 1000);

	//void NIDaqStartAi();
	void drawGroupBox(CWnd* wnd, CPaintDC* dc);

	
public:
	afx_msg void OnTvnKeydownTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButtonVpCalibration();
	CComboBox m_cmbinnerDevices;
//	afx_msg void OnCbnDropdowncalibrainputdevices();
	afx_msg void OnEnChangeeditcalbraininputval();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonUpdateVp();
	afx_msg void OnCbnDropdowncalibrainputdevices();
	afx_msg void OnOK();
};
