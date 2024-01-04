// LineBorderSetingItem.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "LineBorderSetingItem.h"
#include "afxdialogex.h"


// LineBorderSetingItem 对话框

IMPLEMENT_DYNAMIC(LineBorderSetingItem, CDialogEx)

LineBorderSetingItem::LineBorderSetingItem(CWnd* pParent /*=NULL*/)
: CDialogEx(LineBorderSetingItem::IDD, pParent), m_title("")
{

}

LineBorderSetingItem::~LineBorderSetingItem()
{
}

void LineBorderSetingItem::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_limitsitemTitle, m_title);
}

void LineBorderSetingItem::setTitles(CString title)
{
	m_title = title;
	if (m_hWnd)
		UpdateData(FALSE);
}


BEGIN_MESSAGE_MAP(LineBorderSetingItem, CDialogEx)
END_MESSAGE_MAP()


// LineBorderSetingItem 消息处理程序
