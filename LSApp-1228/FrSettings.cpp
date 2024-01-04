// FrSettings.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "FrSettings.h"
#include "afxdialogex.h"


// CFrSettings 对话框

IMPLEMENT_DYNAMIC(CFrSettings, CDialogEx)

CFrSettings::CFrSettings(CWnd* pParent /*=NULL*/)
	: CDialogEx(CFrSettings::IDD, pParent)
{

}

CFrSettings::~CFrSettings()
{
}

void CFrSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BOOL CFrSettings::OnInitDialog()
{

	SetBackgroundColor(RGB(248, 248, 255));

	return TRUE;
}


BEGIN_MESSAGE_MAP(CFrSettings, CDialogEx)
END_MESSAGE_MAP()


// CFrSettings 消息处理程序
