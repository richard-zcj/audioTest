// MYcontrol2.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "MYcontrol2.h"
#include "afxdialogex.h"
#include "JiLiSetNewVersion.h"
#include "PlayRecordSettings.h"
#include "DeviceManage.h"
#include "CalibInput.h"
#include "CalibrOutput.h"

#include "Calibration.h"
#include "InputDialg.h"




extern JiLiSetNewVersion jilisetnew;
extern CPlayRecordSettings playrecordset;
extern CDeviceManage devicemanage;

extern CCalibration calibration;
// MYcontrol2 对话框

IMPLEMENT_DYNAMIC(MYcontrol2, CDialogEx)

//MYcontrol2::MYcontrol2(bool isinput,CWnd* pParent /*=NULL*/)
//: CDialogEx(MYcontrol2::IDD, pParent), ischecked(false), m_factoryname(""),
//fs(44100), m_drivertype(""), vp(1.0), m_bVP(false)
//{
//	m_isinput = isinput;
//}

MYcontrol2::MYcontrol2(bool isinput,int id, CWnd* pParent /*=NULL*/)
: CDialogEx(MYcontrol2::IDD, pParent), ischecked(false), m_factoryname(""),
fs(44100), m_drivertype(""), vp(1.0), m_bVP(false)
{
	m_isinput = isinput;
	m_id = id;
}

MYcontrol2::~MYcontrol2()
{
}
//IDC_mycontroldeviceName
void MYcontrol2::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_mycontroldrivername, m_cmbDriverName);
	DDX_Control(pDX, IDC_mycontroldevicename2, m_cmbdevicename);
	DDX_Control(pDX, IDC_mycontroldeviceName, m_cmbChs);
	DDX_Text(pDX, IDC_mycontroldevicename2, m_factoryname);
	DDX_Text(pDX, IDC_mycontrolchname, m_innerDevicename);

	DDX_Text(pDX, IDC_mycontrolfs, fs);
	DDX_Text(pDX, IDC_mycontrolchnames, vp);
	DDX_Text(pDX, IDC_mycontroldrivername, m_drivertype);

	DDX_Text(pDX, IDC_mycontrolchname, m_chsname);
	//IDC_mycontroldeviceName

	DDX_Control(pDX, IDC_mycontrolbits, m_samplebits);
	DDX_Control(pDX, IDC_mycontrolfs, m_chsFs);
	DDX_Text(pDX, IDC_mycontrolsenOrzengyi, sen);
}

void MYcontrol2::setInnerChsName(CString name)
{
	m_innerDevicename = name;
}

bool MYcontrol2::isHwnd(HWND wnd)
{
	if (GetDlgItem(IDC_mycontrolchname)->m_hWnd||
		GetDlgItem(IDC_mycontroldrivername)->m_hWnd ||
		GetDlgItem(IDC_mycontroldevicename2)->m_hWnd ||
		GetDlgItem(IDC_mycontroldeviceName)->m_hWnd ||
		GetDlgItem(IDC_mycontrolchnames)->m_hWnd ||
		GetDlgItem(IDC_mycontrolfs)->m_hWnd ||
		GetDlgItem(IDC_mycontrolbits)->m_hWnd)
	{
		return true;
	}
	return false;
}

void MYcontrol2::openASIOPanle()
{

	//打开ASIO控制面板
	//C:\Program Files\LISSIG\USB Audio Device Driver\x64\LISSIGUsbAudioCpl.exe

	//system("C:\\Program Files\\LISSIG\\USB Audio Device Driver\\x64\\LISSIGUsbAudioCpl.exe");

	CString drivername;
	UpdateData();
	if (m_drivertype == "ASIO")
	{

		if (m_factoryname == "ASIO Fireface USB")

		{

			ShellExecute(NULL, _T("open"), _T("firefaceusb.exe"),
				NULL, NULL, SW_SHOW);
		}
		else if (m_factoryname == "LISSIG USB Audio Device")
		{

			ShellExecute(NULL, _T("open"), _T("C:\\Program Files\\LISSIG\\USB Audio Device Driver\\x64\\LISSIGUsbAudioCpl.exe"),
				NULL, NULL, SW_SHOW);
		}


	}

	


}

void MYcontrol2::addChs()
{
	if (m_isinput)
	{

		devicemanage.OnBnClickedButtonaddInCh();
		
	}
	else
	{
		devicemanage.OnBnClickedButtonaddOutChs();
	}
}

void MYcontrol2::cal_sen()
{
	UpdateData();
	if (m_isinput)
	{

		sen=calibration.calibrinput.cal_sensitivity(m_innerDevicename, vp, fs);
		CString sValue(_T(""));
		sValue.Format(_T("%.1f"), sen);
		GetDlgItem(IDC_mycontrolsenOrzengyi)->SetWindowText(sValue);
		sen = _ttof(sValue);


		UpdateData(FALSE);
		AfxMessageBox(_T("cal_sen"));
	}
	else
	{
		AfxMessageBox(_T("输出通道无需校准灵敏度值!"));
	}

	
}


void MYcontrol2::cal_Ap()
{
	AfxMessageBox(_T("cal_Ap"));
}

