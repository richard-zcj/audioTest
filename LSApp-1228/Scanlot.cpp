// Scanlot.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "Scanlot.h"
#include "afxdialogex.h"
#include <Imm.h>

#pragma comment(lib, "imm32.lib")


// CScanlot 对话框

IMPLEMENT_DYNAMIC(CScanlot, CDialogEx)

CScanlot::CScanlot(CWnd* pParent /*=NULL*/)
: CDialogEx(CScanlot::IDD, pParent), m_dialogRes(false)
, m_Retest(FALSE)
{

}

CScanlot::~CScanlot()
{
}

void CScanlot::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Text(pDX, IDC_EDIT1, lot1);
	DDX_Text(pDX, IDC_EDIT2, lot2);
	DDX_Text(pDX, IDC_EDIT3, lot3);
	DDX_Text(pDX, IDC_EDIT6, lot4);

	DDX_Check(pDX, IDC_CHECK1, m_lot1_ck);
	DDX_Check(pDX, IDC_CHECK2, m_lot2_ck);
	DDX_Check(pDX, IDC_CHECK3, m_lot3_ck);
	DDX_Check(pDX, IDC_CHECK4, m_lot4_ck);

	DDX_Check(pDX, IDC_CHECK5, m_Retest);
}


BEGIN_MESSAGE_MAP(CScanlot, CDialogEx)
	ON_BN_CLICKED(IDCANCEL, &CScanlot::OnBnClickedCancel)
	ON_BN_CLICKED(IDOK, &CScanlot::OnBnClickedOk)
	ON_BN_CLICKED(IDH_OK, &CScanlot::OnOK)
	ON_EN_SETFOCUS(IDC_EDIT1, &CScanlot::OnEnSetfocusEdit1)
	ON_EN_KILLFOCUS(IDC_EDIT1, &CScanlot::OnEnKillfocusEdit1)
	ON_EN_SETFOCUS(IDC_EDIT2, &CScanlot::OnEnSetfocusEdit2)
	ON_EN_KILLFOCUS(IDC_EDIT2, &CScanlot::OnEnKillfocusEdit2)
	ON_EN_SETFOCUS(IDC_EDIT3, &CScanlot::OnEnSetfocusEdit3)
	ON_EN_KILLFOCUS(IDC_EDIT3, &CScanlot::OnEnKillfocusEdit3)
	ON_EN_SETFOCUS(IDC_EDIT6, &CScanlot::OnEnSetfocusEdit6)
	ON_EN_KILLFOCUS(IDC_EDIT6, &CScanlot::OnEnKillfocusEdit6)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_CHECK1, &CScanlot::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CScanlot::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CScanlot::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CScanlot::OnBnClickedCheck4)
	ON_BN_CLICKED(IDC_CHECK1, &CScanlot::OnBnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK2, &CScanlot::OnBnClickedCheck2)
	ON_BN_CLICKED(IDC_CHECK3, &CScanlot::OnBnClickedCheck3)
	ON_BN_CLICKED(IDC_CHECK4, &CScanlot::OnBnClickedCheck4)
END_MESSAGE_MAP()


// CScanlot 消息处理程序


void CScanlot::OnBnClickedCancel()
{
	// TODO:  在此添加控件通知处理程序代码


	m_dialogRes = false;
	CDialogEx::OnCancel();
}

BOOL CScanlot::OnInitDialog()
{

	//HKL hInput = LoadKeyboardLayout(_T("00000409"), KLF_ACTIVATE);
	//PostMessage(WM_INPUTLANGCHANGEREQUEST, (WPARAM)TRUE, (LPARAM)hInput);

	m_lot1_ck = TRUE;
	m_lot2_ck = TRUE;
	m_lot3_ck = TRUE;
	m_lot4_ck = TRUE;

	if (m_bcls)
	{
		lot1 = "";
		lot2 = "";
		lot3 = "";
		lot4 = "";
	}

	/*lot1 = "https://api-watch.huami.com/forwarding/watchUS?macAddress=F483CB3939BC&pnpNumber=134&pnpVersion=257&BSN=701003483000325&BT=ACB1EEDF216B";
	lot2 = "https://api-watch.huami.com/forwarding/watchUS?macAddress=E75AD9E6589B&pnpNumber=134&pnpVersion=257&BSN=701003483000139&BT=ACB1EEDF22B3";

	lot3 = "https://api-watch.huami.com/forwarding/watchUS?macAddress=CD67A732A407&pnpNumber=134&pnpVersion=257&BSN=701003483000123&BT=ACB1EEDF22BC";
	lot4 = "https://api-watch.huami.com/forwarding/watchUS?macAddress=D22B68CC88E9&pnpNumber=134&pnpVersion=257&BSN=701003483000356&BT=ACB1EEDF21D3";

	buleid1 = "ACB1EEDF216B";
	buleid2 = "ACB1EEDF22B3";
	buleid3 = "ACB1EEDF22BC";
	buleid4 = "ACB1EEDF21D3";

	sn1 = "701003483000325";
	sn2 = "701003483000139";
	sn3 = "701003483000123";
	sn4 = "701003483000356";*/
		
	CRect rc;
	GetWindowRect(rc);

	MoveWindow(220, 400, rc.Width(), rc.Height());

	UpdateData(FALSE);
	return TRUE;
}

