#pragma once
#include <vector>

using namespace std;
// CWaveShow �Ի���

class CWaveShow : public CDialogEx
{
	DECLARE_DYNAMIC(CWaveShow)

public:
	CWaveShow(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CWaveShow();

	void setData(const vector<double>* data);
	void setData(const double* data,size_t len);

	
	

// �Ի�������
	enum { IDD = IDD_DlgWaveShow };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()



public:
	afx_msg LRESULT OnResItemShow(WPARAM wParam, LPARAM lParam);
	afx_msg void OnPaint();



private:
	void DrawThreadBitMap();
	void LoadTxt(vector<double>* data);

private:
	vector<double> m_data;
	CRect m_rc;//��ͼ����
	double maxv ;
};
