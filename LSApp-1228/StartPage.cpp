// StartPage.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSApp.h"
#include "StartPage.h"
#include "afxdialogex.h"


// CStartPage �Ի���

IMPLEMENT_DYNAMIC(CStartPage, CDialogEx)

CStartPage::CStartPage(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStartPage::IDD, pParent)
{

}

CStartPage::~CStartPage()
{
}

void CStartPage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CStartPage::OnInitDialog()
{

	return TRUE;
}


BEGIN_MESSAGE_MAP(CStartPage, CDialogEx)
END_MESSAGE_MAP()


// CStartPage ��Ϣ�������
