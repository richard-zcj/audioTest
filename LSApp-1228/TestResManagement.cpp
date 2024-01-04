// TestResManagement.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "TestResManagement.h"
#include "afxdialogex.h"


// CTestResManagement 对话框

IMPLEMENT_DYNAMIC(CTestResManagement, CDialogEx)

CTestResManagement::CTestResManagement(CWnd* pParent /*=NULL*/)
: CDialogEx(CTestResManagement::IDD, pParent), m_id(-1)
{

}
CTestResManagement::CTestResManagement(int id,CWnd* pParent /*=NULL*/)
	: CDialogEx(CTestResManagement::IDD, pParent)
{
	m_id = id;
}

CTestResManagement::~CTestResManagement()
{
}

void CTestResManagement::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1TestRes, m_list);
}

BOOL CTestResManagement::OnInitDialog()
{
	UpdateData(FALSE);

	m_tip.Create(this);
	m_tip.AddTool(GetDlgItem(IDC_LIST1TestRes), _T("双击切换显示"));
	m_tip.SetDelayTime(200);
	m_tip.SetTipTextColor(RGB(0, 225, 0));
	m_tip.SetTipBkColor(RGB(245,245,255));
	m_tip.Activate(TRUE);


	return TRUE;
}

BOOL CTestResManagement::PreTranslateMessage(MSG* pMsg)
{

	if (pMsg->message == WM_MOUSEMOVE)
		m_tip.RelayEvent(pMsg);

	return CDialog::PreTranslateMessage(pMsg);
}

void CTestResManagement::setItem(const std::vector<CString>& dataItem)
{
	for (size_t i = 0; i < dataItem.size(); i++)
	{
		m_list.InsertString(i, dataItem.at(i));
	}
}

void CTestResManagement::clearItem()
{
	int count = m_list.GetCount();
	if (count>0)
	{
		for (int i = m_list.GetCount(); i >= 0; i--)
		{
			m_list.DeleteString(i);
		}
	}
	
}


BEGIN_MESSAGE_MAP(CTestResManagement, CDialogEx)
	ON_LBN_SETFOCUS(IDC_LIST1TestRes, &CTestResManagement::OnLbnSetfocusList1)
	ON_LBN_SELCHANGE(IDC_LIST1TestRes, &CTestResManagement::OnLbnSelchangeList1testres)
	ON_LBN_DBLCLK(IDC_LIST1TestRes, &CTestResManagement::OnLbnDblclkList1testres)
	ON_NOTIFY(NM_THEMECHANGED, IDC_LIST1TestRes, &CTestResManagement::OnNMThemeChangedList1testres)
	ON_WM_MOUSEHOVER()
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CTestResManagement 消息处理程序


void CTestResManagement::OnLbnSetfocusList1()
{
	// TODO:  在此添加控件通知处理程序代码
	
}


void CTestResManagement::OnLbnSelchangeList1testres()
{
	// TODO:  在此添加控件通知处理程序代码
	//AfxMessageBox(_T("0"));
	m_list.setCurIndex(m_list.GetCurSel());
	Invalidate();
}

int CTestResManagement::getSlotFrameID(CString name)
{
	int id = 0;
	bool bserch = false;
	char sn[10]{0};
	for (int i = 0; i < GetSlotCount(); i++)
	{

		slotParam* pSlot = GetSlotParam(i);



		for (int k = 0; k < pSlot->vSpkItems.size(); k++)
		{

			sprintf(sn, "%d#", i + 1);
			CString s1(sn);
			s1.Append(pSlot->vSpkItems[k].sAliasName);
			if (s1 == name)
			{
				id = pSlot->vSpkItems[k].nWaveFormID;
				bserch = true;
				break;
			}

			memset(sn, 0, 10);
		}



		if (bserch)
			break;



		for (int k = 0; k < pSlot->vMicItems.size(); k++)
		{
			sprintf(sn, "%d#", i + 1);
			CString s1(sn);
			s1.Append(pSlot->vMicItems[k].sAliasName);
			if (s1 == name)
			{
				id = pSlot->vMicItems[k].nWaveFormID;
				break;
			}
			memset(sn, 0, 10);
		}




	}

	return id;
}


