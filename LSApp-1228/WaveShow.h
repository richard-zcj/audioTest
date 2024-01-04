#pragma once
#include <vector>

using namespace std;
// CWaveShow 对话框

class CWaveShow : public CDialogEx
{
	DECLARE_DYNAMIC(CWaveShow)

public:
	CWaveShow(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CWaveShow();

	void setData(const vector<double>* data);
	void setData(const double* data,size_t len);

	
	

// 对话框数据
	enum { IDD = IDD_DlgWaveShow };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()



public:
	afx_msg LRESULT OnResItemShow(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();



private:
	void DrawThreadBitMap();
	void LoadTxt(vector<double>* data);

private:
	vector<double> m_data;
	CRect m_rc;//绘图区域
	double maxv ;
};
