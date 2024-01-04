#pragma once
#include <vector>

using namespace std;

// CPhase �Ի���

class CPhase : public CDialogEx
{
	DECLARE_DYNAMIC(CPhase)

public:
	CPhase(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CPhase();

	void setDara(vector<double>* data);

	void updateshow();

// �Ի�������
	enum { IDD = IDD_dlgPhase };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();

private:
	vector<double>* m_data;
};