void CTestResManagement::OnLbnDblclkList1testres()
{
	// TODO:  在此添加控件通知处理程序代码
	int i=m_list.GetCurSel();
	if (i >= 0)
	{
		CString text;
		m_list.GetText(i, text);

		int senid = getSlotFrameID(text);

		::SendMessage(m_hwnd, RESITEMSSHOW, WPARAM(0), LPARAM(senid));

		//if (text == "FR")
		//{
		//	m_showItems.push_back(0);
		//	::SendMessage(m_hwnd, RESITEMSSHOW, WPARAM(0), LPARAM(m_id));
		//}
		//else if (text == "THD")
		//{
		//	m_showItems.push_back(1);
		//	::SendMessage(m_hwnd, RESITEMSSHOW, WPARAM(1), LPARAM(m_id));
		//}
		//else if (text == "THDN")
		//{
		//	m_showItems.push_back(1);
		//	::SendMessage(m_hwnd, RESITEMSSHOW, WPARAM(2), LPARAM(m_id));
		//}
		//else if (text == "RB")
		//{
		//	//m_showItems.push_back(1);
		//	::SendMessage(m_hwnd, RESITEMSSHOW, WPARAM(3), LPARAM(m_id));
		//}
		//else if (text == "Phase")
		//{
		//	m_showItems.push_back(1);
		//	::SendMessage(m_hwnd, RESITEMSSHOW, WPARAM(4), LPARAM(m_id));
		//}
		//else if (text == "Sensitivity_mic")
		//{
		//	m_showItems.push_back(1);
		//	::SendMessage(m_hwnd, RESITEMSSHOW, WPARAM(5), LPARAM(m_id));
		//}
		//else if (text == "Sensitivity_spk")
		//{
		//	m_showItems.push_back(1);
		//	::SendMessage(m_hwnd, RESITEMSSHOW, WPARAM(6), LPARAM(m_id));
		//}
		//else if (text=="响应信号")
		//{
		//	::SendMessage(m_hwnd2, RESITEMSSHOW, WPARAM(7), LPARAM(m_id));
		//}
		//else if (text == "SNR")
		//{
		//	::SendMessage(m_hwnd, RESITEMSSHOW, WPARAM(8), LPARAM(m_id));
		//}
		//else if (text == "Polarity")
		//{
		//	::SendMessage(m_hwnd, RESITEMSSHOW, WPARAM(9), LPARAM(m_id));
		//}
		//else if (text == "Balance")
		//{
		//	::SendMessage(m_hwnd, RESITEMSSHOW, WPARAM(10), LPARAM(m_id));
		//}
		//else if (text == "NoiseReduction")
		//{
		//	::SendMessage(m_hwnd, RESITEMSSHOW, WPARAM(11), LPARAM(m_id));
		//}
		
		//AfxMessageBox(text);

	}

}

void CTestResManagement::getShowItems(std::vector<int>& dataItem)
{
	dataItem.clear();
	for (size_t i = 0; i < m_showItems.size(); i++)
	{

		dataItem.push_back(m_showItems[i]);

	}
}

void CTestResManagement::setHwnd(HWND wnd, HWND wnd2)
{
	m_hwnd = wnd;
	m_hwnd2 = wnd2;
}

void CTestResManagement::setWindowText(CString title)
{
	SetWindowTextW(title);
}




void CTestResManagement::OnNMThemeChangedList1testres(NMHDR *pNMHDR, LRESULT *pResult)
{
	// 该功能要求使用 Windows XP 或更高版本。
	// 符号 _WIN32_WINNT 必须 >= 0x0501。
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}


void CTestResManagement::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	Invalidate();

	rc.left = point.x;
	rc.top = point.y;
	rc.right = rc.left + 100;
	rc.bottom = rc.top + 30;

	CDialogEx::OnMouseHover(nFlags, point);
}

void CTestResManagement::setRes(bool* res)
{
	m_res = res;
	CString text;
	map<CString, bool> m_res_map;
	//bool bres[6]{false, true, false, true, false, true};


	//m_res = bres;

	for (size_t i = 0; i < 6; i++)
	{
		m_list.GetText(i, text);
		if (m_res[i])
		{

			m_res_map[text] = true;
		}
		else
		{
			m_res_map[text] = false;
		}
	}


	m_list.serRes(m_res_map);

	


	bpass = true;
	bool temp1[6];
	for (size_t i = 0; i < 6; i++)
	{
		temp1[i] = m_res[i];
		if (m_res[i])
		{

			//m_list.SetItemData(i, dwColr1);

		}
		else
		{
			//m_list.SetItemData(i, dwColr);

			bpass = false;
		}
	}

	vector<CString> stile;
	if (bpass)
	{
		CString stitle;
		GetWindowTextW(stitle);
		Split(stitle, _T("pass"), stile);
		if (stile.size() == 0)
		{
			CString spass(_T("pass"));
			stitle.Append(spass);
			//SetWindowTextW(stitle);
		}
	}
	else
	{
		CString stitle;
		GetWindowTextW(stitle);
		Split(stitle, _T("fail"), stile);
		if (stile.size() == 0)
		{
			CString spass(_T("fail"));
			stitle.Append(spass);
			//SetWindowTextW(stitle);
		}

	}

}


void CTestResManagement::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	CFont f;
	f.CreatePointFont(480, _T("宋体"));
	dc.SelectObject(&f);
	dc.SetBkMode(TRANSPARENT);
	if (bpass)
	{

		dc.SetTextColor(RGB(0, 255, 0));
		dc.TextOutW(130, 15, _T("PASS"));
	}
	else
	{
		dc.SetTextColor(RGB(255, 0, 0));
		dc.TextOutW(130, 15, _T("FAIL"));
	}
	

}
