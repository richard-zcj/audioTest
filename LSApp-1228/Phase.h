#pragma once
#include <vector>

using namespace std;

// CPhase 对话框

class CPhase : public CDialogEx
{
	DECLARE_DYNAMIC(CPhase)

public:
	CPhase(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPhase();

	void setDara(vector<double>* data);

	void updateshow();

// 对话框数据
	enum { IDD = IDD_dlgPhase };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	vector<double>* m_data;
};
