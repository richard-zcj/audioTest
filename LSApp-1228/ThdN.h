#pragma once
#include <vector>

using namespace std;

// CThdN �Ի���

class CThdN : public CDialogEx
{
	DECLARE_DYNAMIC(CThdN)

public:
	CThdN(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CThdN();

	void setDara(vector<double>* data);

	void updateshow();

// �Ի�������
	enum { IDD = IDD_dlgthd_n };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	vector<double>* m_data;
};
