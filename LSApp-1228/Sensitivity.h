#pragma once
#include <vector>

using namespace std;

// CSensitivity 对话框

class CSensitivity : public CDialogEx
{
	DECLARE_DYNAMIC(CSensitivity)

public:
	CSensitivity(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CSensitivity();

	void setDara(vector<double>* data);

	void updateshow();

// 对话框数据
	enum { IDD = IDD_dlgSensitivity };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	vector<double>* m_data;
};
