#pragma once


// CCardDlg �Ի���

class CCardDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CCardDlg)

public:
	CCardDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CCardDlg();

// �Ի�������
	enum { IDD = IDD_DLG_CARDSETING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
