#pragma once
#include <vector>

using namespace std;

// CThdN 对话框

class CThdN : public CDialogEx
{
	DECLARE_DYNAMIC(CThdN)

public:
	CThdN(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CThdN();

	void setDara(vector<double>* data);

	void updateshow();

// 对话框数据
	enum { IDD = IDD_dlgthd_n };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	vector<double>* m_data;
};
