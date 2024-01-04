// StartPage.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "StartPage.h"
#include "afxdialogex.h"


// CStartPage 对话框

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


// CStartPage 消息处理程序
