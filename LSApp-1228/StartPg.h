#pragma once


// CStartPg �Ի���

class CStartPg : public CDialogEx
{
	DECLARE_DYNAMIC(CStartPg)

public:
	CStartPg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStartPg();
	virtual BOOL OnInitDialog();

	void SetStartInfo(CString info);

// �Ի�������
	enum { IDD = IDD_DIALOG4 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


private:
	CBrush brush;

	CString m_shwstr;
};
