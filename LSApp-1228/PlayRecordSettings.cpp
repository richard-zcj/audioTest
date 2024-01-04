// PlayRecordSettings.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "PlayRecordSettings.h"
#include "afxdialogex.h"

#include "DeviceManage.h"
#include "MyButton.h"
#include "MyComboBox.h"
#include "JiLiSetNewVersion.h"

#define ID_COMBOX WM_USER+2000
#define ID_COMBOX2 28887

#define ID_COMBOXSELCT WM_USER+4000




extern CDeviceManage devicemanage;
extern JiLiSetNewVersion jilisetnew;

extern int globle_w;
extern int globle_h;

// CPlayRecordSettings 对话框

IMPLEMENT_DYNAMIC(CPlayRecordSettings, CDialogEx)

CPlayRecordSettings::CPlayRecordSettings(CWnd* pParent /*=NULL*/)
: CDialogEx(CPlayRecordSettings::IDD, pParent), mlistId(0)
{
	
}

CPlayRecordSettings::~CPlayRecordSettings()
{
}

void CPlayRecordSettings::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST2, mlistPlayRecordItems);
	DDX_Control(pDX, IDC_COMBO1, m_cmbTestModes);
}

BOOL CPlayRecordSettings::OnInitDialog()
{
	UpdateData(FALSE);

	
	HFONT hf = CreateFont(37 / (1800.0 / globle_h), 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体")
		);
	

	mlistPlayRecordItems.SetExtendedStyle(LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);

	CHeaderCtrl* pheadctrl = (CHeaderCtrl*)mlistPlayRecordItems.GetHeaderCtrl();
	pheadctrl->EnableWindow(FALSE);

	m_cmbTestModes.InsertString(0, _T("MIC"));
	m_cmbTestModes.InsertString(1, _T("SPK"));
	m_cmbTestModes.SetCurSel(0);


	/*CImageList im;
	im.Create(1, 70, ILC_COLOR, 10, 10);
	mlistPlayRecordItems.SetImageList(&im,LVSIL_SMALL);*/

	
	

	mlistPlayRecordItems.InsertColumn(0, _T(""), LVCFMT_LEFT, 0);
	mlistPlayRecordItems.InsertColumn(1, _T("播放"), LVCFMT_LEFT, 330/(2880.0/globle_w));
	mlistPlayRecordItems.InsertColumn(2, _T("激励"), LVCFMT_LEFT, 330 / (2880.0 / globle_w));
	mlistPlayRecordItems.InsertColumn(3, _T("采集"), LVCFMT_LEFT, 330 / (2880.0 / globle_w));
	mlistPlayRecordItems.InsertColumn(4, _T("路径"), LVCFMT_LEFT, 330 / (2880.0 / globle_w));

	
	//mlistPlayRecordItems.InsertItem(mlistId, _T("1"));
	//CFont font;
	////font.CreatePointFont(160/(1800.0/globle_h), _T("宋体"));
	////mlistPlayRecordItems.SetFont(&font);
	//mlistPlayRecordItems.SendMessage(WM_SETFONT, (WPARAM)hf, TRUE);
	//mlistPlayRecordItems.SetItemText(mlistId, 1, CString(""));
	//mlistPlayRecordItems.SetItemText(mlistId, 2, CString(""));
	
	
	/*CRect rc;
	for (size_t i = 0; i < mCmblistIn.size(); i++)
	{
		if (mCmblistIn[i]->m_hWnd == NULL)
		{
			
			mCmblistIn[i]->GetClientRect(rc);
			mCmblistIn[i]->Create(mCmblistIn[i]->GetStyle(),rc , &mlistPlayRecordItems,1);
		}
	}

	for (size_t i = 0; i < mCmblistOut.size(); i++)
	{
		if (mCmblistOut[i]->m_hWnd == NULL)
		{
			mCmblistOut[i]->GetClientRect(rc);
			mCmblistOut[i]->Create(mCmblistOut[i]->GetStyle(), rc, &mlistPlayRecordItems, 1);
		}

	}*/

	for (size_t i = 0; i < mCmblistIn.size(); i++)
	{

		if (mCmblistIn[i]->m_hWnd == NULL)
		{
			delete mCmblistIn[i];
		}

	}
	mCmblistIn.clear();

	for (size_t i = 0; i < mCmblistOut.size(); i++)
	{
		if (mCmblistOut[i]->m_hWnd == NULL)
		{
			delete mCmblistOut[i];
		}
	}
	mCmblistOut.clear();

	InitUIData();

	mlistPlayRecordItems.SetTextBkColor(RGB(102, 205, 170));
	mlistPlayRecordItems.SetBkColor(RGB(102, 205, 170));
	mainf = (CMainFrame*)theApp.GetMainWnd();
	return TRUE;
}