void CScanlot::ClearLots(bool bcls)
{
	m_bcls = bcls;


}


void CScanlot::OnBnClickedOk()
{
	// TODO:  在此添加控件通知处理程序代码

	UpdateData();

	///*if (!m_lot1_focus&&!m_lot2_focus&&!m_lot3_focus&&!m_lot4_focus)
	//{
	//	return;
	//}*/

	vector<CString> temp;
	if (m_lot1_focus)
	{
		//GetDlgItem(IDC_EDIT2)->SetFocus();
		CheckSetFocus();

		Split(lot1, _T("&BT="), temp);
		if (temp.size() <= 1)
		{
			AfxMessageBox(_T("条码数据错误!"));
			return;
		}
		buleid1 = temp[1];
		vector<CString> tempSN;
		Split(temp[0], _T("&BSN="), tempSN);
		if (tempSN.size() <= 1)
		{
			AfxMessageBox(_T("条码数据错误!"));
			return;
		}
		if (tempSN.size() < 1)
		{
			return;
		}
		sn1 = tempSN[1];

	}
	else if (m_lot2_focus)
	{
		//GetDlgItem(IDC_EDIT3)->SetFocus();
		Split(lot2, _T("&BT="), temp);
		if (temp.size() <= 1)
		{
			AfxMessageBox(_T("条码数据错误!"));
			return;
		}
		buleid2 = temp[1];
		vector<CString> tempSN;
		Split(temp[0], _T("&BSN="), tempSN);
		if (tempSN.size() <= 1)
		{
			AfxMessageBox(_T("条码数据错误!"));
			return;
		}
		if (tempSN.size() < 1)
		{
			return;
		}
		sn2 = tempSN[1];

		CheckSetFocus();
	}
	else if (m_lot3_focus)
	{
		//GetDlgItem(IDC_EDIT6)->SetFocus();
		CheckSetFocus();

		Split(lot3, _T("&BT="), temp);
		if (temp.size() <= 1)
		{
			AfxMessageBox(_T("条码数据错误!"));
			return;
		}
		buleid3 = temp[1];
		vector<CString> tempSN;
		Split(temp[0], _T("&BSN="), tempSN);
		if (tempSN.size() <= 1)
		{
			AfxMessageBox(_T("条码数据错误!"));
			return;
		}
		if (tempSN.size() < 1)
		{
			return;
		}
		sn3 = tempSN[1];
	}
	else if (m_lot4_focus)
	{
		//GetDlgItem(IDC_EDIT1)->SetFocus();
		CheckSetFocus();
		Split(lot4, _T("&BT="), temp);
		if (temp.size() <= 1)
		{
			AfxMessageBox(_T("条码数据错误!"));
			return;
		}
		buleid4 = temp[1];
		vector<CString> tempSN;
		Split(temp[0], _T("&BSN="), tempSN);
		if (tempSN.size() <= 1)
		{
			AfxMessageBox(_T("条码数据错误!"));
			return;
		}
		if (tempSN.size() < 1)
		{
			return;
		}
		sn4 = tempSN[1];
	}


	if (lot1 == ""&&lot2 == ""&&lot3 == ""&&lot4 == "")
	{
		return;
	}


	//退出条件，待定


	if (m_lot1_ck&&lot1 == "")
	{
		return;
	}
	if (m_lot2_ck&&lot2 == "")
	{
		return;
	}
	if (m_lot3_ck&&lot3 == "")
	{
		return;
	}
	if (m_lot4_ck&&lot4 == "")
	{
		return;
	}


	
	
	SetSlotEnable(m_lot1_ck, 0);
	SetSlotItem(_T("barcode"), sn1, 0);
	SetSlotItem(_T("buleID"), buleid1, 0);
	if (m_lot1_ck)
	{
		SetSlotItem(_T("result"), _T("0"), 0);
	}

	SetSlotEnable(m_lot2_ck, 1);
	SetSlotItem(_T("barcode"), sn2, 1);
	SetSlotItem(_T("buleID"), buleid2, 1);
	if (m_lot1_ck)
	{
		SetSlotItem(_T("result"), _T("0"), 1);
	}

	SetSlotEnable(m_lot3_ck, 2);
	SetSlotItem(_T("barcode"), sn3, 2);
	SetSlotItem(_T("buleID"), buleid3, 2);
	if (m_lot1_ck)
	{
		SetSlotItem(_T("result"), _T("0"), 2);
	}

	SetSlotEnable(m_lot4_ck, 3);
	SetSlotItem(_T("barcode"), sn4, 3);
	SetSlotItem(_T("buleID"), buleid4, 3);
	if (m_lot1_ck)
	{
		SetSlotItem(_T("result"), _T("0"), 3);
	}

	m_dialogRes = true;


	

	CDialogEx::OnOK();
}