void MYcontrol2::cal_VP()
{
	m_bVP = true;
	UpdateData();

	/*if (m_drivertype != "ASIO")
	{
		AfxMessageBox(_T("校准使用ASIO驱动!!!"));
		return;
	}*/

	
	if (m_isinput)
	{
		CInputDialg inputdlg(true);
		inputdlg.setCaption(CString("输出通道："));
		inputdlg.setunit(_T(""));
		inputdlg.DoModal();
		CString outchsName = inputdlg.getV2();


		int outchs=devicemanage.getChsIndexByName(outchsName, false);//ASIO index

		int inchs = m_cmbChs.GetCurSel();

		if (m_drivertype == "WDM")
		{
			inchs = devicemanage.getMMEChsIndexByName(m_innerDevicename, true);// m_cmbdevicename.GetCurSel();
			outchs = devicemanage.getMMEChsIndexByName(outchsName, false);
		}

		double outvp = devicemanage.getoutvp(outchsName);



		calibration.calibrinput.cal_Vp(fs, m_innerDevicename, vp, outchs, inchs, outvp);



		
	}
	else
	{
		calibration.calibroutput.cal_Vp(m_innerDevicename,vp);
		//AfxMessageBox(_T("cal_VP out"));
		OnCbnEditchangemycontrolchnames();
	}



	UpdateData(FALSE);

	//AfxMessageBox(_T("校准完成,通道VP值已更新..."));
	AfxMessageBox(_T("校准完成"));
	m_bVP = false;

	jilisetnew.data.clear();
	
}


BOOL MYcontrol2::PreTranslateMessage(MSG* pMsg)
{

	

	if (isHwnd(pMsg->hwnd))
	{

		if (pMsg->message == WM_MOUSEMOVE)
		{
			//onDbClicklist();
			char str[300];

			sprintf(str, "x=%d,y=%d\n", pMsg->pt.x, pMsg->pt.y);

			if (abs(pMsg->pt.x - 990) <= 2 && abs(pMsg->pt.y - 260) <= 10)
			{
				HCURSOR hCur = LoadCursor(NULL, IDC_SIZEWE);

				::SetCursor(hCur);
				//AfxMessageBox(CString(str));
			}

			//OutputDebugString(CString(str));
			
		}
		else if (pMsg->message == WM_RBUTTONUP)
		{

			//onRClicklist();
			openMenu();

			return TRUE;
			

		}

	}

	return CDialog::PreTranslateMessage(pMsg);
}

void MYcontrol2::openMenu()
{
	CMenu menu;
	POINT  pt{ 0 };
	GetCursorPos(&pt);
	menu.LoadMenuW(IDR_MYCONTROL);

	menu.GetSubMenu(0)->TrackPopupMenu(0, pt.x, pt.y, this);
}

void MYcontrol2::setChecked()
{
	CEdit* pedit = (CEdit*)GetDlgItem(IDC_mycontrolchname);
	
}

BOOL MYcontrol2::OnInitDialog()
{
	UpdateData(FALSE);
	m_cmbDriverName.InsertString(0, _T("WDM"));
	m_cmbDriverName.InsertString(1, _T("ASIO"));
	m_cmbDriverName.SetCurSel(-1);

	m_samplebits.InsertString(0, _T("16"));
	m_samplebits.InsertString(1, _T("24"));
	m_samplebits.InsertString(2, _T("32"));
	m_samplebits.SetCurSel(1);

	m_chsFs.InsertString(0, _T("44100"));
	m_chsFs.InsertString(1, _T("48000"));
	m_chsFs.InsertString(2, _T("96000"));
	m_chsFs.SetCurSel(0);

	SetBackgroundColor(RGB(248, 248, 255));

	return TRUE;

}

HWND MYcontrol2::getHwnd()
{
	if (this->m_hWnd)
		return m_hWnd;
	else
	{
		return NULL;
	}
}


BEGIN_MESSAGE_MAP(MYcontrol2, CDialogEx)
	ON_WM_RBUTTONUP()
	ON_WM_CTLCOLOR()
	ON_EN_SETFOCUS(IDC_mycontrolchname, &MYcontrol2::OnEnSetfocusmycontrolchname)
	ON_EN_KILLFOCUS(IDC_mycontrolchname, &MYcontrol2::OnEnKillfocusmycontrolchname)
	ON_CBN_SELCHANGE(IDC_mycontroldrivername, &MYcontrol2::OnCbnSelchangemycontroldrivername)

	ON_CBN_SELCHANGE(IDC_mycontroldevicename2, &MYcontrol2::OnCbnSelchangemycontroldevicename2)
	ON_WM_CLOSE()
	ON_WM_MOUSEWHEEL()
	ON_EN_CHANGE(IDC_mycontrolchname, &MYcontrol2::OnEnChangemycontrolchname)
	ON_CBN_EDITCHANGE(IDC_mycontrolchnames, &MYcontrol2::OnCbnEditchangemycontrolchnames)
	ON_WM_PAINT()
	ON_COMMAND(ID_32780, &MYcontrol2::openASIOPanle)
	ON_COMMAND(ID_32778, &MYcontrol2::addChs)
	ON_COMMAND(ID_32795, &MYcontrol2::cal_sen)
	ON_COMMAND(ID_32796, &MYcontrol2::cal_Ap)
	ON_COMMAND(ID_32797, &MYcontrol2::cal_VP)
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEHOVER()
	ON_CBN_SELCHANGE(IDC_mycontroldeviceName, &MYcontrol2::OnSelchangeMycontroldevicename)
//	ON_CBN_EDITCHANGE(IDC_mycontrolfs, &MYcontrol2::OnEditchangeMycontrolfs)
//	ON_CBN_EDITCHANGE(IDC_mycontrolbits, &MYcontrol2::OnEditchangeMycontrolbits)
	ON_CBN_EDITCHANGE(IDC_mycontrolsenOrzengyi, &MYcontrol2::OnEditchangeMycontrolsenorzengyi)
	ON_CBN_SELCHANGE(IDC_mycontrolfs, &MYcontrol2::OnSelchangeMycontrolfs)
	ON_CBN_SELCHANGE(IDC_mycontrolbits, &MYcontrol2::OnSelchangeMycontrolbits)
END_MESSAGE_MAP()


// MYcontrol2 消息处理程序


void MYcontrol2::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnRButtonUp(nFlags, point);
}

bool MYcontrol2::isCheck()
{
	/*bool b = false;

	CString s;
	GetDlgItem(IDC_mycontrolchname)->GetWindowTextW(s);
	CWnd* id = GetDlgItem(IDC_mycontrolchname);
	
	CWnd* id2=GetFocus();
	

	POINT p;
	BOOL b1=GetCursorPos(&p);

	id2=WindowFromPoint(p);
	id2->GetWindowTextW(s);

	if (id == id2)
	{
		b = true;
	}*/


	return ischecked;

}


HBRUSH MYcontrol2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	CBrush bm;
	if (pWnd->GetDlgCtrlID() == IDC_mycontrolchname)
	{
		/*pDC->SetBkColor(RGB(0, 0, 255));
		bm.CreateSolidBrush(RGB(0, 0, 255));*/

		//return bm;

	}


	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void MYcontrol2::OnEnSetfocusmycontrolchname()
{
	// TODO:  在此添加控件通知处理程序代码
	ischecked = true;
}


void MYcontrol2::OnEnKillfocusmycontrolchname()
{
	// TODO:  在此添加控件通知处理程序代码
	//ischecked = false;
}


void MYcontrol2::OnCbnSelchangemycontroldrivername()
{
	// TODO:  在此添加控件通知处理程序代码
	int index = m_cmbDriverName.GetCurSel();

	switch (index)
	{
	case 0:
		//加载wdm驱动信息
		loadWDM(m_isinput);
		break;
	case 1:
		//加载asio驱动信息
		loadASIO(m_isinput);
		break;
	default:
		break;
	}

	if (devicemanage.isInitFinish())
	{
		UpdateData();
		EditChannelDev(_T("driver"), m_drivertype, m_id, m_isinput);
	}
}

void MYcontrol2::loadWDM(bool binput)
{
	clearCmb();	
	audioDriver.LoadWDM(wdmDeviceName, wdmChName, binput);
	//m_samplebits.SetCurSel(1);
	for (size_t i = 0; i < wdmDeviceName.size(); i++)
	{
		m_cmbdevicename.InsertString(i, CString(wdmDeviceName[i].c_str()));
		

	}

	if (m_cmbdevicename.GetCount()>0)
	{
		m_cmbdevicename.SetCurSel(0);
		OnCbnSelchangemycontroldevicename2();
	}
	
}

void MYcontrol2::readRegsterVal(vector<string>& asioDriverName)
{
	asioDriverName.clear();
	HKEY hKey_return = NULL;

	if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, _T("SOFTWARE\\ASIO"), 0,KEY_READ, &hKey_return) != ERROR_SUCCESS)
	{
		AfxMessageBox(_T("registrer read failed"));
	}
	char str[1024]{0};
	char* keyval;
	WCHAR* valname;
	DWORD keySzType = 0;
	DWORD ketSize = 0;

	unsigned long numkeys=0;
	unsigned long longestkeys = 0;
	unsigned long numvales = 0;
	unsigned long longestvales = 0;
	unsigned long longestdatalen = 0;

	unsigned long datalen = 0;
	unsigned long vallen = 0;
	unsigned long type = 0;


	if (RegQueryInfoKeyW(hKey_return, NULL, NULL, NULL, &numkeys, &longestkeys
		, NULL, &numvales, &longestvales, &longestdatalen, NULL, NULL) != ERROR_SUCCESS)
	{
		AfxMessageBox(_T("registrer val read failed"));
	}
	
	keyval = new char[longestkeys + 2]{0};
	valname = new WCHAR[longestkeys + 2]{0};
	for (size_t i = 0; i < numkeys; i++)
	{
		datalen = longestkeys + 1;
		vallen = longestkeys + 1;

		if (RegEnumKeyEx(hKey_return, i, valname, &vallen, NULL,
			NULL, NULL, NULL) == ERROR_SUCCESS)
		{
			memset(str, 0, 1024);
			strncpy(str, audioDriver.getcharp(valname), vallen);
			
			asioDriverName.push_back(str);
		}


	}
	delete[] keyval;
	delete[] valname;

	
}

void MYcontrol2::loadASIO(bool binput)
{
	clearCmb();


	readRegsterVal(asiotypeName);

	m_samplebits.SetCurSel(1);
	for (size_t i = 0; i < asiotypeName.size(); i++)
	{

		m_cmbdevicename.InsertString(i, CString(asiotypeName[i].c_str()));
	}
	   
	
}

void MYcontrol2::clearCmb()
{
	clearchCmb();
	int count = m_cmbdevicename.GetCount();

	for (size_t i = 0; i < count; i++)
	{
		m_cmbdevicename.DeleteString(i);
	}
	m_cmbdevicename.SetWindowTextW(_T(""));
	count = m_cmbdevicename.GetCount();
	while (count>0)
	{
		for (size_t i = 0; i < count; i++)
		{
			m_cmbdevicename.DeleteString(i);
		}
		count = m_cmbdevicename.GetCount();
	}
}

void MYcontrol2::clearchCmb()
{
	int count = m_cmbChs.GetCount();

	for (size_t i = 0; i < count; i++)
	{
		m_cmbChs.DeleteString(i);
	}
	m_cmbChs.SetWindowTextW(_T(""));
	count = m_cmbChs.GetCount();
	while (count>0)
	{
		for (size_t i = 0; i < count; i++)
		{
			m_cmbChs.DeleteString(i);
		}
		count = m_cmbChs.GetCount();
	}
	
}


void MYcontrol2::OnCbnSelchangemycontroldevicename2()
{
	// TODO:  在此添加控件通知处理程序代码
	
	int count = m_cmbdevicename.GetCurSel();
	
	clearchCmb();
	count = m_cmbChs.GetCount();
	
	if (m_cmbDriverName.GetCurSel()==0)
	{
		if (wdmChName.size() == 0)
		{
			//AfxMessageBox(_T("检查设备是否存在..."));
			return;
		}
		if (wdmChName[count] == 2)
		{
			/*m_cmbChs.InsertString(0, _T("R"));
			m_cmbChs.InsertString(1, _T("L"));
			m_cmbChs.InsertString(2, _T("Mono"));*/
			
			m_cmbChs.InsertString(0, _T("Mono"));
			m_cmbChs.InsertString(1, _T("Stereo"));
			count = m_cmbdevicename.GetCurSel();
			m_cmbdevicename.GetLBText(count, m_factoryname);

			//设置wdm默认设备
			//audioDriver.setWDMplaych(count, m_factoryname,m_isinput);
		}
		m_cmbChs.SetCurSel(0);
	}
	else
	{

		//根据选择的不同厂家的驱动
		count = m_cmbdevicename.GetCurSel();
		if (count == -1)
		{
			m_cmbChs.SetCurSel(-1);
		}
		else
		{
			m_factoryname = "";
			m_cmbdevicename.GetLBText(count, m_factoryname);

			if (m_factoryname == "Rstech Rscard" ||
				m_factoryname == "Rstech Rscarddio Device")
			{
				audioDriver.LoadRSASIO(wdmDeviceName, asioChName, m_isinput, 0);
			}
			else if (m_factoryname == "Yamaha Steinberg USB ASIO")
			{
				audioDriver.LoadRSASIO(wdmDeviceName, asioChName, m_isinput, 1);
			}
			else if (m_factoryname == "LISSIG USB Audio Device")
			{
				audioDriver.LoadRSASIO(wdmDeviceName, asioChName, m_isinput, 2);
			}
			else if (m_factoryname == "ASIO Fireface USB")
			{
				audioDriver.LoadRSASIO(wdmDeviceName, asioChName, m_isinput, 3);
			}

			if (audioDriver.GetLastErrMsg().IsEmpty())
			{
				for (size_t i = 0; i < asioChName.size(); i++)
				{
					m_cmbChs.InsertString(i, CString(asioChName[i].c_str()));
				}

				m_cmbChs.SetCurSel(0);
			}
			else
			{
				AfxMessageBox(getChsName() + CString(_T("通道 ")) + audioDriver.GetLastErrMsg());
			}
		}
	}

	UpdateData();

	if (devicemanage.isInitFinish())
	{
		CString sText(_T(""));
		m_cmbdevicename.GetLBText(m_cmbdevicename.GetCurSel(), sText);
		EditChannelDev(_T("device"), sText, m_id, m_isinput);
		EditChannelDev(_T("index"), IntToStr(getchsIndex()), m_id, m_isinput);
	}
	
}

CString MYcontrol2::getInnerName()
{
	if (this->m_hWnd == NULL)
		return CString("");

	UpdateData();
	return m_innerDevicename;
}

CString MYcontrol2::getDriverType()
{
	if (this->m_hWnd == NULL)
		return CString("");
	
	UpdateData();
	return m_drivertype;
}

CString MYcontrol2::getdeviceChsName()
{
	if (this->m_hWnd == NULL)
		return CString("");

	UpdateData();

	CString str;
	int index=m_cmbChs.GetCurSel();
	if (index>=0)
	   m_cmbChs.GetLBText(index, str);
	return str;
}

CString MYcontrol2::getChsName()
{
	if (this->m_hWnd == NULL)
		return CString("");

	UpdateData();
	return m_chsname;
}

double MYcontrol2::getFs()
{
	if (this->m_hWnd)
	{
		UpdateData();

		return fs;
	}

	return 0;

}

double MYcontrol2::getVpValues()
{
	if (this->m_hWnd)
	{
		UpdateData();

		return vp;
	}

	return 0;

}

