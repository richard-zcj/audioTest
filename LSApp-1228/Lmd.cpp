// Lmd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSApp.h"
#include "Lmd.h"
#include "afxdialogex.h"


// CLmd �Ի���

IMPLEMENT_DYNAMIC(CLmd, CDialogEx)

CLmd::CLmd(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLmd::IDD, pParent)
{

}

CLmd::~CLmd()
{
}

void CLmd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CLmd, CDialogEx)
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()


// CLmd ��Ϣ�������


HBRUSH CLmd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}
