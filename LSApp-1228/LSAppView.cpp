
// LSAppView.cpp : CLSAppView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "LSApp.h"
#endif

#include "LSAppDoc.h"
#include "LSAppView.h"
#include "resource.h"



extern int m_docId;
extern vector<int> docIdList;


/*************全局对象*****************/
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
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CLSAppView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_CREATE()
	ON_WM_CLOSE()
END_MESSAGE_MAP()

// CLSAppView 构造/析构

CLSAppView::CLSAppView()
{
	// TODO: 在此处添加构造代码
	
	
	m_onceExcute = false;
}

CLSAppView::~CLSAppView()
{
}

BOOL CLSAppView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CLSAppView 绘制

void CLSAppView::OnDraw(CDC* dc /*pDC*/)
{
	CLSAppDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	
	//进度条隐藏
	/*if (!m_onceExcute)
	{
		if (pDoc->GetTitle() == "蓝牙耳机测试")
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
		
		
	
	


	//dc->TextOutW(100, 100, _T("声学测试系统V1.0...."));

	/*if(pDoc->GetTitle()=="瑞森声卡设置")
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
		
		

	}else if(pDoc->GetTitle()=="一体箱隔音测试")
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
	else if (pDoc->GetTitle() == "激励设置")
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
	else if (pDoc->GetTitle() == "蓝牙串口调试")
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
	else if (pDoc->GetTitle() == "设备校准")
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
	else if (pDoc->GetTitle() == "灵敏度测试_C公司")
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
	else if (pDoc->GetTitle() == "灵敏度测试")
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
	
	
	// TODO: 在此处为本机数据添加绘制代码
}

//绑定界面
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

	//进度条隐藏
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
	return 1;//这里暂时屏蔽，树节点切换功能
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

			  if (doc->GetTitle() == "蓝牙串口调试")
			  {
				  if (docIdList.back() != m_docId&&m_docId==6)
				  {
					  doc->OnCloseDocument();
					  m_docId = 6;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else /*if (doc->GetTitle() == "激励设置")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 5)
				  {
					  doc->OnCloseDocument();
					  m_docId = 5;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else */if (doc->GetTitle() == "激励设置")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 10)
				  {
					  doc->OnCloseDocument();
					  m_docId = 10;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "设备管理")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 11)
				  {
					  doc->OnCloseDocument();
					  m_docId = 11;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "仪器校准")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 12)
				  {
					  doc->OnCloseDocument();
					  m_docId = 12;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "上下限设置")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 13)
				  {
					  doc->OnCloseDocument();
					  m_docId = 13;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "算法设置")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 14)
				  {
					  doc->OnCloseDocument();
					  m_docId = 14;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "播放与采集设置")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 15)
				  {
					  doc->OnCloseDocument();
					  m_docId = 15;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "设备校准")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 3)
				  {
					  doc->OnCloseDocument();
					  m_docId = 3;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "瑞森声卡设置")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 0)
				  {
					  doc->OnCloseDocument();
					  m_docId = 0;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "一体箱隔音测试")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 4)
				  {
					  doc->OnCloseDocument();
					  m_docId = 4;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "灵敏度测试_C公司")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 2)
				  {
					  doc->OnCloseDocument();
					  m_docId = 2;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "采集卡(4022)测试")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 8)
				  {
					  doc->OnCloseDocument();
					  m_docId = 8;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "灵敏度测试")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 1)
				  {
					  doc->OnCloseDocument();
					  m_docId = 1;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "RME声卡测试")
			  {
				  if (docIdList.back() != m_docId&&m_docId == 7)
				  {
					  doc->OnCloseDocument();
					  m_docId = 7;
					  AfxGetApp()->m_pDocManager->OnFileNew();
				  }
			  }
			  else if (doc->GetTitle() == "蓝牙耳机测试")
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


// CLSAppView 打印


void CLSAppView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CLSAppView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CLSAppView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CLSAppView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
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


// CLSAppView 诊断

#ifdef _DEBUG
void CLSAppView::AssertValid() const
{
	CView::AssertValid();
}

void CLSAppView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CLSAppDoc* CLSAppView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CLSAppDoc)));
	return (CLSAppDoc*)m_pDocument;
}
#endif //_DEBUG


// CLSAppView 消息处理程序
