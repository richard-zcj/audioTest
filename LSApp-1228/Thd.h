#pragma once
#include <vector>

using namespace std;
// CThd �Ի���

class CThd : public CDialogEx
{
	DECLARE_DYNAMIC(CThd)

public:
	CThd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CThd();

	void setDara(vector<double>* data);

	void updateshow();

// �Ի�������
	enum { IDD = IDD_dlgthd };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	vector<double>* m_data;
};
