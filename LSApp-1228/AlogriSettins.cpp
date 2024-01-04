// AlogriSettins.cpp : 实现文件
//

#include "stdafx.h"
#include "LSApp.h"
#include "AlogriSettins.h"
#include "afxdialogex.h"



#define ANALYSISCLICK WM_USER+300

extern int globle_w;
extern int globle_h;

// CAlogriSettins 对话框

IMPLEMENT_DYNAMIC(CAlogriSettins, CDialogEx)

CAlogriSettins::CAlogriSettins(CWnd* pParent /*=NULL*/)
	: CDialogEx(CAlogriSettins::IDD, pParent)
{

}

CAlogriSettins::~CAlogriSettins()
{
}

void CAlogriSettins::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_alogrimsettree, m_alogriTree);
	DDX_Control(pDX, IDC_TAB1, m_tab);
	DDX_Control(pDX, IDC_TREEanalysis, m_analysisSelect);
}



BOOL CAlogriSettins::OnInitDialog()
{
	UpdateData(FALSE);//若使用该函数，则需要添加界面更新函数


	hroot = m_alogriTree.InsertItem(_T("算法参数设置"), 0, 0, TVI_ROOT);

	m_alogriTree.InsertItem(_T("FR"), 1, 1, hroot, TVI_LAST);
	m_alogriTree.InsertItem(_T("THD"), 1, 1, hroot, TVI_LAST);
	m_alogriTree.InsertItem(_T("RB"), 1, 1, hroot, TVI_LAST);
	m_alogriTree.InsertItem(_T("PHASE"), 1, 1, hroot, TVI_LAST);
	m_alogriTree.Expand(hroot, TVE_EXPAND);


	hroot = m_analysisSelect.InsertItem(_T("分析项选择"), 0, 0, TVI_ROOT);
	tree_spk= m_analysisSelect.InsertItem(_T("SPK"), 1, 1, hroot, TVI_LAST);
	m_analysisSelect.InsertItem(_T("FR"), 1, 1, tree_spk, TVI_LAST);//1
	m_analysisSelect.InsertItem(_T("THD"), 1, 1, tree_spk, TVI_LAST);
	/*m_analysisSelect.InsertItem(_T("Sensitivity"), 1, 1, tree_spk, TVI_LAST);
	m_analysisSelect.InsertItem(_T("SNR"), 1, 1, tree_spk, TVI_LAST);
	m_analysisSelect.InsertItem(_T("Phase"), 1, 1, tree_spk, TVI_LAST);
	m_analysisSelect.InsertItem(_T("Polarity"), 1, 1, tree_spk, TVI_LAST);
	m_analysisSelect.InsertItem(_T("THD+N"), 1, 1, tree_spk, TVI_LAST);*/
	m_analysisSelect.InsertItem(_T("RB"), 1, 1, tree_spk, TVI_LAST);
	//m_analysisSelect.InsertItem(_T("Balance"), 1, 1, tree_spk, TVI_LAST);
	tree_mic = m_analysisSelect.InsertItem(_T("MIC"), 1, 1, hroot, TVI_LAST);
	m_analysisSelect.InsertItem(_T("FR"), 1, 1, tree_mic, TVI_LAST);
	m_analysisSelect.InsertItem(_T("THD"), 1, 1, tree_mic, TVI_LAST);
	m_analysisSelect.InsertItem(_T("Sensitivity"), 1, 1, tree_mic, TVI_LAST);
	/*m_analysisSelect.InsertItem(_T("SNR"), 1, 1, tree_mic, TVI_LAST);
	m_analysisSelect.InsertItem(_T("Phase"), 1, 1, tree_mic, TVI_LAST);
	m_analysisSelect.InsertItem(_T("THD+N"), 1, 1, tree_mic, TVI_LAST);
	m_analysisSelect.InsertItem(_T("RB"), 1, 1, tree_mic, TVI_LAST);
	m_analysisSelect.InsertItem(_T("Noise Reduction"), 1, 1, tree_mic, TVI_LAST);*/
	

	m_analysisSelect.Expand(hroot, TVE_EXPAND);
	m_analysisSelect.Expand(tree_spk, TVE_EXPAND);
	m_analysisSelect.Expand(tree_mic, TVE_EXPAND);




	frset.Create(CFrSettings::IDD, &m_tab);
	thd.Create(CThdSettings::IDD, &m_tab);
	rb.Create(CRbSettings::IDD, &m_tab);
	//窗口设置
	/*thd.setMincycles();*/

	CRect rc;
	m_tab.GetClientRect(rc);
	//rc.top += 25;
	//rc.bottom -= 5;
	//rc.left += 5;
	//rc.right -= 5;
	frset.MoveWindow(&rc);
	thd.MoveWindow(&rc);
	rb.MoveWindow(&rc);


	frset.ShowWindow(FALSE);
	thd.ShowWindow(TRUE);
	rb.ShowWindow(FALSE);


	//m_alogriTree.SetBkColor(RGB(248, 248, 255));
	

	return TRUE;
}


