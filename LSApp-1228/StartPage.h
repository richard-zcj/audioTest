#pragma once

#include "resource.h"
// CStartPage �Ի���

class CStartPage : public CDialogEx
{
	DECLARE_DYNAMIC(CStartPage)

public:
	CStartPage(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CStartPage();

// �Ի�������
	enum { IDD = IDD_StartPage };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
