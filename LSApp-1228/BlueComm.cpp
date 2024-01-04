// BlueComm.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "LSApp.h"
#include "BlueComm.h"
#include "afxdialogex.h"



// CBlueComm �Ի���

IMPLEMENT_DYNAMIC(CBlueComm, CDialogEx)

CBlueComm::CBlueComm(CWnd* pParent /*=NULL*/)
: CDialogEx(CBlueComm::IDD, pParent), index(0)
{

}

CBlueComm::~CBlueComm()
{
}

void CBlueComm::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, mlistBox);
	DDX_Control(pDX, IDC_CmbPort, m_port);
}

BOOL CBlueComm::OnInitDialog()
{
	UpdateData(0);
	m_port.InsertString(0, _T("COM1"));
	m_port.InsertString(1, _T("COM2"));
	m_port.InsertString(2, _T("COM3"));
	m_port.InsertString(3, _T("COM4"));
	m_port.InsertString(4, _T("COM5"));
	m_port.SetCurSel(2);


	CImage image;
	HRESULT res =-1;
	res=image.Load(_T("./res/bg.jpg"));

	CBitmap bitmap;
	bitmap.Attach(image.Detach());
	brush.DeleteObject();
	brush.CreatePatternBrush(&bitmap);

	return TRUE;
}

BEGIN_MESSAGE_MAP(CBlueComm, CDialogEx)
	ON_BN_CLICKED(IDC_BtnOpen, &CBlueComm::OnBnClickedBtnopen)
	ON_BN_CLICKED(IDC_BtnClose, &CBlueComm::OnBnClickedBtnclose)
	ON_BN_CLICKED(IDC_BUTTON1, &CBlueComm::OnBnClickedButtonDisconn)
	ON_BN_CLICKED(IDC_BUTTON2, &CBlueComm::OnBnClickedButtonConn)
	ON_BN_CLICKED(IDC_BUTTON3, &CBlueComm::OnBnClickedButtonSerchBlue)
	ON_BN_CLICKED(IDC_BUTTON4, &CBlueComm::OnBnClickedButtonStopSerch)
	ON_BN_CLICKED(IDC_BUTTON5, &CBlueComm::OnBnClickedButtonClearScreen)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON6, &CBlueComm::OnBnClickedButtonSetVol)
	ON_BN_CLICKED(IDC_BUTTON7, &CBlueComm::OnBnClickedButtonModeSwatch)
	ON_BN_CLICKED(IDC_BUTTON8, &CBlueComm::OnBnClickedButtonDeviceStatus)
	ON_BN_CLICKED(IDC_BtnSend, &CBlueComm::OnBnClickedBtnsend)
END_MESSAGE_MAP()


// CBlueComm ��Ϣ�������
HANDLE hcom;
char blueID[12];
DWORD WINAPI recive(LPVOID param)
{
	CBlueComm* com = (CBlueComm*)param;
	char buff[1024]{0};
	COMSTAT comstat;
	DWORD err,aclen,len=0;
	int i = 0;
	char bluedevice[16]{0};
	while (hcom!=NULL)
	{
		ClearCommError(hcom, &err, &comstat);
		len = comstat.cbInQue>1204 ? 1024 : comstat.cbInQue;
		ReadFile(hcom, buff, len, &aclen, NULL);
		
		CString ss(buff);
		//com->mlistBox.InsertString(i++, ss);
		if ((len = strlen(buff)) > 0)
		{
			memset(bluedevice, 0, 7);
			memcpy(bluedevice, buff, 7);
			if (strcmp(bluedevice, "DEVICE=") == 0 || (bluedevice[0]=='D'&&
				bluedevice[0] == 'E'&&bluedevice[0] == 'V' && bluedevice[0] == 'I' && bluedevice[0] == 'C'
				&& bluedevice[0] == 'E'))
			{
				memset(bluedevice, 0, 7);
				memcpy(bluedevice, buff + 7, 12);
				memcpy(blueID, bluedevice, 12);
			}

		

			//for (size_t i = 0; i < 100000; i++)
			{
				if (com->m_hWnd)
				{
					com->mlistBox.InsertString(com->index, ss);	
					com->index++;
				}
				
				
			}
			
			if (com->m_hWnd)
			     com->mlistBox.SetWindowTextW(ss);
			com->setMode(ss);

			

		}
		
		memset(buff, 0, len);

		Sleep(50);

	}

	return 0;
}

void CBlueComm::setMode(CString s)
{
	m_mode = s;
}

bool CBlueComm::opencomm(CString portName)
{
	
	DCB dcb;


	vector<HANDLE>::iterator it = find(list_coms.begin(), list_coms.end(), hcom);
	if (it != list_coms.end())
	{
		//AfxMessageBox(_T("��Ҫ�ظ��򿪣�"));
		return true;
	}

	
	

	hcom = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);//FILE_FLAG_OVERLAPPED
	COMSTAT comstat;
	DWORD err;
	ClearCommError(hcom, &err, &comstat);

	if (err != 0)
	{
		//AfxMessageBox(_T("���ڴ�ʧ��!"));
		hcom = NULL;
		return false;
	}
	BOOL b = GetCommState(hcom, &dcb);
	dcb.BaudRate = 921600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = 0;//0,1,2===>1,1.5,2 ʹ�ú�Ķ�Ӧ��ϵ��ȷ��ֹͣλ����

	
	b = SetCommState(hcom, &dcb);
	

	if (b == 0)
	{
		AfxMessageBox(_T("��������ʧ�ܣ�"));
		hcom = NULL;
		return false;
	}
	list_coms.push_back(hcom);
	//CreateThread(NULL, 0, recive, this, 0, NULL);
	return true;
}

void CBlueComm::clearcms()
{
	
}

void CBlueComm::OnBnClickedBtnopen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	UpdateData(1);
	DCB dcb;
	
	

	if (hcom != 0)
	{
		AfxMessageBox(_T("��Ҫ�ظ��򿪣�"));
		return;
	}

	CString portName;
	m_port.GetWindowTextW(portName);

	hcom = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);//FILE_FLAG_OVERLAPPED
	COMSTAT comstat;
	DWORD err;
	ClearCommError(hcom, &err, &comstat);

	if (err != 0)
	{
		AfxMessageBox(_T("���ڴ�ʧ��!"));
		hcom = NULL;
		return;
	}
	BOOL b = GetCommState(hcom, &dcb);
	//dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = 921600;
	dcb.ByteSize = 8;
	dcb.Parity = NOPARITY;
	dcb.StopBits = 0;//0,1,2===>1,1.5,2 ʹ�ú�Ķ�Ӧ��ϵ��ȷ��ֹͣλ����

	//����
	/*dcb.fInX = FALSE;
	dcb.fOutX = FALSE;
	dcb.fNull = FALSE;
	dcb.fBinary = TRUE;
	dcb.fParity = FALSE;
	dcb.fOutxCtsFlow = FALSE;
	dcb.fOutxDsrFlow = FALSE;*/
	b = SetCommState(hcom, &dcb);
	//ClearCommError(hcom, &err, &comstat);

	if (b == 0)
	{
		AfxMessageBox(_T("��������ʧ�ܣ�"));
		hcom = NULL;
		return;
	}


	CreateThread(NULL, 0, recive, this, 0, NULL);


	GetDlgItem(IDC_BtnOpen)->SetWindowTextW(_T("�ѿ���"));

	/*char* ss1 = ">DISC";
	memcpy(buff, ss1, strlen(ss));
	bw = WriteFile(hcom, buff, strlen(ss), NULL, NULL);*/
	
//	bw=CloseHandle(hcom);
}


void CBlueComm::OnBnClickedBtnclose()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	
	BOOL bw = CloseHandle(hcom);
	hcom = NULL;
	GetDlgItem(IDC_BtnOpen)->SetWindowTextW(_T("�򿪴���"));
}


void CBlueComm::OnBnClickedButtonDisconn()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	char buff[1024]{0};
	char* ss = ">DISC\r\n";
	memcpy(buff, ss, strlen(ss));
	DWORD  len = 0;
	DWORD tolen = 0;
	tolen = strlen(ss);
	BOOL bw = WriteFile(hcom, buff, tolen, &len, NULL);
}

void CBlueComm::disconnect()
{
	for (size_t i = 0; i < list_coms.size(); i++)
	{
		char buff[1024]{0};
		char* ss = ">DISC\r\n";
		memcpy(buff, ss, strlen(ss));
		DWORD  len = 0;
		DWORD tolen = 0;
		tolen = strlen(ss);
		BOOL bw = WriteFile(list_coms[i], buff, tolen, &len, NULL);
		bw = CloseHandle(list_coms[i]);
		list_coms[i] = NULL;
	}

	list_coms.clear();
	
}