void MYcontrol2::setVpVal(double vpval)
{
	vp = vpval;
	if (this->m_hWnd)
	{
		UpdateData(FALSE);
	}
}
//1 1.2
// vpp/sqrt(2)=vrms
//vpp=sqrt(2)*vrms
//rme 0.25 0.366
//0.366/0.25=1.464 vp=1.464
//
void MYcontrol2::playASIO(double fs,double v)
{
	if (m_bVP)
	{
		jilisetnew.data.clear();
		int playlen = fs * 5;
		for (size_t i = 0; i <playlen; i++)
		{

			jilisetnew.data.push_back(0.1*pow(2, 31)*sqrt(2)*sin(2.0*3.1415926 * 1000 * i / fs));
		}
	}
	

	int ch_index = m_cmbChs.GetCurSel();
	int* index = new int[ch_index+1];
	for (size_t i = 0; i < ch_index+1; i++)
	{
		index[i] = i;
	}
	double recordtime = 1.0*jilisetnew.data.size() / fs;
	size_t len = fs*recordtime;
	if (jilisetnew.data.size() == 0)
	{
		AfxMessageBox(_T("激励信号未设置！"));
		return;
	}

	if (len != jilisetnew.data.size())
	{
		len = jilisetnew.data.size();
	}

	int** playdat = new int*[8];
	
	playdat[0] = new int[len];
	playdat[1] = new int[len];
	playdat[2] = new int[len];
	playdat[3] = new int[len];
	playdat[4] = new int[len];
	playdat[5] = new int[len];
	playdat[6] = new int[len];
	playdat[7] = new int[len];
	
	double ss = pow(2, 31);
	for (size_t t = 0; t < ch_index + 1; t++)
	{
		for (size_t i = 0; i < jilisetnew.data.size(); i++)
		{

			playdat[t][i] = jilisetnew.data[i];

		}

	}
	
	/*playdat[0] = data;*/
	/*for (size_t i = 0; i < fs*4; i++)
	{
		data2[i] = 1.0 * sqrt(2) * ss * sin(2.0*3.1415926 * 4000 * i / fs);
	}
	playdat[1] = data2;*/
	audioDriver.playASIO(index, playdat, ch_index+1, fs);

	/*delete[] data;
	delete[] data2;*/
	delete[] playdat[0];
	delete[] playdat;
	delete[] index;
}

void MYcontrol2::playASIO2(double fs, int asioIndex)
{
	int ch_index = asioIndex;
	int* index = new int[ch_index + 1];
	for (size_t i = 0; i < ch_index + 1; i++)
	{
		index[i] = i;
	}
	double recordtime = 1.0*jilisetnew.data.size() / fs;
	size_t len = fs*recordtime;
	if (jilisetnew.data.size() == 0)
	{
		AfxMessageBox(_T("激励信号未设置！"));
		return;
	}

	if (len != jilisetnew.data.size())
	{
		len = jilisetnew.data.size();
	}

	int** playdat = new int*[8];

	playdat[0] = new int[len];
	playdat[1] = new int[len];
	playdat[2] = new int[len];
	playdat[3] = new int[len];
	playdat[4] = new int[len];
	playdat[5] = new int[len];
	playdat[6] = new int[len];
	playdat[7] = new int[len];

	double ss = pow(2, 31);
	for (size_t t = 0; t < ch_index + 1; t++)
	{
		for (size_t i = 0; i < jilisetnew.data.size(); i++)
		{

			playdat[t][i] = jilisetnew.data[i];

		}

	}

	/*playdat[0] = data;*/
	/*for (size_t i = 0; i < fs*4; i++)
	{
	data2[i] = 1.0 * sqrt(2) * ss * sin(2.0*3.1415926 * 4000 * i / fs);
	}
	playdat[1] = data2;*/
	audioDriver.playASIO(index, playdat, ch_index + 1, fs);

	/*delete[] data;
	delete[] data2;*/
	delete[] playdat[0];
	delete[] playdat;
	delete[] index;
}


/*****************播放+采集 自由选择通道******************************/
void MYcontrol2::recordASIO(double fs, int*** data, double& recordtime, int* playchs, int* recordchs)
{


	if (m_bVP)
	{
		jilisetnew.data.clear();
		int playlen = fs*recordtime;
		for (size_t i = 0; i <playlen; i++)
		{

			jilisetnew.data.push_back(0.1*pow(2, 31)*sqrt(2) / cal_vp*sin(2.0*3.1415926 * 1000 * i / fs));
		}
	}
	


	int len1 = jilisetnew.data.size();
	int buling = fs*0.05;
	//len1 -= 2 * buling;
	vector<int> data_play;
	for (size_t i = 0; i < len1; i++)
	{
		data_play.push_back(jilisetnew.data[i]);
	}

	for (size_t i = 0; i < buling; i++)
	{
		data_play.push_back(0);
	}
	
	 recordtime = 1.0*data_play.size() / fs;
	
	//int index[] = { 0,1};
	/*int* del = NULL;
	int size = 0;*/

	//int index[] = { 0, 1 };
	 size_t len = data_play.size();
	int** playdat = new int*[8];
	
	/*int* data1 = new int[len]{0};
	int* data2 = new int[len]{0};
	int* data3 = new int[len]{0};
	int* data4 = new int[len]{0};
	
	int* data5 = new int[len]{0};
	int* data6 = new int[len]{0};
	
	int* data7 = new int[len]{0};
	int* data8 = new int[len]{0};*/

	if (len != data_play.size())
	{
		len = jilisetnew.data.size();
		//recordtime = ceil(recordtime);
	}
	for (size_t i = 0; i < 8; i++)
	{
		playdat[i] = new int[len]{0};
	}
	
	
	for (size_t i = 0; i < fs*recordtime; i++)
	{
		//data1[i] =0.5 *sqrt(2)* 1000000000 * sin(2.0*3.1415926 * 1000 * i / fs);
	}

	//给1通道播放激励信号
	
	for (size_t i = 0; i < data_play.size(); i++)
	{
		playdat[0][i] = data_play[i];
	}
	for (size_t i = 0; i < data_play.size(); i++)
	{
		playdat[1][i] = data_play[i];
	}
	//playdat[1] = data2;
	

	for (size_t i = 0; i < data_play.size(); i++)
	{
		playdat[2][i] = data_play[i];
	}
	

	for (size_t i = 0; i < data_play.size(); i++)
	{
		playdat[3][i] = data_play[i];
	}
	

	for (size_t i = 0; i < data_play.size(); i++)
	{
		playdat[4][i] = data_play[i];
	}
	

	for (size_t i = 0; i < data_play.size(); i++)
	{
		playdat[5][i] = data_play[i];
	}
	

	for (size_t i = 0; i < data_play.size(); i++)
	{
		playdat[6][i] = data_play[i];
	}
	

	for (size_t i = 0; i < data_play.size(); i++)
	{
		playdat[7][i] = data_play[i];
	}
	

	
	//int* playchs = new int[1]{0};
	//playchs[0] = 0;
	audioDriver.setplaydata(playdat, _msize(playchs)/4, playchs);
	
	//这里默认录音只有一个通道
	
	audioDriver.recordASIO(recordchs, data, _msize(recordchs)/4, fs, recordtime);

	
	
	/**************内存回收处理*********************/
	for (size_t i = 0; i < 8; i++)
	{
		if (len>0&&playdat[i] != NULL)
		{
			delete playdat[i];
			playdat[i] = NULL;
		}
		
	}
	delete playdat;

}

