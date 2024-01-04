// DeviceSet.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "DeviceSet.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream>
#include <string>

// CDeviceSet 对话框

IMPLEMENT_DYNAMIC(CDeviceSet, CDialogEx)

CDeviceSet::CDeviceSet(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDeviceSet::IDD, pParent)
{
	upudatedata();
}

CDeviceSet::~CDeviceSet()
{
}

void CDeviceSet::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDITSEN1, sen1);
	DDX_Text(pDX, IDC_EDITSEN2, sen2);

	DDX_Control(pDX, IDC_EDITSEN1, m_editsen1);
	DDX_Control(pDX, IDC_EDITSEN2, m_editsen2);
}

BOOL CDeviceSet::OnInitDialog()
{
	
	

	return TRUE;
}

void CDeviceSet::upudatedata()
{
	if (this->m_hWnd)
	   UpdateData(FALSE);
	std::ifstream infile;
	CString path = getExcutePath();
	path.Append(_T("\\conf1.ini"));
	infile.open(path, std::ios::in);
	if (!infile.is_open())
	{
		AfxMessageBox(_T("open file err"));
	}


	std::string str;
	getline(infile, str);
	int pos = str.find(':');
	int count = str.length();
	char* temp = new char[count - pos]{0};

	for (size_t i = 0; i < count - pos; i++)
	{
		temp[i] = str[pos + i + 1];
	}
	if (this->m_hWnd)
	  m_editsen1.SetWindowTextW(CString(temp));
	sen1 = atof(temp);
	infile.close();
	delete[] temp;

	path = getExcutePath();
	path.Append(_T("\\conf2.ini"));
	infile.open(path, std::ios::in);
	if (!infile.is_open())
	{
		AfxMessageBox(_T("open file err"));
	}

	str.clear();
	getline(infile, str);
	pos = str.find(':');
	count = str.length();
	temp = new char[count - pos]{0};

	for (size_t i = 0; i < count - pos; i++)
	{
		temp[i] = str[pos + i + 1];
	}
	if (this->m_hWnd)
	   m_editsen2.SetWindowTextW(CString(temp));
	sen2 = atof(temp);
	infile.close();
	delete[] temp;

}

CString CDeviceSet::getExcutePath()
{
	HMODULE module = GetModuleHandle(0);
	wchar_t* pwstr = getLPWSTR(MAX_PATH);
	int pos = GetModuleFileName(module, pwstr, MAX_PATH);

	if (pos < 0)
	{
		AfxMessageBox(_T("get path err"));
		return CString("");
	}
	char* p = getcharFromLPWSTR(pwstr, pos);
	CString path(p);
	delete[] pwstr;
	pos = path.ReverseFind('\\');
	CString leftstr = path.Left(pos);
	delete[] p;

	return leftstr;

}

/*********使用后需要删除wchar_t* ***************/
wchar_t* CDeviceSet::getLPWSTR(int len)
{
	wchar_t *wide = new wchar_t[len];
	len = WideCharToMultiByte(CP_OEMCP, NULL, wide, -1, NULL, 0, NULL, FALSE);
	char* p = new char[len];
	WideCharToMultiByte(CP_OEMCP, NULL, wide, -1, p, len, NULL, FALSE);

	delete[] p;

	return wide;

}
/*********使用后需要删除char* ***************/
char* CDeviceSet::getcharFromLPWSTR(wchar_t* wide, int len)
{
	len = WideCharToMultiByte(CP_OEMCP, NULL, wide, -1, NULL, 0, NULL, FALSE);
	char *pfile = new char[len];
	WideCharToMultiByte(CP_OEMCP, NULL, wide, -1, pfile, len, NULL, FALSE);

	return pfile;

}

double CDeviceSet::getsen1()
{
	if (this->m_hWnd)
	     UpdateData();
	return sen1;
}
double CDeviceSet::getsen2()
{
	if (this->m_hWnd)
	   UpdateData();
	return sen2;
}
void CDeviceSet::setsen1(double sen)
{
	
	sen1 = sen;
	if(this->m_hWnd)
	    UpdateData(FALSE);
}
void CDeviceSet::setsen2(double sen)
{
	
	sen2 = sen;
	if (this->m_hWnd)
	   UpdateData(FALSE);
}



BEGIN_MESSAGE_MAP(CDeviceSet, CDialogEx)
END_MESSAGE_MAP()


// CDeviceSet 消息处理程序
