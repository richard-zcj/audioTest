#pragma once


// CGradle �Ի���

class CGradle : public CDialogEx
{
	DECLARE_DYNAMIC(CGradle)

public:
	CGradle(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CGradle();

// �Ի�������
	enum { IDD = IDD_DlgGradle };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