void CPlayRecordSettings::InitUIData()
{
	bool bSpk = m_cmbTestModes.GetCurSel() == 1;
	for (int i = 0; i < GetPlayRecordCount(bSpk); i++)
	{
		if ((i + 1) > mCmblistOut.size() &&
			(i + 1) > mCmblistIn.size())
		{

			mlistPlayRecordItems.InsertItem(mlistId, _T("1"));
			mlistPlayRecordItems.SetItemText(mlistId, 1, CString(""));
			mlistPlayRecordItems.SetItemText(mlistId, 2, CString(""));
			mlistPlayRecordItems.SetItemText(mlistId, 3, CString(""));
			mlistPlayRecordItems.SetItemText(mlistId, 4, CString(""));

		}

		

		for (int j = 1; j < 5; j++)
		{
			CComboBox* combx = NULL;
			CEdit * edit = NULL;
			if (j != 4)
			{
				combx = new CComboBox;//需要管理地址
			}
			else
			{
				edit = new CEdit;
			}

			//MyComboBox* combx = new MyComboBox;
			HFONT hf = CreateFont(32 / (1800.0 / globle_h), 0, 0, 0, FW_MEDIUM,
				FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
				DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体")
				);

			CFont font;
			font.CreatePointFont(150, _T("宋体"));

			CRect rc;
			mlistPlayRecordItems.GetSubItemRect(i, j, LVIR_LABEL, rc);

			int nCount;
			if (j == 1)
			{
				nCount = GetDevCount(false);
				if (nCount == 0)
				{
					delete combx;
					combx = NULL;
					mainf->setStatusStr(CString("设备管理模块，输出通道无数据..."));
					return;
				}

				/*vector<CString> temp = devicemanage.getInnerNames_OutCHS();
				if (temp.size() == 0)
				{
					delete combx;
					combx = NULL;
					mainf->setStatusStr(CString("设备管理模块，输出通道无数据..."));
					return;
				}*/

				combx->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, 
					rc, &mlistPlayRecordItems, ID_COMBOX + mlistId);

				combx->SendMessage(WM_SETFONT, (WPARAM)hf, TRUE);
				
				combx->ResetContent();
				for (size_t n = 0; n < nCount; n++)
				{
					combx->InsertString(n, GetDevParam(n, false)->sName);
				}

				combx->SetCurSel(GetPlayRecord(i, bSpk)->nPlayIndex);
				mCmblistOut.push_back(combx);
			}
			else
			{
				/*vector<CString> temp = devicemanage.getInnerNames_InCHS();
				if (temp.size() == 0)
				{
					delete combx;
					combx = NULL;
					mainf->setStatusStr(CString("设备管理模块，输入通道无数据..."));
					return;
				}*/

				nCount = GetDevCount(true);
				if (nCount == 0)
				{
					delete combx;
					combx = NULL;
					mainf->setStatusStr(CString("设备管理模块，输入通道无数据..."));
					return;
				}

				if (j != 4)
				{
					combx->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST,
						rc, &mlistPlayRecordItems, ID_COMBOX2 + mlistId);
					combx->SetFont(&font);
					combx->SendMessage(WM_SETFONT, (WPARAM)hf, TRUE);
				}
				else
				{
					edit->Create(WS_CHILD | WS_VISIBLE, rc, &mlistPlayRecordItems, ID_COMBOX + mlistId);
					edit->SendMessage(WM_SETFONT, (WPARAM)hf, TRUE);
				}


				if (j == 3)
				{
					combx->ResetContent();
					for (size_t n = 0; n < nCount; n++)
					{
						combx->InsertString(n, GetDevParam(n, true)->sName);
					}

					combx->SetCurSel(GetPlayRecord(i, bSpk)->nRecordIndex);
					mCmblistIn.push_back(combx);

				}
				else if (j == 2)
				{
					nCount = GetStimCount();
					if (nCount == 0)
					{
						delete combx;
						combx = NULL;
						mainf->setStatusStr(CString("激励模块，未保存激励无数据..."));
						return;
					}

					//temp = jilisetnew.getJiliname();
					combx->ResetContent();
					for (size_t n = 0; n < nCount; n++)
					{
						combx->InsertString(n, GetStimParam(n)->sName);
					}

					combx->SetCurSel(GetPlayRecord(i, bSpk)->nStimIndex);
					m_jili_list.push_back(combx);
				}

			}

			mlistId++;

			//*pResult = 0;

			Invalidate();
		}
	}
}

