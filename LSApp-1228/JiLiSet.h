#pragma once

#include "DrawWaveformBitmap.h"
#include "afxwin.h"
#include <vector>
#define ID_JILISET WM_USER+1

#define EXCITATION "jiliset"

// CJiLiSet 对话框

class CJiLiSet : public CDialogEx
{
	DECLARE_DYNAMIC(CJiLiSet)

public:
	CJiLiSet(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CJiLiSet();

// 对话框数据
	enum { IDD = IDD_DLG_JILISET };

	vector<double> data;              //信号数据集合
	vector<double> data2;              //信号数据集合,


	double getDb(double p);
	double getPa(double db);

	void setvoltage(double v);

	bool isdrawrun;
	std::map<int, int> pos;

	

private:

	
	double* x_point;
	int pointSize;
	double* outdata = NULL;
	double* outdata2 = NULL;
	

	int fs;
	double voltage;                   //电压
	int min_period;                    //最小周期
	int min_time;                    //最短时间

	double scale;
	double scaleY;
	double scaleY1;

	bool OpenDrawThread();
	void DrawThread();
	void DrawThreadBitMap();
	void updatePindian();

	bool bExit = false;
	int start_frequency;               //开始频率
	int stop_frequency;                //结束频率
	int num_dot;//频点数
	bool b_fanfda = false;
	std::vector<int> pindian;
	

	CJiLiSet* m_this=NULL;

	BOOL bfenli;

	
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	void AFXAPI DDV_MinMaxFloat(CDataExchange* pDX, float const& value, float minVal, float maxVal);

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg LRESULT DrawJiLiSet(WPARAM wParam,LPARAM lParam);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButtonFangDa();
	afx_msg void OnBnClickedButtonSuoXiao();
	afx_msg void OnStnClickedJiliset();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	//CEdit editV;
	afx_msg void OnEnUpdateEditv();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnClose();
	CStatic m_title;
	afx_msg void OnBnClickedButtonYfangda();
};
