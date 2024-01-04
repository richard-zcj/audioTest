// GetInput.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "GetInput.h"
#include "afxdialogex.h"


// CGetInput 对话框

IMPLEMENT_DYNAMIC(CGetInput, CDialogEx)

CGetInput::CGetInput(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGetInput::IDD, pParent)
{

}

CGetInput::~CGetInput()
{
}

void CGetInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGetInput, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGetInput::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CGetInput::OnBnClickedCancel)
END_MESSAGE_MAP()


// CGetInput 消息处理程序


void CGetInput::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码

	GetDlgItem(IDC_EDITgetinput)->GetWindowTextW(m_jiliname);

	CDialogEx::OnOK();
}


void CGetInput::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码



	CDialogEx::OnCancel();
}

void CGetInput::getjiliName(CString& name)
{
	name = m_jiliname;
}