BOOL CPlayRecordSettings::PreTranslateMessage(MSG* pMsg)
{
	if (mCmblistIn.size() > 0)
	{
		
		int index = -1;
		
		   if ((index = findComx(mCmblistIn, pMsg->hwnd)) != -1)
			{

			   /* char sg[1024]{0};
			   sprintf(sg, "%d", pMsg->message);
			   mainf->setStatusStr(CString(sg));*/

			 

			   if (pMsg->message == 275)
				{
					//AfxMessageBox(_T("ID_COMBOX"));
					//OnCmbDropDown(true);
				  
				   {
					   OnCmbDropDown2(true, index);
					   
				   }
					
					/*CString cbmtext;
					mCmblistIn[index]->GetWindowTextW(cbmtext);
					if (cbmtext != "")
					{
						OnCmbDropDown3();
					}*/
					
					
					
					::SendMessage(m_hWnd, ID_COMBOXSELCT, 0, 0);
				}
				

				
					
				

			}
			else if ((index=findComx(mCmblistOut, pMsg->hwnd))!=-1)
			{
				if (pMsg->message == 275)
				{
					
					//AfxMessageBox(_T("ID_COMBOX"));
					//OnCmbDropDown(false);
					OnCmbDropDown2(false, index);

				}
			}


			/*if (mlistPlayRecordItems.m_hWnd == pMsg->hwnd)
			{
				char sg[1024]{0};
				sprintf(sg, "%d", pMsg->message);
				mainf->setStatusStr(CString(sg));
				if (pMsg->message == 15)
				{
					return 0;
				}
			}*/

		  /* if (pMsg->message != 675 && pMsg->message != 275
			   && pMsg->message != 512 && pMsg->message != 513 && pMsg->message != 514)*/
		   {
			  /* char sg[1024]{0};
			   sprintf(sg, "%d", pMsg->message);
			   mainf->setStatusStr(CString(sg));*/
		   }
		 
		
		
	}
	
	

	return CDialog::PreTranslateMessage(pMsg);
}

int CPlayRecordSettings::findComx(const vector<CComboBox*>& coblist, HWND wnd)
{
	int index = -1;

	for (size_t i = 0; i < coblist.size(); i++)
	{
		if (coblist[i]->m_hWnd == wnd)
		{
			index = i;
		}
	}

	return index;

}


BEGIN_MESSAGE_MAP(CPlayRecordSettings, CDialogEx)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST2, &CPlayRecordSettings::OnNMDblclkList2)
	ON_MESSAGE(ID_COMBOXSELCT, &CPlayRecordSettings::OnCmbDropDown3)
	/*ON_BN_CLICKED(ID_COMBOX, &CPlayRecordSettings::OnCmbDropDown)*/
	ON_WM_PAINT()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST2, &CPlayRecordSettings::OnLvnItemchangedList2)
	ON_BN_CLICKED(IDC_BUTTON1, &CPlayRecordSettings::OnBnClickedButtonSavedata)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CPlayRecordSettings::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON2, &CPlayRecordSettings::OnBnClickedButton2)
END_MESSAGE_MAP()


// CPlayRecordSettings 消息处理程序


void CPlayRecordSettings::OnNMDblclkList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	
	

	

	CRect rc;
	int row = pNMItemActivate->iItem;
	int col = pNMItemActivate->iSubItem;
	mlistPlayRecordItems.GetSubItemRect(row, col, LVIR_LABEL, rc);
	
	if (row == -1)
	{
		return;
	}

	if ((row+1) > mCmblistOut.size()&&
		(row+1) > mCmblistIn.size())
	{

		mlistPlayRecordItems.InsertItem(mlistId, _T("1"));
		mlistPlayRecordItems.SetItemText(mlistId, 1, CString(""));
		mlistPlayRecordItems.SetItemText(mlistId, 2, CString(""));
		mlistPlayRecordItems.SetItemText(mlistId, 3, CString(""));
		mlistPlayRecordItems.SetItemText(mlistId, 4, CString(""));

	}

	CComboBox* combx=NULL;
	CEdit * edit = NULL;
	if (col!=4)
	    combx = new CComboBox;//需要管理地址
	else
	{
		edit = new CEdit;

	}
	//MyComboBox* combx = new MyComboBox;
	HFONT hf = CreateFont(32/(1800.0/globle_h), 0, 0, 0, FW_MEDIUM, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, _T("宋体")
		);

	CFont font;
	font.CreatePointFont(150, _T("宋体"));
	

	if (col == 1)
	{
		vector<CString> temp = devicemanage.getInnerNames_OutCHS();
		if (temp.size() == 0)
		{
			delete combx;
			combx = NULL;
			mainf->setStatusStr(CString("设备管理模块，输出通道无数据..."));
			return;
		}
		
		
		  combx->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, rc, &mlistPlayRecordItems, ID_COMBOX + mlistId);
		
		
		//combx->SetFont(&font);
		

		combx->SendMessage(WM_SETFONT, (WPARAM)hf, TRUE);
		/*DRAWITEMSTRUCT dis{ 0 };
		dis.CtlType = ODT_COMBOBOX;
		dis.hDC = (HDC)GetDC();
		CRect rc;
		GetWindowRect(rc);
		dis.rcItem = rc;
		combx->DrawItem(&dis);*/

		//combx->Attach(IDC_testcombox, this);

		
		combx->ResetContent();
		for (size_t i = 0; i < temp.size(); i++)
		{
			combx->InsertString(i, temp[i]);
		}
		/*combx->InsertString(0, _T("1"));
		combx->InsertString(1, _T("2"));
		combx->InsertString(2, _T("3"));
		combx->InsertString(3, _T("4"));*/

		
		mCmblistOut.push_back(combx);
	}
	else
	{
		vector<CString> temp = devicemanage.getInnerNames_InCHS();
		if (temp.size() == 0)
		{
			delete combx;
			combx = NULL;
			mainf->setStatusStr(CString("设备管理模块，输入通道无数据..."));
			return;
		}

		if (col != 4)
		{
			combx->Create(WS_CHILD | WS_VISIBLE | WS_VSCROLL | CBS_DROPDOWNLIST, rc, &mlistPlayRecordItems, ID_COMBOX2 + mlistId);
			combx->SetFont(&font);
			combx->SendMessage(WM_SETFONT, (WPARAM)hf, TRUE);
		}
		else
		{
			edit->Create(WS_CHILD | WS_VISIBLE, rc, &mlistPlayRecordItems, ID_COMBOX + mlistId);
			edit->SendMessage(WM_SETFONT, (WPARAM)hf, TRUE);
		}

		
		if (col == 3)
		{
			combx->ResetContent();
			for (size_t i = 0; i < temp.size(); i++)
			{
				combx->InsertString(i, temp[i]);
			}
			/*combx->InsertString(0, _T("1"));
			combx->InsertString(1, _T("2"));
			combx->InsertString(2, _T("3"));
			combx->InsertString(3, _T("4"));*/

			mCmblistIn.push_back(combx);

		}
		else if (col==2)
		{
			temp = jilisetnew.getJiliname();
			combx->ResetContent();
			for (size_t i = 0; i < temp.size(); i++)
			{
				combx->InsertString(i, temp[i]);
			}

			m_jili_list.push_back(combx);
		}
		
	}
	mlistId++;

	*pResult = 0;

	Invalidate();
}

