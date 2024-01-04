
// LSAppView.cpp : CLSAppView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "LSApp.h"
#endif

#include "LSAppDoc.h"
#include "LSAppView.h"
#include "resource.h"



extern int m_docId;
extern vector<int> docIdList;


/*************ȫ�ֶ���*****************/
MyDlg m_dlg;
CLmd lmd;
CDeviceSet deviceSet;
CGeyin geyin;
CJiLiSet jiliset;
CBlueComm blueComm;
TestRME testRME;
CBlueTestD bluetestD;
CBlueEreTest blueEreTest;
JiLiSetNewVersion jilisetnew;
CDeviceManage devicemanage;
CCalibration calibration;
CTestBorderLineSettings testborderline;
CAlogriSettins  alogriSettins;
CPlayRecordSettings playrecordset;



#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CLSAppView

IMPLEMENT_DYNCREATE(CLSAppView, CView)

BEGIN_MESSAGE_MAP(CLSAppView, CView)
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLSAppView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CLSAppView ����/����

CLSAppView::CLSAppView()
{
	// TODO: �ڴ˴���ӹ������
	
	
	m_onceExcute = false;
}

CLSAppView::~CLSAppView()
{
}

BOOL CLSAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ

	return CView::PreCreateWindow(cs);
}

// CLSAppView ����

void CLSAppView::OnDraw(CDC* dc /*pDC*/)
{
	CLSAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	
	//����������
	/*if (!m_onceExcute)
	{
		if (pDoc->GetTitle() == "������������")
		{
			if (blueEreTest.m_hWnd)
			{
				blueEreTest.HideProcess(false);
			}
		}
		else
		{
			if (blueEreTest.m_hWnd)
			{
				blueEreTest.HideProcess(true);
			}
		}
	}*/
		
		
	
	


	//dc->TextOutW(100, 100, _T("��ѧ����ϵͳV1.0...."));

	/*if(pDoc->GetTitle()=="��ɭ��������")
	{
		for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		{
			if (*var == 0)
			{
				docIdList.erase(var);
				docIdList.push_back(0);
				break;
			}
		}
		
		

	}else if(pDoc->GetTitle()=="һ�����������")
	{
		for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		{
			if (*var == 4 )
			{
				docIdList.erase(var);
				docIdList.push_back(4);
				break;
			}
		}
		
	    
	}
	else if (pDoc->GetTitle() == "��������")
	{

		for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		{
			if (*var == 5 )
			{
				docIdList.erase(var);
				docIdList.push_back(5);
				break;
			}
		}
		
		
	}
	else if (pDoc->GetTitle() == "�������ڵ���")
	{

		for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		{
			if (*var == 6 )
			{
				docIdList.erase(var);
				docIdList.push_back(6);
				break;
			}
		}
		


	}
	else if (pDoc->GetTitle() == "�豸У׼")
	{

		for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		{
			if (*var == 3)
			{
				docIdList.erase(var);
				docIdList.push_back(3);
				break;
			}
		}
		


	}
	else if (pDoc->GetTitle() == "�����Ȳ���_C��˾")
	{

		for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		{
			if (*var == 2)
			{
				docIdList.erase(var);
				docIdList.push_back(2);
				break;
			}
		}
		
	}
	else if (pDoc->GetTitle() == "�����Ȳ���")
	{

		for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		{
			if (*var == 1)
			{
				docIdList.erase(var);
				docIdList.push_back(1);
				break;
			}
		}
		


	
	}*/
	
	
	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
}

