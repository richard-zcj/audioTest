// Gradle.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSApp.h"
#include "Gradle.h"
#include "afxdialogex.h"


// CGradle �Ի���

IMPLEMENT_DYNAMIC(CGradle, CDialogEx)

CGradle::CGradle(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGradle::IDD, pParent)
{

}

CGradle::~CGradle()
{
}

void CGradle::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CGradle, CDialogEx)
END_MESSAGE_MAP()


// CGradle ��Ϣ�������
