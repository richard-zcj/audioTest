#pragma once
#include <vector>

using namespace std;

// CRubZ �Ի���

class CRubZ : public CDialogEx
{
	DECLARE_DYNAMIC(CRubZ)

public:
	CRubZ(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRubZ();

	void setDara(vector<double>* data);

	void updateshow();

// �Ի�������
	enum { IDD = IDD_dlgrub };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
private:
	vector<double>* m_data;
};