BEGIN_MESSAGE_MAP(CAlogriSettins, CDialogEx)
	ON_NOTIFY(NM_CLICK, IDC_alogrimsettree, &CAlogriSettins::OnNMClickTree1)
	ON_NOTIFY(NM_CLICK, IDC_TREEanalysis, &CAlogriSettins::OnNMClickTreeanalysis)
	ON_NOTIFY(NM_SETFOCUS, IDC_TREEanalysis, &CAlogriSettins::OnNMSetfocusTreeanalysis)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREEanalysis, &CAlogriSettins::OnTvnEndlabeleditTreeanalysis)
	ON_MESSAGE(ANALYSISCLICK, &CAlogriSettins::OnanalysisClick)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CAlogriSettins 消息处理程序


void CAlogriSettins::OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码

	CTreeCtrl* ctreectrl = (CTreeCtrl*)GetDlgItem(IDC_alogrimsettree);

	CPoint pt = GetCurrentMessage()->pt;

	ctreectrl->ScreenToClient(&pt);

	UINT uFlags = 0;

	HTREEITEM hItem = ctreectrl->HitTest(pt, &uFlags);

    CString alogriName=	m_alogriTree.GetItemText(hItem);


	//这里根据点击的节点，切换对应的设置界面
	if (alogriName == "FR")
	{
		frset.ShowWindow(TRUE);
		rb.ShowWindow(FALSE);
		thd.ShowWindow(FALSE);
		
	}
	else if (alogriName == "THD")
	{
		frset.ShowWindow(FALSE);
		rb.ShowWindow(FALSE);
		thd.ShowWindow(TRUE);
		thd.OnBnClickedButtonSaveAlogrithmThdData();
		

	}
	else if (alogriName == "RB")
	{
		frset.ShowWindow(FALSE);
		thd.ShowWindow(FALSE);
		rb.ShowWindow(TRUE);
		rb.OnBnClickedButtonSave();

	}
	else if (alogriName == "PHASE")
	{

	}

	*pResult = 0;
}

ThdData CAlogriSettins::getThdData()
{
	return thd.getdata();
}

RBdata CAlogriSettins::getRbData()
{
	return rb.getdata();
}


HWND CAlogriSettins::GettreeH()
{
	

	return m_analysisSelect.m_hWnd;
}

void CAlogriSettins::OnNMClickTreeanalysis(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码
	int x, y,h,span;
	h = (80 - 58) / (1800.0 / globle_h);//第一个选择项的top buttom
	span = (85 - 80) / (1800.0 / globle_h);
	vector<int> anlgitems = { 1, 2, 3, 4,5,6,7,8,9 };
	CTreeCtrl* ctreectrl = (CTreeCtrl*)GetDlgItem(IDC_TREEanalysis);

	CPoint pt = GetCurrentMessage()->pt;

	ctreectrl->ScreenToClient(&pt);

	UINT uFlags = 0;
	x = pt.x;
	y = pt.y;
	HTREEITEM hItem = ctreectrl->HitTest(pt, &uFlags);

	CString alogriName = m_alogriTree.GetItemText(hItem);

	HTREEITEM hChild = m_analysisSelect.GetChildItem(hroot);


	hChild = m_analysisSelect.GetChildItem(tree_spk);

	
	while (hChild != NULL)
	{
		BOOL IsCkeck=m_analysisSelect.GetCheck(hChild);
		
		alogriName = m_analysisSelect.GetItemText(hChild);
		
		if (IsCkeck)
		{
			
			
			if (alogriName=="FR")
			   addanalysis(&analysis_spk, anlgitems[0]);
			else if ((alogriName == "THD"))
			{
				addanalysis(&analysis_spk, anlgitems[1]);
			}
			else if ((alogriName == "Sensitivity"))
			{
				addanalysis(&analysis_spk, anlgitems[2]);
			}
			else if ((alogriName == "SNR"))
			{
				addanalysis(&analysis_spk, anlgitems[3]);
			}
			else if ((alogriName == "Phase"))
			{
				addanalysis(&analysis_spk, anlgitems[4]);
			}
			else if ((alogriName == "Polarity"))
			{
				addanalysis(&analysis_spk, anlgitems[5]);
			}
			else if ((alogriName == "THD+N"))
			{
				addanalysis(&analysis_spk, anlgitems[6]);
			}
			else if ((alogriName == "RB"))
			{
				addanalysis(&analysis_spk, anlgitems[7]);
			}
			else if ((alogriName == "Balance"))
			{
				addanalysis(&analysis_spk, anlgitems[8]);
			}

		}
		else
		{
			
			if (alogriName == "FR")
				delanalysis(&analysis_spk, anlgitems[0]);
			else if ((alogriName == "THD"))
			{
				delanalysis(&analysis_spk, anlgitems[1]);
			}
			else if ((alogriName == "Sensitivity"))
			{
				delanalysis(&analysis_spk, anlgitems[2]);
			}
			else if ((alogriName == "SNR"))
			{
				delanalysis(&analysis_spk, anlgitems[3]);
			}
			else if ((alogriName == "Phase"))
			{
				delanalysis(&analysis_spk, anlgitems[4]);
			}
			else if ((alogriName == "Polarity"))
			{
				delanalysis(&analysis_spk, anlgitems[5]);
			}
			else if ((alogriName == "THD+N"))
			{
				delanalysis(&analysis_spk, anlgitems[6]);
			}
			else if ((alogriName == "RB"))
			{
				delanalysis(&analysis_spk, anlgitems[7]);
			}
			else if ((alogriName == "Balance"))
			{
				delanalysis(&analysis_spk, anlgitems[8]);
			}
			
		}
		hChild = m_analysisSelect.GetNextSiblingItem(hChild);
	}


	return;

	/********************以下代码暂不使用**********************************/

	if (x >= 42 / (2880.0 / globle_w) && x <= 64 / (2880.0 / globle_w) && y >= 29 / (1800.0 / globle_h) && y <= 52 / (1800.0 / globle_h))
	{
		BOOL isCheck = m_analysisSelect.GetCheck(tree_spk);
		updateAnalysisAll_spk(isCheck == 1 ? 0 : 1, &anlgitems);
	}


	
	int x1 = 60.0;
	int x2 = 86.0;

	int y1 = 80;
	int y2 = 58;
	/****************************SPK****************************************/
	//FR_
	if (x >= x1 && x <= x2 && y >= y2 && y <= y1)
	{
		updateAnalysisStatus(hItem, 1,false);
	}

	if (x >= x1 && x <= x2 && y >= (y1 + span) && y <= (y1 + span + h))
	{
		updateAnalysisStatus(hItem, 2, false);
	}

	if (x >= x1 && x <= x2 && y >= (y1 + span * 2 + h) && y <= (y1 + (span + h) * 2))
	{
		updateAnalysisStatus(hItem, 3, false);
	}

	if (x >= x1 && x <= x2 && y >= (y1 + span * 3 + h * 2) && y <= (y1 + (span + h) * 3))
	{
		updateAnalysisStatus(hItem, 4, false);
	}

	if (x >= x1 && x <= x2 && y >= (y1 + span * 4 + h * 3) && y <= (y1 + (span + h) * 4))
	{
		updateAnalysisStatus(hItem, 5, false);//phase
	}

	if (x >= x1 && x <= x2 && y >= (y1 + span * 5 + h * 4) && y <= (y1 + (span + h) * 5))
	{
		updateAnalysisStatus(hItem, 6, false);
	}

	if (x >= x1 && x <= x2 && y >= (y1 + span * 6 + h * 5) && y <= (y1 + (span + h) * 6))
	{
		updateAnalysisStatus(hItem, 7, false);
	}

	if (x >= x1 && x <= x2 && y >= (y1 + span * 7 + h * 6) && y <= (y1 + (span + h) * 7))
	{
		updateAnalysisStatus(hItem, 8, false);
	}

	if (x >= x1 && x <= x2 && y >= (y1 + span * 8 + h * 7) && y <= (y1 + (span + h) * 8))
	{
		updateAnalysisStatus(hItem, 9, false);
	}

	

	/* x1 = 60;
	 x2 = 86;*/
	int top = 338.0 / (1800.0 / globle_h);
	int buttom = 361.0 / (1800.0 / globle_h);
	/****************************MIC****************************************/
	//FR_
	 if (x >= x1 && x <= x2 && y >= top && y <= buttom)
	{
		updateAnalysisStatus(hItem, 1, true);
	}

	 if (x >= x1 && x <= x2 && y >= (buttom + span) && y <= (buttom + span + h))
	{
		 updateAnalysisStatus(hItem, 2, true);
	}

	 if (x >= x1 && x <= x2 && y >= (buttom + span * 2 + h) && y <= (buttom + (span + h) * 2))
	{
		 updateAnalysisStatus(hItem, 3, true);
	}

	 if (x >= x1 && x <= x2 && y >= (buttom + span * 3 + h * 2) && y <= (buttom + (span + h) * 3))
	{
		 updateAnalysisStatus(hItem, 4, true);
	}

	 if (x >= x1 && x <= x2 && y >= (buttom + span * 4 + h * 3) && y <= (buttom + (span + h) * 4))
	{
		 updateAnalysisStatus(hItem, 5, true);//phase
	}

	 if (x >= x1 && x <= x2 && y >= (buttom + span * 5 + h * 4) && y <= (buttom + (span + h) * 5))
	{
		 updateAnalysisStatus(hItem, 6, true);
	}

	 if (x >= x1 && x <= x2 && y >= (buttom + span * 6 + h * 5) && y <= (buttom + (span + h) * 6))
	{
		 updateAnalysisStatus(hItem, 7, true);
	}

	 if (x >= x1 && x <= x2 && y >= (buttom + span * 7 + h * 6) && y <= (buttom + (span + h) * 7))
	{
		 updateAnalysisStatus(hItem, 8, true);
	}

	 /*if (x >= x1 && x <= x2 && y >= (buttom + span * 8 + h * 7) && y <= (buttom + (span + h) * 8))
	{
		 updateAnalysisStatus(hItem, 9, true);
	}*/

	
	

	*pResult = 0;
}

vector<int>* CAlogriSettins::getAnalysisData_mic()
{
	return &analysis_mic;
}
vector<int>* CAlogriSettins::getAnalysisData_spk()
{
	return &analysis_spk;
}
void CAlogriSettins::updateAnalysisAll_mic(BOOL IsCkeck, const vector<int>* items)
{
	
	HTREEITEM hChild = m_analysisSelect.GetChildItem(tree_mic);
	while (hChild != NULL)
	{
		m_analysisSelect.SetCheck(hChild, IsCkeck);
		hChild = m_analysisSelect.GetNextSiblingItem(hChild);
		if (IsCkeck)
		{
			for (size_t i = 0; i < items->size(); i++)
			{
				addanalysis(&analysis_mic, items->at(i));
			}
			
		}
		else
		{
			for (size_t i = 0; i < items->size(); i++)
			{
				delanalysis(&analysis_mic, items->at(i));
			}
		}
	}
}

void CAlogriSettins::updateAnalysisAll_spk(BOOL IsCkeck, const vector<int>* items)
{

	HTREEITEM hChild = m_analysisSelect.GetChildItem(tree_spk);
	while (hChild != NULL)
	{
		m_analysisSelect.SetCheck(hChild, IsCkeck);
		hChild = m_analysisSelect.GetNextSiblingItem(hChild);
		if (IsCkeck)
		{
			for (size_t i = 0; i < items->size(); i++)
			{
				addanalysis(&analysis_spk, items->at(i));
			}

		}
		else
		{
			for (size_t i = 0; i < items->size(); i++)
			{
				delanalysis(&analysis_spk, items->at(i));
			}
		}
	}
}

