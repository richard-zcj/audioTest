// LogIn.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "LogIn.h"
#include "afxdialogex.h"
#include "BlueEreTest.h"




extern CBlueEreTest blueEreTest;
// CLogIn 对话框

IMPLEMENT_DYNAMIC(CLogIn, CDialogEx)

CLogIn::CLogIn(CWnd* pParent /*=NULL*/)
: CDialogEx(CLogIn::IDD, pParent), m_resDialg(false)
{

}

CLogIn::~CLogIn()
{
}

void CLogIn::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPaddress, edtHttpAddr);
	DDX_Control(pDX, IDC_cmbUseName, edtUser);
	DDX_Control(pDX, IDC_editpasswd, edtPwd);
	DDX_Control(pDX, IDC_COMBO2, edtStation);
	DDX_Control(pDX, IDC_COMBO3, edtWorkGroup);
}


BEGIN_MESSAGE_MAP(CLogIn, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CLogIn::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CLogIn::OnBnClickedButton2)
	ON_BN_CLICKED(IDOK, &CLogIn::OnBnClickedButton1)
END_MESSAGE_MAP()


// CLogIn 消息处理程序


void CLogIn::OnBnClickedButton1()
{
	// TODO:  在此添加控件通知处理程序代码


	CString sHost(_T(""));
	CString sUser(_T(""));
	CString sPwd(_T(""));
	CString sStation(_T(""));
	CString sWorkNo(_T(""));
	edtHttpAddr.GetWindowText(sHost);
	edtUser.GetWindowText(sUser);
	edtPwd.GetWindowText(sPwd);
	edtStation.GetWindowText(sStation);
	edtWorkGroup.GetWindowText(sWorkNo);

	if (sHost.IsEmpty())
	{
		AfxMessageBox(_T("IP不能为空"));
		return;
	}

	if (sUser.IsEmpty())
	{
		AfxMessageBox(_T("账号不能为空"));
		return;
	}

	if (sStation.IsEmpty())
	{
		AfxMessageBox(_T("机台编号不能为空"));
		return;
	}

	if (sWorkNo.IsEmpty())
	{
		AfxMessageBox(_T("工序代码不能为空"));
		return;
	}

	m_pHttp = new CHttpClient(sHost, sStation, sWorkNo, m_sSoftVer, false);
	if (m_pHttp == NULL)
	{
		AfxMessageBox(_T("创建HTTP对象错误"));
		return;
	}

	blueEreTest.setHTTPClinet(m_pHttp, sStation);

	if (sUser == _T("离线用户"))
	{

	}
	else
	{


		bool bRet = m_pHttp->MesLogin(sUser, sPwd);
		if (!bRet)
		{

			AfxMessageBox(_T("登录失败..."));
			return;
		}
		m_isOffline = true;
	}
	//AfxMessageBox(_T("登录成功！"));




	m_resDialg = true;
	OnOK();
}


void CLogIn::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	m_resDialg = false;
	OnCancel();
}

bool CLogIn::GetDalgRes()
{
	
	return m_resDialg;
}

BOOL CLogIn::OnInitDialog()
{
	UpdateData(FALSE);

	CString ip=ReadStr(_T("Login"), _T("IP"), _T(""), GetConfigIniFile());
	
	edtHttpAddr.SetWindowTextW(ip);

	CString user = ReadStr(_T("Login"), _T("UserName"), _T(""), GetConfigIniFile());
	
	edtUser.InsertString(0, user);
	edtUser.InsertString(1, _T("离线用户"));
	edtUser.SetCurSel(0);

	CString station = ReadStr(_T("Login"), _T("Station"), _T(""), GetConfigIniFile());
	edtStation.SetWindowTextW(station);


	CString workgroup = ReadStr(_T("Login"), _T("WorkGroup"), _T(""), GetConfigIniFile());

	edtWorkGroup.SetWindowTextW(workgroup);


	return TRUE;
}

bool CLogIn::IsOffLine()
{
	return m_isOffline;
}