void MYcontrol2::micCalibrationRecordASIO(double fs, int*** data, int recordtime)
{
	int index[] = { m_cmbChs.GetCurSel() };
	audioDriver.recordASIO(index, data, 1, fs, recordtime);


}

void MYcontrol2::setCalVp(double vp)
{
	cal_vp = vp;
}
double MYcontrol2::getCalVp()
{
	return cal_vp;
}


void MYcontrol2::micCalibrationRecordASIO2(double fs, int*** data, double recordtime, int asioIndex)
{
	int index[] = { asioIndex };
	audioDriver.setplaydata(NULL, 0, NULL);
	audioDriver.recordASIO(index, data, 1, fs, recordtime);


}

void MYcontrol2::setASIOStop(bool bstop)
{
	audioDriver.setASIOStop(bstop);
}

void MYcontrol2::micCalibrationRecordWDM(double fs, int*** data, int recordtime)
{

}

void MYcontrol2::recordWDM(double time, char* filename)
{
	int index = 0;// m_cmbdevicename.GetCurSel();
	audioDriver.openRecordThread(time, index);
	
	
}

void MYcontrol2::playWDM(double time, char* filename, CString playChsName)
{
	//int index = m_cmbdevicename.GetCurSel();
	//time = jilisetnew.getPlayTimes();
	
	audioDriver.play(CString(filename), time, 0, playChsName);

	

}
void MYcontrol2::playWDM2(double time, char* filename)
{
	int index = 0;
	time = 0;

	audioDriver.play(CString(filename), time, index, m_factoryname);



}

void  MYcontrol2::playMME()
{
	audioDriver.playMME(2,this);
}

void MYcontrol2::recordMME(double recordTimes, void* obj, int chs, int bits, DWORD fs, int i)
{
	audioDriver.recordMME(recordTimes, NULL, chs, bits, fs, i);
}

void MYcontrol2::startMM()
{
	audioDriver.startMM();
}

void MYcontrol2::MMEinit(int n, double time, const vector<MMERecordParams>& mmesparam, int nIndex)
{
	audioDriver.MMEinit(n, time, mmesparam, nIndex);
}
void MYcontrol2::saveMMEFile(double time)
{
	audioDriver.saveMMEFile(fs, getMMEchs(), getMMEbits(), time);
}

void MYcontrol2::createMMEbuffer(int n)
{
	audioDriver.createMMEbuffer(n);
}

void MYcontrol2::stopRe()
{
	audioDriver.stopRe();
}

int MYcontrol2::getchsIndex()
{

	if (m_drivertype == "ASIO")
	{
		return m_cmbdevicename.GetCurSel();
	}

	int devid = -1;
	CString s;
	m_cmbdevicename.GetLBText(m_cmbdevicename.GetCurSel(), s);	

	if (m_isinput)
	{
		int ndev = waveOutGetNumDevs();
		WAVEINCAPSW pic;
		for (size_t i = 0; i < ndev; i++)
		{
			waveInGetDevCaps(i, &pic, sizeof(WAVEINCAPSW));
			if (CString(pic.szPname) == s)
			{
				devid = i;
				break;

			}
		}
	}
	else
	{
		int ndev = waveOutGetNumDevs();
		WAVEOUTCAPSW pic;
		for (size_t i = 0; i < ndev; i++)
		{
			waveOutGetDevCaps(i, &pic, sizeof(WAVEOUTCAPSW));
			if (CString(pic.szPname) == s)
			{
				devid = i;
				break;

			}
		}
	}
	

	if (devid == -1)
		AfxMessageBox(_T("未找到WDM通道..."));
	return devid;
	
	//return m_cmbdevicename.GetCurSel();
}
CString MYcontrol2::getChsName_Show()
{
	return m_factoryname;
}

int MYcontrol2::getMMEchs()
{
	if (m_drivertype == "WDM")
	{
		if (m_cmbChs.GetCurSel() == 0)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	
	return -1;
}

int MYcontrol2::getMMEdevID()
{
	return getchsIndex();
}
int MYcontrol2::getMMEbits()
{

	int ret = 0;
	switch (m_samplebits.GetCurSel())
	{

	case 0:
		ret = 16;
		break;
	case 1:
		ret = 24;
		break;
	case 2:
		ret = 32;
		break;
	} 


	return ret;

}

//播放与录音的通道在程序中设定
void MYcontrol2::wdmplayWithRecord(int time, char* filename, int index_out, int index_in, CString inChsName,double fs)
{

	audioDriver.wdmplayAndRecord(time, index_in, CString(filename), index_out, inChsName,fs);

}

void MYcontrol2::mmeplayWithRecord(int playIndex)
{
	audioDriver.mmeplayAndRecord(fs, getMMEbits(), m_cmbdevicename.GetCurSel(), playIndex,cal_vp);
	
}

void MYcontrol2::playMME(int* data, int playIndex)
{
	
	audioDriver.playMME(data, fs, getMMEchs(), getMMEbits(), playIndex, cal_vp);
}

bool MYcontrol2::wdmisdone()
{
	return audioDriver.wdmisRecordDone();
}

int MYcontrol2::getASIOchIndex()
{
	if (m_hWnd == NULL)
		return -1;
	return m_cmbChs.GetCurSel();
}

void MYcontrol2::delMMEbuffer()
{
	audioDriver.delMMEbuffer();
}


void MYcontrol2::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	CDialogEx::OnClose();
}


BOOL MYcontrol2::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
		
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void MYcontrol2::OnEnChangemycontrolchname()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码

	//playrecordset.OnCmbDropDown();
	UpdateData();

	if (devicemanage.isInitFinish())
	{
		EditChannelDev(_T("name"), m_chsname, m_id, m_isinput);
	}
}

void MYcontrol2::updatePlayAndRecordCmb()
{
	//playrecordset.OnCmbDropDown();
}

void MYcontrol2::setWDMChs(int index)
{
	//int index2 = m_cmbdevicename.GetCurSel();
	//audioDriver.setWDMplaych(index2);
}

void MYcontrol2::setwdmindex_out(CString name)
{
	audioDriver.setwdmindex_out( name);
}
void MYcontrol2::setwdmName_in(CString name)
{
	audioDriver.setwdmName_in(name);
}


void MYcontrol2::OnCbnEditchangemycontrolchnames()
{
	// TODO:  在此添加控件通知处理程序代码
	if (!m_isinput)
	{
		//更新激励
		if (jilisetnew.m_hWnd)
		{
			jilisetnew.OnBnClickedButtonUpdate();
		}
	}

	if (devicemanage.isInitFinish())
	{
		CString sValue(_T(""));
		sValue.Format(_T("%.3f"), vp);
		EditChannelDev(_T("vp"), sValue, m_id, m_isinput);
	}
}

void MYcontrol2::setMainf(CMainFrame* mf)
{
	audioDriver.setMainf(mf);
}


void MYcontrol2::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()


}

void MYcontrol2::generateV(double v, double f)
{
	jilisetnew.GenerateV(v,f);
}


void MYcontrol2::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值



	CDialogEx::OnMouseMove(nFlags, point);
}


void MYcontrol2::OnMouseHover(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	char str[300];

	sprintf(str, "x=%d,y=%d\n", point.x, point.y);

	//if (abs(point.x - 700) <= 2 && abs(point.y - 1500) <= 10)
	{
		HCURSOR hCur = LoadCursor(NULL, IDC_SIZEWE);

		::SetCursor(hCur);
	}

	OutputDebugString(CString(str));

	CDialogEx::OnMouseHover(nFlags, point);
}

double MYcontrol2::getSen()
{
	if (this->m_hWnd)
	{
		UpdateData();

		return sen;
	}

	return 0;
}

void MYcontrol2::SetDriverSelected(int nIndex)
{
	m_cmbDriverName.SetCurSel(nIndex);
}

bool MYcontrol2::cal_vpByMME(double fs, double outvp)
{
	
	int mmeindex = getchsIndex();
	int chs = m_cmbChs.GetCurSel();
	int bit = m_samplebits.GetCurSel();
	if (bit == 0)
	{
		bit = 16;

	}
	else if (bit == 1)
	{
		bit =24;
	}
	else if (bit == 2)
	{
		bit =32;
	}
	else
	{
		AfxMessageBox(_T("量化位数未选择"));
		return false;
		
	}

	if (chs==0)
	{
		chs = 1;

	}
	else if (chs == 1)
	{
		chs = 2;
	}
	else
	{
		AfxMessageBox(_T("通道未选择!"));
		return false;
	}
	audioDriver.cal_vpByMME(fs, chs, bit, mmeindex, outvp);

	return true;
}


int MYcontrol2::getCMBname(CString name)
{
	int index = -1;

	int count = m_cmbdevicename.GetCount();
	CString lbname;
	for (size_t i = 0; i < count; i++)
	{
		m_cmbdevicename.GetLBText(i, lbname);
		if (lbname == name)
		{
			index = i;
			break;

		}
	}

	if (index == -1)
	{
		index = 0;
		AfxMessageBox(_T("加载WDM通道名称失败..."));
	}
	return index;


}