void CAlogriSettins::delanalysis(vector<int>* data, int element)
{
	vector<int>::iterator it;

	for (it = data->begin(); it !=data->end(); it++)
	{
		if (*it == element)
		{
			data->erase(it);
			break;
		}
	}
}

void CAlogriSettins::addanalysis(vector<int>* data, int element)
{
	vector<int>::iterator it;
	bool isadd = true;
	for (it = data->begin(); it != data->end(); it++)
	{
		if (*it == element)
		{
			isadd = false;
			break;
		}
	}

	if (isadd)
		data->push_back(element);

}


void CAlogriSettins::OnNMSetfocusTreeanalysis(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO:  在此添加控件通知处理程序代码



	CTreeCtrl* ctreectrl = (CTreeCtrl*)GetDlgItem(IDC_TREEanalysis);

	CPoint pt = GetCurrentMessage()->pt;

	ctreectrl->ScreenToClient(&pt);

	UINT uFlags = 0;

	HTREEITEM hItem = ctreectrl->HitTest(pt, &uFlags);

	CString alogriName = m_alogriTree.GetItemText(hItem);

	*pResult = 0;
}


void CAlogriSettins::OnTvnEndlabeleditTreeanalysis(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
}

LRESULT CAlogriSettins::OnanalysisClick(WPARAM wPara, LPARAM lPara)
{

	

	return 0;
}

void CAlogriSettins::updateAnalysisStatus(HTREEITEM item, int index, bool ismic)
{
	 
	 BOOL isCheck = m_analysisSelect.GetCheck(item);


	 if (ismic)
	 {

		 if (!isCheck)
		 {

			 addanalysis(&analysis_mic, index);

		 }

		 else
		 {
			 delanalysis(&analysis_mic, index);
		 }

		 m_analysisSelect.SetCheck(tree_mic, 0);
	 }
	 else
	 {
		 if (!isCheck)
		 {

			 addanalysis(&analysis_spk, index);

		 }

		 else
		 {
			 delanalysis(&analysis_spk, index);
		 }

		 m_analysisSelect.SetCheck(tree_spk, 0);
	 }

	
	
}


void CAlogriSettins::OnBnClickedButtonsaveAnalysis()
{
	// TODO:  在此添加控件通知处理程序代码
	
	vector<int> anlgitems = {1,2,3,4};
	HTREEITEM hChild= m_analysisSelect.GetChildItem(hroot);
	
	CString text;
	while (hChild!=NULL)
	{
		text = m_analysisSelect.GetItemText(hChild);

		if (text == "分析项选择")
		{
			BOOL isCheck = m_analysisSelect.GetCheck(hChild);
			updateAnalysisAll_mic(isCheck, &anlgitems);
		}
		else
		{
			/*if (text == "FR_")
			{
				updateAnalysisStatus(hChild, 1);
			}

			if (text == "THD_")
			{
				updateAnalysisStatus(hChild, 2);
			}

			if (text == "THD+N_")
			{
				updateAnalysisStatus(hChild, 3);
			}

			if (text == "RB_")
			{
				updateAnalysisStatus(hChild, 4);
			}*/
		}
		




		hChild=m_analysisSelect.GetNextSiblingItem(hChild);
		

	}


	//::SendMessage(GetSafeHwnd(), ANALYSISCLICK, 0, 0);

}


void CAlogriSettins::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO:  在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialogEx::OnPaint()

	/*CRect rc;
	m_tab.GetWindowRect(rc);
	ScreenToClient(rc);

	dc.FillSolidRect(rc, RGB(248, 248, 255));*/

}
