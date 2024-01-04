// CardDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "CardDlg.h"
#include "afxdialogex.h"


// CCardDlg 对话框

IMPLEMENT_DYNAMIC(CCardDlg, CDialogEx)

CCardDlg::CCardDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCardDlg::IDD, pParent)
{

}

CCardDlg::~CCardDlg()
{
}

void CCardDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CCardDlg, CDialogEx)
END_MESSAGE_MAP()


// CCardDlg 消息处理程序
