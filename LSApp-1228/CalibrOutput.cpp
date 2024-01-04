// CalibrOutput.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "CalibrOutput.h"
#include "afxdialogex.h"
#include "DeviceManage.h"
#include "wavHandler.h"
#include "InputDialg.h"


#define EXCITATION	"Ao0"
extern CDeviceManage devicemanage;

// CCalibrOutput 对话框

IMPLEMENT_DYNAMIC(CCalibrOutput, CDialogEx)
//CUSB4431 * pDevUSB4431;
CCalibrOutput::CCalibrOutput(CWnd* pParent /*=NULL*/)
: CDialogEx(CCalibrOutput::IDD, pParent), vpp(0.8), vp(1), ap(1),
m_involtage(0.1), m_outvoltage(1), m_involtageAP(0.1), m_outvoltageAP(1)
{
	/*pDevUSB4431 = new CUSB4431();
	if (pDevUSB4431)
		pDevUSB4431->AddCallBackPtr(this);*/
}

CCalibrOutput::~CCalibrOutput()
{
}

void CCalibrOutput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_tree);

	DDX_Text(pDX, IDC_calibraoutputdevices, devicename);
	DDX_Text(pDX, IDC_EDIToutvv, v);
	DDX_Text(pDX, IDC_editcalibrationoutvp, vp);
	DDX_Text(pDX, IDC_editcalibrationoutAPva, ap);

	DDX_Control(pDX, IDC_calibraoutputdevices, m_cmboutDevices);
	
	DDX_Text(pDX, IDC_EDITinputvolaty, m_involtage);
	DDX_Text(pDX, IDC_EDIToutputvolaty, m_outvoltage);

	DDX_Text(pDX, IDC_EDITinputvolatyAP, m_involtageAP);
	DDX_Text(pDX, IDC_EDIToutputvolatyAP, m_outvoltageAP);
}


BEGIN_MESSAGE_MAP(CCalibrOutput, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CCalibrOutput::OnBnClickedButtonStartCalibration)
	ON_BN_CLICKED(IDC_BUTTON2, &CCalibrOutput::OnBnClickedButtonDelInstru)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, &CCalibrOutput::OnNMClickTree1)
	ON_NOTIFY(NM_KILLFOCUS, IDC_TREE1, &CCalibrOutput::OnNMKillfocusTree1)
	ON_NOTIFY(TVN_KEYDOWN, IDC_TREE1, &CCalibrOutput::OnTvnKeydownTree1)
	ON_BN_CLICKED(IDC_BUTTON3, &CCalibrOutput::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CCalibrOutput::OnBnClickedButtonstopNIplay)
	ON_BN_CLICKED(IDC_BUTTON5, &CCalibrOutput::OnBnClickedButtonUpdateoutCHsVps)
	ON_BN_CLICKED(IDC_BUTTON9, &CCalibrOutput::OnBnClickedButtonUpdateAP)
	ON_CBN_DROPDOWN(IDC_calibraoutputdevices, &CCalibrOutput::OnCbnDropdowncalibraoutputdevices)
	ON_BN_CLICKED(IDH_OK, &CCalibrOutput::OnOK)
//	ON_WM_PAINT()
ON_WM_PAINT()
ON_CBN_DROPDOWN(IDC_COMBO1, &CCalibrOutput::OnCbnDropdownCombo1)
END_MESSAGE_MAP()


// CCalibrOutput 消息处理程序

//0.366/0.25=1.464 vp=1.464  rme
void CCalibrOutput::OnBnClickedButtonStartCalibration()
{


	

	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	//判断是否重名
	if (isdupname(devicename))
	{
		AfxMessageBox(_T("设备标识符重名！"));
		//GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("开始校准"));
		return;
	}

	GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("校准中"));
	char buf[20]{0};
	sprintf(buf, "设备%d.cfg", ++maxdeviceid);
	CString decname = CString(buf);
	if (devicename != "")
	{
		decname = devicename;
		decname.Append(CString(".cfg"));
		maxdeviceid--;
	}
	
	m_tree.InsertItem(decname, 1, 1, hroot, TVI_LAST);
	m_tree.Expand(hroot, TVE_EXPAND);

	int** dat = NULL;
	devicemanage.setDeviceVpVal(devicename, 1, false);
	devicemanage.calbrationplay(devicename, m_involtage);
	if (dat&&*dat)
	{
		char* fileName1 = "D:\\TestRecord\\mic01.wav";
		wavwrite(fileName1, 1, 44100, 32, (char*)dat[0], 44100 * 4);
	}

	//CInputDialg* inputdlg = new CInputDialg;
	//inputdlg->ShowWindow(TRUE);


	CInputDialg inputdlg(false);
	inputdlg.setCaption(CString("输出电压值："));
	inputdlg.DoModal();
	

	m_outvoltage=inputdlg.getV();
	if (m_outvoltage == 0)
	{
		AfxMessageBox(_T("无输出电压...."));
		GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("开始校准"));
		return;
	}
	vp = m_outvoltage / m_involtage;

	int intap = vp * 10000;
	vp = 1.0*intap / 10000.0;

	UpdateData(FALSE);
	GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("开始校准"));
    
}

void CCalibrOutput::cal_Vp(CString innerDeviceName, double& vpout)
{
	int** dat = NULL;
	if (!devicemanage.calbrationplay(innerDeviceName, 0))
		return;

	if (dat&&*dat)
	{
		char* fileName1 = "D:\\TestRecord\\mic01.wav";
		wavwrite(fileName1, 1, 44100, 32, (char*)dat[0], 44100 * 4);
	}

	//CInputDialg* inputdlg = new CInputDialg;
	//inputdlg->ShowWindow(TRUE);


	CInputDialg inputdlg(false);
	inputdlg.setCaption(CString("输出电压值："));
	inputdlg.setunit(CString("VRms"));
	inputdlg.DoModal();


	m_outvoltage = inputdlg.getV();
	if (m_outvoltage == 0)
	{
		AfxMessageBox(_T("无输出电压...."));
		//GetDlgItem(IDC_BUTTON1)->SetWindowTextW(_T("开始校准"));
		return;
	}

	m_involtage = 0.1;//给的激励有效值为0.1m_involtage，万用表读的有效值m_outvoltage
	vp = m_outvoltage / m_involtage;

	int intap = vp * 10000;
	vp = 1.0*intap / 10000.0;

	vpout = vp;

	//UpdateData(FALSE);
}

void  CCalibrOutput::OnOK()
{
	return;

}

BOOL CCalibrOutput::OnInitDialog()
{
	UpdateData(FALSE);


	hroot = m_tree.InsertItem(_T("输出通道1"), 0, 0, TVI_ROOT);
	hroot2 = m_tree.InsertItem(_T("输出通道2"), 0, 0, TVI_ROOT);


	m_tree.InsertItem(_T("设备1.cfg"), 1, 1, hroot, TVI_LAST);
	m_tree.InsertItem(_T("设备2.cfg"), 1, 1, hroot, TVI_LAST);
	m_tree.InsertItem(_T("设备3.cfg"), 1, 1, hroot, TVI_LAST);
	m_tree.InsertItem(_T("设备4.cfg"), 1, 1, hroot, TVI_LAST);

	m_tree.InsertItem(_T("设备1.cfg"), 1, 1, hroot2, TVI_LAST);
	m_tree.Expand(hroot, TVE_EXPAND);



	getmaxdeviceId();
	SetBackgroundColor(RGB(248, 248, 255));
	
	OnCbnDropdowncalibraoutputdevices();
	return TRUE;
}

bool CCalibrOutput::isdupname(CString addname)
{
	bool isdup = false;
	addname.Append(CString(".cfg"));
	HTREEITEM child = m_tree.GetChildItem(hroot);

	while (child != NULL)
	{
		CString name = m_tree.GetItemText(child);
		
		if (name == addname)
			isdup = true;

		child = m_tree.GetNextSiblingItem(child);

	}


	return isdup;

}


void CCalibrOutput::getmaxdeviceId()
{
	HTREEITEM child = m_tree.GetChildItem(hroot);
	int id = 0;
	maxdeviceid = 0;
	while (child != NULL)
	{
		CString str = m_tree.GetItemText(child);
		if (str.GetLength() >= 3)
		{
			if (str[0] == CString(_T("设")) &&
				str[1] == CString(_T("备")))
			{
				bool bisnum = true;
				for (size_t i = 2; i < str.GetLength(); i++)
				{
					if (str[i] == (CString(_T("0")) || CString(_T("0"))) || CString(_T("1")) || CString(_T("2"))
						|| CString(_T("3")) || CString(_T("4")) || CString(_T("5")) || CString(_T("6")) || CString(_T("7"))
						|| CString(_T("8")) || CString(_T("9")))
					{

					}
					else
					{
						bisnum = false;

					}
				}


				if (bisnum)
				{

					CString sss = str.Right(str.GetLength() - 2);
					int len1 = WideCharToMultiByte(CP_ACP, 0, sss, -1, NULL, 0, NULL, NULL);
					char* ch = new char[len1 + 1]{0};
					WideCharToMultiByte(CP_ACP, 0, sss, -1, ch, len1, NULL, NULL);
					id = atoi(ch);
				}
			}
		}

		if (maxdeviceid < id)
		{
			maxdeviceid = id;
		}

		child = m_tree.GetNextSiblingItem(child);


	}

}



void CCalibrOutput::OnBnClickedButtonDelInstru()
{
	// TODO:  在此添加控件通知处理程序代码
	CString s = m_tree.GetItemText(hItemdel);
	if (s.Compare(CString("输出通道1")) == 0)
	{
		return;
	}
	if (s.Compare(CString("输出通道2")) == 0)
	{
		return;
	}
	if (s.Compare(CString("")) == 0)
	{
		return;
	}



	for (size_t i = 0; i < deldat.size(); i++)
	{
		m_tree.DeleteItem(deldat[i]);
	}

	if (deldat.size() == 0)
	{
		m_tree.DeleteItem(hItemdel);
	}
	else
	{
		deldat.clear();
	}



	getmaxdeviceId();
}


void CCalibrOutput::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	CTreeCtrl* ctreectrl = (CTreeCtrl*)GetDlgItem(IDC_TREE1);

	CPoint pt = GetCurrentMessage()->pt;

	ctreectrl->ScreenToClient(&pt);

	UINT uFlags = 0;

	HTREEITEM hItem = ctreectrl->HitTest(pt, &uFlags);

	if (hItem != NULL && (TVHT_ONITEM&uFlags))
	{
		ctreectrl->SelectItem(hItem);

		int nDat = ctreectrl->GetItemData(hItem);

		if (iskeysheetf)
		{
			deldat.clear();
			hItemdel2 = hItem;

			HTREEITEM child = m_tree.GetChildItem(hroot);
			bool istag = false;
			while (child != NULL)
			{
				if (m_tree.GetItemText(child) == m_tree.GetItemText(hItemdel))
					istag = true;



				if (istag)
				{
					m_tree.SetItemState(child, TVIS_SELECTED, TVIS_SELECTED);
					deldat.push_back(child);
				}

				if (m_tree.GetItemText(child) == m_tree.GetItemText(hItemdel2))
					istag = false;




				child = m_tree.GetNextSiblingItem(child);
			}

			iskeysheetf = false;


		}
		else
			hItemdel = hItem;





	}
	else
	{
		if (deldat.size() > 1)
		{
			HTREEITEM child = m_tree.GetChildItem(hroot);
			while (child != NULL)
			{

				m_tree.SetItemState(child, 0, TVIS_SELECTED);

				child = m_tree.GetNextSiblingItem(child);
			}
		}


	}
	*pResult = 0;
}


void CCalibrOutput::OnNMKillfocusTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	

	if (deldat.size() > 1)
	{
		HTREEITEM child = m_tree.GetChildItem(hroot);
		while (child != NULL)
		{

			m_tree.SetItemState(child, 0, TVIS_SELECTED);

			child = m_tree.GetNextSiblingItem(child);
		}
	}

	if (iskeysheetf)
		return;
	CString title;
	CWnd* wnd = m_tree.GetFocus();
	if (wnd)
	{
		wnd->GetWindowTextW(title);
		if (title != ""&&title.Compare(CString("删除仪器")) == 0)
		{
			
			return;
		}

	}
	
	hItemdel = NULL;
	*pResult = 0;
}


void CCalibrOutput::OnTvnKeydownTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVKEYDOWN pTVKeyDown = reinterpret_cast<LPNMTVKEYDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	if (pTVKeyDown->wVKey == VK_SHIFT)
	{


		iskeysheetf = true;//防止killfous报错
	}
	*pResult = 0;
}



//1  0.8
//2 1.6'
//3 2.4
void CCalibrOutput::OnBnClickedButton3()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	double * outdata = new double[44100 * 3];
	double vv = v;
	for (size_t i = 0; i < 44100 * 3; i++)
	{
		
		outdata[i] = vv*sqrt(2)*sin(2.0*3.1415926 * 1000 * i / 44100);
	}



	double voltage = vv*sqrt(2);
	bool b_ok = false;
	DAQmxAoTaskParam param;
	//if (pDevUSB4431)
	//{
	//	pDevUSB4431->AddCallBackPtr(this);
	//	pDevUSB4431->GetAoTaskParam(EXCITATION, &param);
	//	param.sampleMode = DAQmx_Val_ContSamps;// DAQmx_Val_ContSamps;
	//	param.sampleRate = 44100;
	//	param.minVoltage = -3.5/sqrt(2);
	//	param.maxVoltage = 3.5 / sqrt(2);
	//	param.samples = 44100*3;
	//	param.data = outdata;
	//	param.physicalChannel = "Dev1/ao0";
	//	b_ok = pDevUSB4431->LoadAoTask(EXCITATION, param, NULL);
	//	if (b_ok)
	//		b_ok = pDevUSB4431->StartAoTask(EXCITATION);
	//	/*if (b_ok)
	//		MessageBox(_T("数据已下发IN采集卡"), _T("成功！"));*/
	//}
}