void CPlayRecordSettings::GetPlayRecordInfo(int index,CString& palyChs, CString& jili, CString& recordChs, CString& path)
{
	palyChs = "";
	jili = "";
	recordChs = "";
	path = "";
	if (index < mCmblistIn.size())
	{
		mCmblistIn[index]->GetWindowTextW(recordChs);
	}
	if (index < mCmblistOut.size())
	{
		mCmblistOut[index]->GetWindowTextW(palyChs);
	}
	if (index < m_jili_list.size())
	{
		m_jili_list[index]->GetWindowTextW(jili);
	}
}

LRESULT CPlayRecordSettings::OnCmbDropDown3(WPARAM wParam, LPARAM lParam)
{
	
	
	CString cbmtext;
	mCmblistIn[0]->GetWindowTextW(cbmtext);
	

	return 0;
}

void CPlayRecordSettings::OnCmbDropDown2(bool isIn, int cWnd)
{

	return;
	CString cbmtext;
	

	/*char sg[1024]{0};
	sprintf(sg, "%d", pMsg->message);*/
	//mainf->setStatusStr(cbmtext);




	vector<CString> temp;
	if (isIn)
	{
		mCmblistIn[cWnd]->GetWindowTextW(cbmtext);


		if (cbmtext == "")
		{
			return;
		}

		temp = devicemanage.getInnerNames_InCHS();

		if (mCmblistIn.size()>0)
			m_curIndex_in = mCmblistIn[cWnd]->GetCurSel();
		if (mCmblistOut.size()>0 && cWnd<mCmblistOut.size())
			m_curIndex_out = mCmblistOut[cWnd]->GetCurSel();

		mCmblistIn[cWnd]->ResetContent();

		for (size_t j = 0; j < temp.size(); j++)
		{
			mCmblistIn[cWnd]->InsertString(j, temp[j]);
		}

		/***********************这里比较ASIO 与WDM驱动对应的设备名称，不容易实现暂时搁置********************************/
		//根据输入通道加载输出通道信息  

		//加载法则：如果判断当前选择的输入通道设备
		//输出通道与输入通道设备不同则加载所有驱动的通道
		//如果输出通道的设备相同则加载相同驱动的通道

		temp.clear();
		temp = devicemanage.getInnerNames_OutCHS();
		char* drvername;
		char* drvernameout;
		devicemanage.getDriverType(cbmtext, &drvername);
		//for (size_t i = 0; i < mCmblistOut.size(); i++)
		{
			if (mCmblistOut.size()>cWnd)
			   mCmblistOut[cWnd]->ResetContent();

			for (size_t j = 0; j < temp.size(); j++)
			{
				devicemanage.getDriverType(temp[j], &drvernameout);

				//查看设备名称是否相同

				if (strcmp(drvername, "ASIO") == 0)
				{
					if (strcmp(drvername, drvernameout) == 0)
					{

					}
				}
				else
				{

				}
				
				if (mCmblistOut.size()>cWnd)
				    mCmblistOut[cWnd]->InsertString(j, temp[j]);
			}

		}



		if (mCmblistIn.size()>0)
			mCmblistIn[cWnd]->SetCurSel(m_curIndex_in);

		if (mCmblistOut.size()>0 && cWnd<mCmblistOut.size())
			mCmblistOut[cWnd]->SetCurSel(m_curIndex_out);


	}
	else
	{
		mCmblistOut[cWnd]->GetWindowTextW(cbmtext);


		if (cbmtext == "")
		{
			return;
		}
		
		temp = devicemanage.getInnerNames_OutCHS();

		if (mCmblistOut.size()>0)
			m_curIndex_out = mCmblistOut[cWnd]->GetCurSel();
		if (mCmblistIn.size()>0 && cWnd<mCmblistIn.size())
			m_curIndex_in = mCmblistIn[cWnd]->GetCurSel();

		mCmblistOut[cWnd]->ResetContent();

		for (size_t j = 0; j < temp.size(); j++)
		{
			mCmblistOut[cWnd]->InsertString(j, temp[j]);
		}


		//根据输出通道加载输出通道信息

		//加载法则：如上所述

		temp.clear();
		temp = devicemanage.getInnerNames_InCHS();
		char* drvername;
		char* drvernameout;
		devicemanage.getDriverType(cbmtext, &drvername);
		//for (size_t i = 0; i < mCmblistIn.size(); i++)
		{
			if (mCmblistIn.size()>cWnd)
			   mCmblistIn[cWnd]->ResetContent();

			for (size_t j = 0; j < temp.size(); j++)
			{
				devicemanage.getDriverType(temp[j], &drvernameout);

				//查看设备名称是否相同

				if (strcmp(drvername, "ASIO") == 0)
				{
					if (strcmp(drvername, drvernameout) == 0)
					{

					}
				}
				else
				{

				}
				if (mCmblistIn.size()>cWnd)
				   mCmblistIn[cWnd]->InsertString(j, temp[j]);
			}

		}



		if (mCmblistIn.size()>0 && cWnd<mCmblistIn.size())
			mCmblistIn[cWnd]->SetCurSel(m_curIndex_in);

		if (mCmblistOut.size()>0)
			mCmblistOut[cWnd]->SetCurSel(m_curIndex_out);

	}

	mainf->setStatusStr(cbmtext);
}

