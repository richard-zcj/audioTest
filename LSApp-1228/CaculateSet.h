#pragma once


// CCaculateSet �Ի���

class CCaculateSet : public CDialogEx
{
	DECLARE_DYNAMIC(CCaculateSet)

public:
	CCaculateSet(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCaculateSet();
	virtual BOOL OnInitDialog();

	int getDotNum();
	void setDotNum(int num);

// �Ի�������
	enum { IDD = IDD_dlgCaculateSet };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()

private:

	int m_dotNum;
	
public:
	afx_msg void OnBnClickedButtonSave();
};
