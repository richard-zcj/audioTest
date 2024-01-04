#pragma once
#include <vector>

using namespace std;

// CRubZ 对话框

class CRubZ : public CDialogEx
{
	DECLARE_DYNAMIC(CRubZ)

public:
	CRubZ(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CRubZ();

	void setDara(vector<double>* data);

	void updateshow();

// 对话框数据
	enum { IDD = IDD_dlgrub };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	vector<double>* m_data;
};