void CBlueComm::OnBnClickedButtonConn()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	char buff[1024]{0};
	char* ss = ">CONN=";
	memcpy(buff, ss, strlen(ss));
	memcpy(buff + strlen(ss), blueID, 12);
	//memcpy(buff + (strlen(ss)+12), "\r\n", 2);
	DWORD  len = 0;
	DWORD tolen;
	tolen = strlen(buff);
	BOOL bw = WriteFile(hcom, buff, tolen, &len, NULL);
}

void CBlueComm::connectRre(char* id)
{
	char buff[1024]{0};
	char* ss = ">CONN=";
	memcpy(buff, ss, strlen(ss));
	memcpy(buff + strlen(ss), id, 12);
	//memcpy(buff + (strlen(ss)+12), "\r\n", 2);
	DWORD  len = 0;
	DWORD tolen;
	tolen = strlen(buff);
	BOOL bw = WriteFile(hcom, buff, tolen, &len, NULL);
	hcom = NULL;
	
	
}

char* CBlueComm::searchEre()
{
	char* id = new char[13]{0};
	char buff[1024]{0};
	COMSTAT comstat;
	char bluedevice[16]{0};
	DWORD err, aclen=0, len = 0;
	if (hcom == NULL)
	{
		AfxMessageBox(_T("����δ�򿪣�"));
		return "";
	}

	char buff1[9]{0};
	char* ss1 = ">SEARCH=5";
	memcpy(buff1, ss1, 9);
	DWORD  len1 = 0;
	DWORD tolen;
	tolen = 9;


	BOOL bw = WriteFile(hcom, buff1, tolen, &len1, NULL);

	FlushFileBuffers(hcom);


	//��ȡ��������,��ȡ�豸id
	
	
	while (aclen>0)
	{//��������ȡ������
		ReadFile(hcom, buff, len, &aclen, NULL);

	}
	int times = 0;
	while (aclen <= 20)
	{
		memset(buff, 0, 1024);
		ClearCommError(hcom, &err, &comstat);
		if (err != 0)
		{

			BOOL bw = CloseHandle(hcom);
			hcom = NULL;
			break;
		}
		len = comstat.cbInQue>1204 ? 1024 : comstat.cbInQue;
		ReadFile(hcom, buff, len, &aclen, NULL);
		Sleep(50);
		times++;
		if (times >300)
			break;
		
	}

	CString ss(buff);
	if ((len = strlen(buff)) > 0)
	{
		memset(bluedevice, 0, 16);
		memcpy(bluedevice, buff, 7);
		if (strcmp(bluedevice, "DEVICE=") == 0 || (bluedevice[0] == 'D'&&
			bluedevice[0] == 'E'&&bluedevice[0] == 'V' && bluedevice[0] == 'I' && bluedevice[0] == 'C'
			&& bluedevice[0] == 'E'))
		{
			memset(bluedevice, 0, 7);
			memcpy(bluedevice, buff + 7, 12);
			memcpy(id, bluedevice, 12);
		}
	}

	

	return id;

}


void CBlueComm::OnBnClickedButtonSerchBlue()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	if (hcom == NULL)
	{
		AfxMessageBox(_T("����δ�򿪣�"));
		return;
	}

	char buff[9]{0};
	char* ss = ">SEARCH=5";
	memcpy(buff, ss, 9);
	DWORD  len = 0;
	DWORD tolen;
	tolen = 9;
	BOOL bw = WriteFile(hcom, buff, tolen, &len, NULL);

	FlushFileBuffers(hcom);
}


void CBlueComm::OnBnClickedButtonStopSerch()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	char buff[1024]{0};
	char* ss = ">SEARCH STOP\r\n";
	memcpy(buff, ss, strlen(ss));
	DWORD  len = 0;
	DWORD tolen;
	tolen = strlen(ss);
	BOOL bw = WriteFile(hcom, buff, tolen, &len, NULL);
}


void CBlueComm::OnBnClickedButtonClearScreen()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	int count=0;

	
	while ((count = mlistBox.GetCount())>0)
	{
		for (size_t i = 0; i < count; i++)
		{
			mlistBox.DeleteString(i);
		}
	}

	index = 0;
}


