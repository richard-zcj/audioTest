#pragma once
#include <vector>
#include <set>
#include "MainFrm.h"
using namespace std;
// CDataShow 对话框

class CDataShow : public CDialogEx
{
	DECLARE_DYNAMIC(CDataShow)

public:
	CDataShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDataShow();

	void initPos();

	void setwindowtile(CString title);
	void setchartname(CString charttitle);//含窗口移动代码
	void rotalchartY();
	void setYtitle(CString titleY);
	void setXtitle(CString titleX);

	void setData(vector<double>* data,int size);
	void getData(vector<double>& data,int nSlot);
	void setDataX(vector<double>* data, int size);
	void setDataBorderLine(const vector<double>& dataBorderLine, int size);
	void setDataBorderLinedown(const vector<double>& dataBorderLinedown, int size);
	void setDataBorderLineup(const vector<double>& dataBorderLineup, int size);
	void setYscale(int yscale);
	void setProportional(int dengfenshu);
	void setTestResult(bool isSuccess);
	bool getTestResult();
	void setResultStr(CString str);
	void setColor(COLORREF clo);
	void setOwnerDraw(bool bowner,double min);

	void SetTimerStart();
	void SetTimerStop();

	void setdata1(const vector<double> dat,int nslot);
	void setShowData(vector<double>& data, const vector<double> dat);

	double GetbownerDrwa_min();
	int showDataMax(const vector<double>& data, double& max, bool m_scale = true);
	


// 对话框数据
	enum { IDD = IDD_DlgDataShow };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	


private:
	void drawProportional();
	void drawLog10();
	void drawLog10_muti();

	int getDrawlogX(int XLen, double xdata, double proportional, double xorgval, int XOrg);
	int getDrawBorderLineX(int XLen, double xdata, const set<double>& XPos);

	bool checkDataIsNULL();

	

	void GetShowData(const vector<double> data, vector< int>& tempdata,const double& max);
	


	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnPaint();

private:
	CString m_Ytitle;
	vector<double> m_showTestdata;
	vector<double> m_showTestdata2;
	vector<double> m_showTestdata3;
	vector<double> m_showTestdata4;
	vector<double> m_showTestdataX;
	vector<double> m_showTestdataBorderLine;
	vector<double> m_showTestdataBorderLinedown;
	vector<double> m_showTestdataBorderLineup;
	vector<double> m_showTestborderLine;//测试框线
	set<double> m_showTestdataXPos;




	CString getshowXstr(double xval);
	int yScale;
	int dengfenshu;
	bool m_isSuccess;

	CString result;

	CMainFrame* mainf;

	int marshCount_X;//网格线数量
	int marshCount_Y;
	int dotN;

	int moveYTHDN ;
	int moveXTHDN ;

	int moveYFR ;
	int moveXFR ;

	int moveYTHD ;
	int moveXTHD ;

	int moveYRB ;
	int moveXRB ;

	int moveYPhase;
	int moveXPhase;

	CString title;
	COLORREF m_color;
	CRect rc;
	int xMove = 50;
	int addXlen = 30;
	int orgX, orgY;
	int lenX, lenY;
	int endX, endY;

	int frm_w;
	int frm_h;

	int res_w;
	int res_h;

	int cy_titl;

	CRect chart_rc;
	CRect res_rc;
	CRect x_rc;

	bool m_bownerDrwa;//刻度值与实际绘制数据不至于
	double m_bownerDrwa_min;

	COLORREF oldDlgcolor;
	bool m_truncolor;
	CString m_Xtitle;

	int m_blindTimes;

	COLORREF slot1color;
	COLORREF slot2color;
	COLORREF slot3color;
	COLORREF slot4color;


public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnWindowPosChanged(WINDOWPOS* lpwndpos);
	afx_msg void OnWindowPosChanging(WINDOWPOS* lpwndpos);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
