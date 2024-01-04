#pragma once
#include <vector>

using namespace std;
// CThd 对话框

class CThd : public CDialogEx
{
	DECLARE_DYNAMIC(CThd)

public:
	CThd(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CThd();

	void setDara(vector<double>* data);

	void updateshow();

// 对话框数据
	enum { IDD = IDD_dlgthd };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	vector<double>* m_data;
};
