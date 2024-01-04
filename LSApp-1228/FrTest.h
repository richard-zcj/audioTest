#pragma once
#include <vector>

using namespace std;
// CFrTest 对话框

class CFrTest : public CDialogEx
{
	DECLARE_DYNAMIC(CFrTest)

public:
	CFrTest(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFrTest();

	void setDara(vector<double>* data);

	void updateshow();

// 对话框数据
	enum { IDD = IDD_dlgFr };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	vector<double>* m_data;
};
