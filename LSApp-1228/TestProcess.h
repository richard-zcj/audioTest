#pragma once


// CTestProcess �Ի���

class CTestProcess : public CDialogEx
{
	DECLARE_DYNAMIC(CTestProcess)

public:
	CTestProcess(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTestProcess();
	virtual BOOL OnInitDialog();

	void setProcessMove(int x);

// �Ի�������
	enum { IDD = IDD_DIgTestProcess };

private:
	CRect rc;
	int l;
	int t;
	int r;
	int b;
	int m_rcmove;
	CRect picrc;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnClose();
};
