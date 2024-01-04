// Lmd.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "Lmd.h"
#include "afxdialogex.h"


// CLmd 对话框

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


// CLmd 消息处理程序


HBRUSH CLmd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}
