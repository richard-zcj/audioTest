#pragma once


// CLmd �Ի���

class CLmd : public CDialogEx
{
	DECLARE_DYNAMIC(CLmd)

public:
	CLmd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CLmd();

// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