//�󶨽���
int CLSAppView:: OnCreate(LPCREATESTRUCT lpCreatestruct)
{
	WriteDebugLog(_T("LSAppView Create Start"));
	if(CView::OnCreate(lpCreatestruct)==-1)
	{
		return -1;
	}
	/*CRect rect(500,500,200,100);
	m_button1.Create(_T("test"),WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,rect,this,IDB_BITMAP_BUTTON1);
	m_button1.ShowWindow(SW_SHOW);*/

	/*CFormDlg frm;
	frm.ShowWindow(SW_SHOW);*/

	//����������
	m_onceExcute = true;
	/*if (m_docId==9)
	{
		if (blueEreTest.m_hWnd)
		{
			blueEreTest.HideProcess(false);
		}
	}
	else
	{
		if (blueEreTest.m_hWnd)
		{
			blueEreTest.HideProcess(true);
		}
	}*/


	int w = GetSystemMetrics(SM_CXSCREEN);
	int h = GetSystemMetrics(SM_CYSCREEN);

	if (m_docId == 0 && m_dlg.m_hWnd==NULL)
	{
		
	   m_dlg.Create(MyDlg::IDD,this);
	   m_dlg.ShowWindow(SW_HIDE);
	   //lmd.DestroyWindow();
	}
	else if (m_docId == 1 && lmd.m_hWnd == NULL)
	{

		lmd.Create(CLmd::IDD,this);
		lmd.ShowWindow(SW_SHOW);
		//m_dlg.DestroyWindow();
	
	}
	else if (m_docId == 2 && lmd.m_hWnd == NULL)
	{

		lmd.Create(CLmd::IDD,this);
		lmd.ShowWindow(SW_SHOW);
		//m_dlg.DestroyWindow();
	
	}
	else if (m_docId == 3 && deviceSet.m_hWnd == NULL)
	{
	
		deviceSet.Create(CDeviceSet::IDD,this);
		deviceSet.upudatedata();
		deviceSet.ShowWindow(SW_SHOW);
		
		//m_dlg.DestroyWindow();
	    
	}
	else if (m_docId == 4 && geyin.m_hWnd == NULL)
	{
	
		geyin.Create(CGeyin::IDD,this);
		geyin.ShowWindow(SW_SHOW);
		//m_dlg.DestroyWindow();
	    
	}
	else if (m_docId == 5 && jiliset.m_hWnd == NULL)
	{
	    
		jiliset.Create(CJiLiSet::IDD,this);
		jiliset.ShowWindow(SW_SHOW);
		
	    
	}
	else if (m_docId == 6 && blueComm.m_hWnd == NULL)
	{

		blueComm.Create(CBlueComm::IDD, this);
		blueComm.ShowWindow(SW_SHOW);


	}
	else if (m_docId == 7 && testRME.m_hWnd == NULL)
	{

		testRME.Create(TestRME::IDD, this);
		testRME.ShowWindow(SW_SHOW);


	}
	else if (m_docId == 8 && bluetestD.m_hWnd == NULL)
	{

		bluetestD.Create(CBlueTestD::IDD, this);
		bluetestD.ShowWindow(SW_SHOW);


	}
	else if (m_docId == 9 && blueEreTest.m_hWnd == NULL)
	{
		
		blueEreTest.Create(CBlueEreTest::IDD, this);
		blueEreTest.MoveWindow(0, 0, w, h);
		blueEreTest.ShowWindow(SW_SHOW);


	}
	else if (m_docId == 10 && jilisetnew.m_hWnd == NULL)
	{

		jilisetnew.Create(JiLiSetNewVersion::IDD, this);
		jilisetnew.MoveWindow(0, 0, w, h);
		jilisetnew.ShowWindow(SW_SHOW);


	}
	else if (m_docId == 11 && devicemanage.m_hWnd == NULL)
	{

		devicemanage.Create(CDeviceManage::IDD, this);
		devicemanage.MoveWindow(0, 0, w, h);
		devicemanage.ShowWindow(SW_SHOW);

		



	}
	else if (m_docId == 12 && calibration.m_hWnd == NULL)
	{

		calibration.Create(CCalibration::IDD, this);
		calibration.MoveWindow(0, 0, w, h);
		calibration.ShowWindow(SW_SHOW);


	}
	else if (m_docId == 13 && testborderline.m_hWnd == NULL)
	{

		testborderline.Create(CTestBorderLineSettings::IDD, this);
		testborderline.MoveWindow(0, 0, w, h);
		testborderline.ShowWindow(SW_SHOW);
		
		


	}
	else if (m_docId == 14 && alogriSettins.m_hWnd == NULL)
	{
		alogriSettins.Create(CAlogriSettins::IDD, this);
		alogriSettins.MoveWindow(0, 0, w, h);
		alogriSettins.ShowWindow(SW_SHOW);
	}
	else if (m_docId == 15 && playrecordset.m_hWnd == NULL)
	{
		playrecordset.Create(CPlayRecordSettings::IDD, this);
		playrecordset.MoveWindow(0, 0, w, h);
		playrecordset.ShowWindow(SW_SHOW);
	}
	//else if (m_docId == 1301 && testborderline.m_hWnd == NULL)
	//{
	//	
	//	
	//	/*testborderline.showItem(0, this);*/
	//	

	//}
	m_onceExcute = false;
	return 1;//������ʱ���Σ����ڵ��л�����
	CMDIFrameWnd* pFrame = (CMDIFrameWnd*)AfxGetApp()->m_pMainWnd;
	//if (blueComm.m_hWnd != NULL)
	{	
		//::SendMessage(pFrame->m_hWndMDIClient, WM_MDIACTIVATE, (WPARAM)blueComm.GetParent()->GetParent()->GetSafeHwnd(), 0);
		//int count=AfxGetApp()->m_pDocManager->GetOpenDocumentCount();

		POSITION pos = theApp.GetFirstDocTemplatePosition();

		while (pos!=NULL)
		{
			CDocTemplate* doctemp = theApp.GetNextDocTemplate(pos);
			POSITION posdoc =doctemp->GetFirstDocPosition();

			while (posdoc!=NULL)
			{

			  CDocument* doc=doctemp->GetNextDoc(posdoc);

			  if (doc->GetTitle() == "�������ڵ���")
			  {
				  if (docIdList.back() != m_docId&&m_docId==6)
				  {
					  doc->OnCloseDocument();
					  m_docId = 6;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else /*if (doc->GetTitle() == "��������")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 5)
				  {
					  doc->OnCloseDocument();
					  m_docId = 5;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else */if (doc->GetTitle() == "��������")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 10)
				  {
					  doc->OnCloseDocument();
					  m_docId = 10;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "�豸����")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 11)
				  {
					  doc->OnCloseDocument();
					  m_docId = 11;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "����У׼")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 12)
				  {
					  doc->OnCloseDocument();
					  m_docId = 12;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "����������")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 13)
				  {
					  doc->OnCloseDocument();
					  m_docId = 13;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "�㷨����")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 14)
				  {
					  doc->OnCloseDocument();
					  m_docId = 14;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "������ɼ�����")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 15)
				  {
					  doc->OnCloseDocument();
					  m_docId = 15;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "�豸У׼")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 3)
				  {
					  doc->OnCloseDocument();
					  m_docId = 3;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "��ɭ��������")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 0)
				  {
					  doc->OnCloseDocument();
					  m_docId = 0;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "һ�����������")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 4)
				  {
					  doc->OnCloseDocument();
					  m_docId = 4;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "�����Ȳ���_C��˾")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 2)
				  {
					  doc->OnCloseDocument();
					  m_docId = 2;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "�ɼ���(4022)����")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 8)
				  {
					  doc->OnCloseDocument();
					  m_docId = 8;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "�����Ȳ���")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 1)
				  {
					  doc->OnCloseDocument();
					  m_docId = 1;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "RME��������")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 7)
				  {
					  doc->OnCloseDocument();
					  m_docId = 7;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "������������")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 9)
				  {
					  doc->OnCloseDocument();
					  m_docId = 9;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }


			}
		  

		}
	}

	
	WriteDebugLog(_T("LSAppView Create Finish"));

	return 1;
	
    
	

}


// CLSAppView ��ӡ


void CLSAppView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLSAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CLSAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CLSAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}

void CLSAppView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CLSAppView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CLSAppView ���

#ifdef _DEBUG
void CLSAppView::AssertValid() const
{
	CView::AssertValid();
}

void CLSAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLSAppDoc* CLSAppView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLSAppDoc)));
	return (CLSAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CLSAppView ��Ϣ�������
