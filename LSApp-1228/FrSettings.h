#pragma once

#include "resource.h"
// CFrSettings �Ի���

class CFrSettings : public CDialogEx
{
	DECLARE_DYNAMIC(CFrSettings)

public:
	CFrSettings(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CFrSettings();

// �Ի�������
	enum { IDD = IDD_dlgFrsettings };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
};
