#pragma once
#include <vector>

using namespace std;

// CSensitivity �Ի���

class CSensitivity : public CDialogEx
{
	DECLARE_DYNAMIC(CSensitivity)

public:
	CSensitivity(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CSensitivity();

	void setDara(vector<double>* data);

	void updateshow();

// �Ի�������
	enum { IDD = IDD_dlgSensitivity };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	vector<double>* m_data;
};
