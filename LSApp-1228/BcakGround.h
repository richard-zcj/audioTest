#pragma once
#include "resource.h"
#include "inkpicture1.h"

// CBcakGround �Ի���

class CBcakGround : public CDialogEx
{
	DECLARE_DYNAMIC(CBcakGround)

public:
	CBcakGround(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CBcakGround();
	virtual BOOL OnInitDialog();

// �Ի�������
	enum { IDD = IDD_DlgBcakGround };

private:
	CBitmap m_bg;
	CBrush brush;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CInkpicture1 m_showbg;
};
