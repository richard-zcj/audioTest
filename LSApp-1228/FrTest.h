#pragma once
#include <vector>

using namespace std;
// CFrTest �Ի���

class CFrTest : public CDialogEx
{
	DECLARE_DYNAMIC(CFrTest)

public:
	CFrTest(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFrTest();

	void setDara(vector<double>* data);

	void updateshow();

// �Ի�������
	enum { IDD = IDD_dlgFr };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	vector<double>* m_data;
};