void CPlayRecordSettings::OnCmbDropDown(bool isIn)
{
	
	vector<CString> temp = devicemanage.getInnerNames_InCHS();
	if (mCmblistIn.size()>0)
	   m_curIndex_in = mCmblistIn[0]->GetCurSel();
	if (mCmblistOut.size()>0)
	    m_curIndex_out = mCmblistOut[0]->GetCurSel();
	for (size_t i = 0; i < mCmblistIn.size(); i++)
	{
		mCmblistIn[i]->ResetContent();
		
		for (size_t j = 0; j < temp.size(); j++)
		{
			mCmblistIn[i]->InsertString(j, temp[j]);
		}
	}

	temp.clear();
	temp = devicemanage.getInnerNames_OutCHS();

	for (size_t i = 0; i < mCmblistOut.size(); i++)
	{
		mCmblistOut[i]->ResetContent();
		
		for (size_t j = 0; j < temp.size(); j++)
		{
			mCmblistOut[i]->InsertString(j, temp[j]);
		}

	}

	if (mCmblistIn.size()>0)
	    mCmblistIn[0]->SetCurSel(m_curIndex_in);

	if (mCmblistOut.size()>0)
	   mCmblistOut[0]->SetCurSel(m_curIndex_out);

}

void CPlayRecordSettings::GetPlayAndRecord(CString& playname, CString& recordname)
{
	//UpdateData();
	if (this->m_hWnd == NULL)
	{
		return;
	}

	if (mCmblistIn.size()>0)
	{
		//int index = mCmblistIn[0]->GetCurSel();
		mCmblistIn[0]->GetWindowTextW(recordname);
		//mCmblistIn[0]->GetLBText(mCmblistIn[0]->GetCurSel(), recordname);
	}
	if (mCmblistOut.size()>0)
	{
		//int index = mCmblistOut[0]->GetCurSel();
		mCmblistOut[0]->GetWindowTextW(playname);
		//mCmblistOut[0]->GetLBText(mCmblistOut[0]->GetCurSel(), playname);
	}
	/*for (size_t i = 0; i < mCmblistIn.size(); i++)
	{

	}

	for (size_t i = 0; i < mCmblistOut.size(); i++)
	{

	}*/
}