bool CScanlot::isRetestMode()
{
	
	return (bool)(m_Retest == TRUE);
}

bool CScanlot::GetDialogRes()
{
	return m_dialogRes;
}

void CScanlot::OnOK()
{
	return;
}


void CScanlot::OnEnSetfocusEdit1()
{
	// TODO:  在此添加控件通知处理程序代码
	m_lot1_focus = true;
}


void CScanlot::OnEnKillfocusEdit1()
{
	// TODO:  在此添加控件通知处理程序代码
	m_lot1_focus = false;
}


void CScanlot::OnEnSetfocusEdit2()
{
	// TODO:  在此添加控件通知处理程序代码
	m_lot2_focus = true;
}


void CScanlot::OnEnKillfocusEdit2()
{
	// TODO:  在此添加控件通知处理程序代码
	m_lot2_focus = false;
}


void CScanlot::OnEnSetfocusEdit3()
{
	// TODO:  在此添加控件通知处理程序代码
	m_lot3_focus = true;
}


void CScanlot::OnEnKillfocusEdit3()
{
	// TODO:  在此添加控件通知处理程序代码
	m_lot3_focus = false;
}


void CScanlot::OnEnSetfocusEdit6()
{
	// TODO:  在此添加控件通知处理程序代码
	m_lot4_focus = true;
}


void CScanlot::OnEnKillfocusEdit6()
{
	// TODO:  在此添加控件通知处理程序代码
	m_lot4_focus = false;
}


void CScanlot::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	GetDlgItem(IDC_EDIT1)->SetFocus();
}


void CScanlot::CheckSetFocus()
{
	UpdateData();

	if (GetDlgItem(IDC_EDIT1)->IsWindowEnabled() && lot1.IsEmpty())
	{
		GetDlgItem(IDC_EDIT1)->SetFocus();
	}
	else if (GetDlgItem(IDC_EDIT2)->IsWindowEnabled() && lot2.IsEmpty())
	{
		GetDlgItem(IDC_EDIT2)->SetFocus();
	}
	else if (GetDlgItem(IDC_EDIT3)->IsWindowEnabled() && lot3.IsEmpty())
	{
		GetDlgItem(IDC_EDIT3)->SetFocus();
	}
	else if (GetDlgItem(IDC_EDIT6)->IsWindowEnabled() && lot4.IsEmpty())
	{
		GetDlgItem(IDC_EDIT6)->SetFocus();
	}
}


void CScanlot::OnBnClickedCheck1()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();

	if (m_lot1_ck)
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow();
	}
	else
	{
		GetDlgItem(IDC_EDIT1)->EnableWindow(FALSE);
	}

	CheckSetFocus();

	UpdateData(FALSE);
}


void CScanlot::OnBnClickedCheck2()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();

	if (m_lot2_ck)
	{
		GetDlgItem(IDC_EDIT2)->EnableWindow();
	}
	else
	{
		GetDlgItem(IDC_EDIT2)->EnableWindow(FALSE);
	}

	CheckSetFocus();

	UpdateData(FALSE);
}


void CScanlot::OnBnClickedCheck3()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();

	if (m_lot3_ck)
	{
		GetDlgItem(IDC_EDIT3)->EnableWindow();
	}
	else
	{
		GetDlgItem(IDC_EDIT3)->EnableWindow(FALSE);
	}

	CheckSetFocus();

	UpdateData(FALSE);
}


void CScanlot::OnBnClickedCheck4()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();

	if (m_lot4_ck)
	{
		GetDlgItem(IDC_EDIT6)->EnableWindow();
	}
	else
	{
		GetDlgItem(IDC_EDIT6)->EnableWindow(FALSE);
	}

	CheckSetFocus();

	UpdateData(FALSE);
}

CString CScanlot::getBuleID(int index)
{
	CString ret("");
	switch (index)
	{

	case 0:
		ret = buleid1;
		break;
	case 1:
		ret = buleid2;
		break;
	case 2:
		ret = buleid3;
		break;
	case 3:
		ret = buleid4;
		break;
	}

	return ret;
}
CString CScanlot::getBarCodeSn(int index)
{
	CString ret("");
	switch (index)
	{

	case 1:
		ret = sn1;
		break;
	case 2:
		ret = sn2;
		break;
	case 3:
		ret = sn3;
		break;
	case 4:
		ret = sn4;
		break;
	}

	return ret;
}