void MYcontrol2::SetDeviceSelected(CString name)
{
	/*int nIndex = -1;
	CString sText;
	for (int i = 0; i < m_cmbdevicename.GetCount(); i++)
	{
		sText = _T("");
		m_cmbdevicename.GetLBText(i, sText);

		if (StrCmp(lpName, sText) == 0)
		{
			nIndex = i;
			break;
		}
	}*/



	CString sname;
	for (size_t i = 0; i < m_cmbdevicename.GetCount(); i++)
	{
		m_cmbdevicename.GetLBText(i,sname);
		if (sname == name)
		{
			m_cmbdevicename.SetCurSel(i);
			break;
		}
	}

	
	
	UpdateData();

	return;

	UpdateData();
	if (m_drivertype == "ASIO")
	{
//		
		return;
	}
	

	bool bfind = false;
	if (!m_isinput)
	{
		int ndev = waveOutGetNumDevs();
		WAVEOUTCAPSW pic;
		
		for (size_t i = 0; i < ndev; i++)
		{
			waveOutGetDevCaps(i, &pic, sizeof(WAVEOUTCAPSW));
			//if (i == nIndex)
			{
				m_cmbdevicename.SetCurSel(getCMBname(pic.szPname));
				bfind = true;
				break;

			}
		}
	}
	else
	{
		int ndev = waveInGetNumDevs();
		WAVEINCAPSW pic;

		for (size_t i = 0; i < ndev; i++)
		{
			waveInGetDevCaps(i, &pic, sizeof(WAVEINCAPSW));
			//if (i == nIndex)
			{
				m_cmbdevicename.SetCurSel(getCMBname(pic.szPname));
				bfind = true;
				break;

			}
		}
	}

	
	if (!bfind)
		m_cmbdevicename.SetWindowTextW(_T(""));
	
	UpdateColors(FALSE);
}

void MYcontrol2::calSpkASIO(int*** recorddata, int playchs, double fs)
{
	double time = 0;
	int* play = new int[1];
	play[0] = playchs;

	int* recordchs = new int[4];
	recordchs[0] = 0;
	recordchs[1] = 1;
	recordchs[2] = 2;
	recordchs[3] = 3;
	recordASIO(fs, recorddata, time, play, recordchs);

	//内存清理
}

void MYcontrol2::SetChannelSelected(int nIndex)
{
	m_cmbChs.SetCurSel(nIndex);
}

void MYcontrol2::SetSamplingRateVal(double nValue)
{
	int nIndex = -1;
	CString sText;
	for (int i = 0; i < m_chsFs.GetCount(); i++)
	{
		sText = _T("");
		m_chsFs.GetLBText(i, sText);
		if (_ttof(sText) == nValue)
		{
			nIndex = i;
			break;
		}
	}

	m_chsFs.SetCurSel(nIndex);
}

void MYcontrol2::SetBitDepthVal(int nValue)
{
	int nIndex = -1;
	CString sText;
	for (int i = 0; i < m_samplebits.GetCount(); i++)
	{
		sText = _T("");
		m_samplebits.GetLBText(i, sText);
		if (StrToInt(sText) == nValue)
		{
			nIndex = i;
			break;
		}
	}

	m_samplebits.SetCurSel(nIndex);
}

void MYcontrol2::SetSensitivityOrGainVal(double nValue)
{
	sen = nValue;
	if (this->m_hWnd)
	{
		UpdateData(FALSE);
	}
}


void MYcontrol2::OnSelchangeMycontroldevicename()
{
	// TODO:  在此添加控件通知处理程序代码
	if (devicemanage.isInitFinish())
	{
		UpdateData();
		EditChannelDev(_T("channel"), IntToStr(m_cmbChs.GetCurSel()), m_id, m_isinput);
	}
}


//void MYcontrol2::OnEditchangeMycontrolfs()
//{
//	// TODO:  在此添加控件通知处理程序代码
//	
//}

//void MYcontrol2::OnEditchangeMycontrolbits()
//{
//	// TODO:  在此添加控件通知处理程序代码
//}


void MYcontrol2::OnEditchangeMycontrolsenorzengyi()
{
	// TODO:  在此添加控件通知处理程序代码
	if (devicemanage.isInitFinish())
	{
		UpdateData();
		CString sValue(_T(""));
		sValue.Format(_T("%.3f"), getSen());
		if (m_isinput)
		{
			EditChannelDev(_T("sensitivity"), sValue, m_id, m_isinput);
		}
		else
		{
			EditChannelDev(_T("gain"), sValue, m_id, m_isinput);
		}
	}
}


void MYcontrol2::OnSelchangeMycontrolfs()
{
	// TODO:  在此添加控件通知处理程序代码
	if (devicemanage.isInitFinish())
	{
		CString sValue(_T(""));
		sValue.Format(_T("%.3f"), getFs());
		EditChannelDev(_T("sample"), sValue, m_id, m_isinput);
	}
}


void MYcontrol2::OnSelchangeMycontrolbits()
{
	// TODO:  在此添加控件通知处理程序代码
	if (devicemanage.isInitFinish())
	{
		EditChannelDev(_T("bit"), IntToStr(getMMEbits()), m_id, m_isinput);
	}
}


void MYcontrol2::setIsNoPlay(bool bplay)
{
	audioDriver.setIsNoPlay(bplay);
}