void CPlayRecordSettings::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	
	//mlistPlayRecordItems.SetBkColor(RGB(255, 34, 12));
	//mlistPlayRecordItems.SetTextBkColor(RGB(102, 205, 170));

	//mlistPlayRecordItems.SetBkColor(RGB(102, 205, 170));
	//mlistPlayRecordItems.SetBkColor(RGB(0, 0, 0));
	

	/*CDC* dc2 = mlistPlayRecordItems.GetDC();

	int olddc=dc.SaveDC();
	CBrush brush;
	CPen pen;
	pen.CreatePen(0, 1, RGB(255, 0, 0));
	brush.CreateSolidBrush(RGB(255, 0, 0));
	dc.SelectObject(pen);
*/
	

	/*CRect rc;
	GetDlgItem(IDC_LIST2)->GetWindowRect(rc);
	ScreenToClient(rc);
	dc.FillSolidRect(rc, RGB(0, 0, 0));
	dc.FillPath();*/
	
	//drawGrid(mlistPlayRecordItems.GetDC());

	/*char sg[1024]{0};
	sprintf(sg, "%s", "onpain");
	mainf->setStatusStr(CString(sg));*/
	
}

void CPlayRecordSettings::drawGrid(CDC* dc)
{
	CRect rc;
	GetDlgItem(IDC_LIST2)->GetWindowRect(rc);
	ScreenToClient(rc);
	//dc = GetDlgItem(IDC_LIST2)->GetDC();
	dc->MoveTo(rc.left, rc.top+30);
	dc->LineTo(rc.right, rc.top+50);

	dc->FillSolidRect(rc, RGB(255, 34, 12));
	dc->FillPath();

	dc->MoveTo(200, 400);
	dc->LineTo(800, 800);

	MoveToEx(*dc, 0, 800,NULL);
	LineTo(*dc, 1200, 800);

}


void CPlayRecordSettings::OnLvnItemchangedList2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

void CPlayRecordSettings::OnBnClickedButtonSavedata()
{
	// TODO:  在此添加控件通知处理程序代码
	PlayRecordDialogData* ss = NULL;
	
	if (m_cmbTestModes.GetCurSel() == 0)
	{
		for (size_t i = 0; i < m_playrecorddata.size(); i++)
		{
			delete m_playrecorddata[i];

		}
		m_playrecorddata.clear();
	}
	else
	{
		for (size_t i = 0; i < m_playrecorddata2.size(); i++)
		{
			delete m_playrecorddata2[i];

		}
		m_playrecorddata2.clear();
	}
	for (size_t i = 0; i < mlistPlayRecordItems.GetItemCount() - 1; i++)
	{
		ss = new PlayRecordDialogData;

		 if (i < mCmblistIn.size())
		 {
			 mCmblistIn[i]->GetWindowTextW(ss->record);
		 }
		 if (i < mCmblistOut.size())
		 {
			 mCmblistOut[i]->GetWindowTextW(ss->play);
		 }
		 if (i < m_jili_list.size())
		 {
			 m_jili_list[i]->GetWindowTextW(ss->jili);
		 }

		 if (m_cmbTestModes.GetCurSel() == 0)
		 {
			 ss->mode = 0;
			 m_playrecorddata.push_back(ss);
		 }
		 else
		 {
			 ss->mode = 1;
			 m_playrecorddata2.push_back(ss);
		 }
	}
}

vector<PlayRecordDialogData*>* CPlayRecordSettings::getPlayRecordData()
{
	return &m_playrecorddata;
}

vector<PlayRecordDialogData*>* CPlayRecordSettings::getSpkPlayRecordData()
{
	return &m_playrecorddata2;
}


void CPlayRecordSettings::OnCbnSelchangeCombo1()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData();
	InitUIData();
}


void CPlayRecordSettings::OnBnClickedButton2()
{
	// TODO:  在此添加控件通知处理程序代码
	bool bSpk = true;
	
	for (int n = 0; n < 2; n++)
	{
		if (n == 1)
		{
			bSpk = false;
		}

		int nCount = GetPlayRecordCount(bSpk);
		for (int i = 0; i < nCount; i++)
		{
			CalcStimData(i, bSpk);
		}
	}
}