HBRUSH CBlueComm::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//return brush;
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������

	if (pWnd == this)
		return brush;
	else if (nCtlColor == CTLCOLOR_STATIC)
	{
		//���þ�̬�ı�͸��
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(RGB(245,245,245));
		return HBRUSH(GetStockObject(HOLLOW_BRUSH));
	}
	

	return hbr;
}

void CBlueComm::setIndex(int num)
{
	index = num;
}



void CBlueComm::OnBnClickedButtonSetVol()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	char buff[1024]{0};
	char* ss = ">SET_VOL=15\r\n";
	memcpy(buff, ss, strlen(ss));
	DWORD  len = 0;
	DWORD tolen;
	tolen = strlen(ss);
	BOOL bw = WriteFile(hcom, buff, tolen, &len, NULL);


}

void CBlueComm::getDeviceStatus()
{
	char buff[1024]{0};
	char* ss = ">STATUS";
	memcpy(buff, ss, strlen(ss));
	DWORD  len = 0;
	DWORD tolen;
	tolen = strlen(ss);
	BOOL bw = WriteFile(hcom, buff, tolen, &len, NULL);
	CString msg;
	mlistBox.GetWindowTextW(msg);

}

void CBlueComm::getDeviceMode(CString& str)
{
	char buff[1024]{0};
	char* ss = ">STATUS";
	memcpy(buff, ss, strlen(ss));
	DWORD  len = 0;
	DWORD tolen;
	tolen = strlen(ss);
	BOOL bw = WriteFile(hcom, buff, tolen, &len, NULL);
	
	str = m_mode;
}


void CBlueComm::OnBnClickedButtonModeSwatch()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	CString ss;
	char buff[1024]{0};
	GetDlgItem(IDC_BUTTON7)->GetWindowTextW(ss);
	if ( ss== "¼��(ģʽ)")
	{
		GetDlgItem(IDC_BUTTON7)->SetWindowTextW(_T("����(ģʽ)"));
		//�л�����
		char* ss = ">OPEN A2DP";
		memcpy(buff, ss, strlen(ss));
		DWORD  len = 0;
		DWORD tolen;
		tolen = strlen(ss);
		BOOL bw = WriteFile(hcom, buff, tolen, &len, NULL);


	}
	else
	{
		GetDlgItem(IDC_BUTTON7)->SetWindowTextW(_T("¼��(ģʽ)"));

		//�л���¼��
		char* ss = ">OPEN HFP";
		memcpy(buff, ss, strlen(ss));
		DWORD  len = 0;
		DWORD tolen;
		tolen = strlen(ss);
		BOOL bw = WriteFile(hcom, buff, tolen, &len, NULL);


	}

}

void CBlueComm::switchmode(CString btnstr)
{
	CString ss = btnstr;
	char buff[1024]{0};
	//GetDlgItem(IDC_BUTTON7)->GetWindowTextW(ss);

	for (size_t i = 0; i < list_coms.size(); i++)
	{
		if (ss == "����(SPK)ģʽ")
		{
			//GetDlgItem(IDC_BUTTON7)->SetWindowTextW(_T("����(ģʽ)"));
			//�л�����
			char* ss = ">OPEN A2DP";
			memcpy(buff, ss, strlen(ss));
			DWORD  len = 0;
			DWORD tolen;
			tolen = strlen(ss);
			BOOL bw = WriteFile(list_coms[i], buff, tolen, &len, NULL);


		}
		else
		{
			//GetDlgItem(IDC_BUTTON7)->SetWindowTextW(_T("¼��(ģʽ)"));

			//�л���¼��
			char* ss = ">OPEN HFP";
			memcpy(buff, ss, strlen(ss));
			DWORD  len = 0;
			DWORD tolen;
			tolen = strlen(ss);
			BOOL bw = WriteFile(list_coms[i], buff, tolen, &len, NULL);


		}
	}

	
}


void CBlueComm::OnBnClickedButtonDeviceStatus()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	getDeviceStatus();
}


void CBlueComm::OnBnClickedBtnsend()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������

	CString command;
	GetDlgItem(IDC_SENDCOMMAND)->GetWindowTextW(command);
	USES_CONVERSION;
	char buff[1024]{0};
	char* ss = T2A(command);
	memcpy(buff, ss, strlen(ss));
	DWORD  len = 0;
	DWORD tolen;
	tolen = strlen(ss);
	BOOL bw = WriteFile(hcom, buff, tolen, &len, NULL);


}