void CCalibrOutput::OnBnClickedButtonstopNIplay()
{
	// TODO:  在此添加控件通知处理程序代码
	//pDevUSB4431->StopAoTask(EXCITATION);
}


void CCalibrOutput::OnBnClickedButtonUpdateoutCHsVps()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	if (devicename == "")
	{
		AfxMessageBox(_T("更新通道名称不存在在！！"));
		return;
	}
    
	devicemanage.setDeviceVpVal(devicename, vp, false);
	AfxMessageBox(_T("VP值更新完成！"));
}


void CCalibrOutput::OnBnClickedButtonUpdateAP()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();


	devicemanage.setDeviceVpVal(devicename, 1, false);
	devicemanage.calbrationplay(devicename, m_involtageAP);


	CInputDialg inputdlg(false);
	inputdlg.setCaption(CString("输出电压值："));
	inputdlg.DoModal();


	m_outvoltageAP = inputdlg.getV();
	if (m_outvoltageAP == 0)
	{
		AfxMessageBox(_T("无输出电压...."));
		return;
	}

	ap = m_outvoltageAP / m_involtageAP;

	int intap = ap * 10000;
	ap = 1.0*intap / 10000.0;

	devicemanage.setApval(ap);
	UpdateData(FALSE);
}


void CCalibrOutput::OnCbnDropdowncalibraoutputdevices()
{
	// TODO:  在此添加控件通知处理程序代码
	if (devicemanage.m_hWnd)
	{
		vector<CString> temp = devicemanage.getInnerNames_OutCHS();

		m_cmboutDevices.ResetContent();

		for (size_t j = 0; j < 1; j++)//temp.size()
		{
			m_cmboutDevices.InsertString(j, temp[j]);
		}

		m_cmboutDevices.SetCurSel(0);
	}
	
}

void CCalibrOutput::drawGroupBox(CWnd* wnd, CPaintDC* dc)
{
	CRect rc;

	int dc_id = dc->SaveDC();
	CBrush groupbox_brush(RGB(255, 0, 8));
	CPen pen;
	pen.CreatePen(0, 1, RGB(255, 0, 8));
	//dc->SelectObject(pen);

	wnd->GetWindowRect(rc);
	ScreenToClient(rc);
	rc.top += 14;
	MoveToEx(*dc, rc.left + 100, rc.top, NULL);
	LineTo(*dc, rc.right, rc.top);

	MoveToEx(*dc, rc.right, rc.top, NULL);
	LineTo(*dc, rc.right, rc.bottom);

	MoveToEx(*dc, rc.right, rc.bottom, NULL);
	LineTo(*dc, rc.left - 1, rc.bottom);

	MoveToEx(*dc, rc.left - 1, rc.bottom, NULL);
	LineTo(*dc, rc.left - 1, rc.top);

	MoveToEx(*dc, rc.left - 1, rc.top, NULL);
	LineTo(*dc, rc.left + 19, rc.top);

	dc->RestoreDC(dc_id);
}



//void CCalibrOutput::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO:  在此处添加消息处理程序代码
//	// 不为绘图消息调用 __super::OnPaint()
//	CWnd* pwnd = GetDlgItem(IDC_STATIC);
//	drawGroupBox(pwnd, &dc);
//
//	pwnd = GetDlgItem(IDC_STATICgongfang);
//	drawGroupBox(pwnd, &dc);
//
//	pwnd = GetDlgItem(IDC_STATICoutdevicemange);
//	drawGroupBox(pwnd, &dc);
//
//
//}


void CCalibrOutput::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 __super::OnPaint()

	CWnd* pwnd = GetDlgItem(IDC_STATIC);
	drawGroupBox(pwnd, &dc);

	pwnd = GetDlgItem(IDC_STATICgongfang);
	drawGroupBox(pwnd, &dc);

	pwnd = GetDlgItem(IDC_STATICoutdevicemange);
	drawGroupBox(pwnd, &dc);
}


void CCalibrOutput::OnCbnDropdownCombo1()
{
	// TODO:  在此添加控件通知处理程序代码
}
