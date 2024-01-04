
// LSAppDoc.cpp : CLSAppDoc 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "LSApp.h"
#include "JiLiSet.h"
#include "BlueComm.h"
#include "DeviceManage.h"
#include "BlueEreTest.h"
#endif

#include "LSAppDoc.h"
#include <propkey.h>
#include "globle.h"

#include "MyDlg.h"
#include "MainFrm.h"

extern CJiLiSet jiliset;
extern CBlueComm blueComm;
extern CDeviceManage devicemanage;
extern CBlueEreTest blueEreTest;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLSAppDoc

IMPLEMENT_DYNCREATE(CLSAppDoc, CDocument)

	
	
	
BEGIN_MESSAGE_MAP(CLSAppDoc, CDocument)
	ON_COMMAND(WM_KEYDOWN,SetDocTitile)
	
END_MESSAGE_MAP()


 vector<int> docIdList;
 bool bExtDoc;
// CLSAppDoc 构造/析构

CLSAppDoc::CLSAppDoc()
{

	bExtDoc=false;
	// TODO: 在此添加一次性构造代码

}

CLSAppDoc::~CLSAppDoc()
{
}

BOOL CLSAppDoc::OnNewDocument()
{

	((CMainFrame*)theApp.GetMainWnd())->setBackGroundShow(false);

	if((find(begin(docIdList),end(docIdList), m_docId)!=end(docIdList)))
    {
		         bExtDoc=true;
				 return FALSE;
	}
	
	/*if((find(begin(docIdList),end(docIdList), 0)!=end(docIdList)))
	{
	    int res=AfxMessageBox(_T("确认修改吗？"),MB_OKCANCEL);
		if(res==2)
			return TRUE;
	}*/
	

	if (!CDocument::OnNewDocument())
		return FALSE;


	

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)
	if(m_docId==0)
	{
		
		this->SetTitle(_T("瑞森声卡设置"));//
		

	}
	else if(m_docId==1)
    {
	   this->SetTitle(_T("灵敏度测试"));
	}else if(m_docId==2)
    {
	   this->SetTitle(_T("灵敏度测试_C公司"));
	}else if(m_docId==3)
	{
	   this->SetTitle(_T("设备校准"));
	}else if(m_docId==4)
	{
	   this->SetTitle(_T("一体箱隔音测试"));
	}else if(m_docId==5)
	{
	   //this->SetTitle(_T("激励设置"));
	}
	else if (m_docId == 6)
	{
		this->SetTitle(_T("蓝牙串口调试"));
	}
	else if (m_docId == 7)
	{
		this->SetTitle(_T("RME声卡测试"));
	}
	else if (m_docId == 8)
	{
		this->SetTitle(_T("采集卡(4022)测试"));
	}
	else if (m_docId == 9)
	{
		this->SetTitle(_T("蓝牙耳机测试"));
	}
	else if (m_docId == 10)
	{
		this->SetTitle(_T("激励设置"));
	}
	else if (m_docId == 11)
	{
		this->SetTitle(_T("设备管理"));
	}
	else if (m_docId == 12)
	{
		this->SetTitle(_T("仪器校准"));
	}
	else if (m_docId == 13 )
	{
		this->SetTitle(_T("上下限设置"));
	}
	else if (m_docId == 14)
	{
		this->SetTitle(_T("算法设置"));
	}
	else if (m_docId == 15)
	{
		this->SetTitle(_T("播放与采集设置"));
	}

	
	docIdList.push_back(m_docId);
	

	return TRUE;
}


void CLSAppDoc::OnCloseDocument()
{


	//return;//禁止关闭

	if(bExtDoc)
	{
		bExtDoc=false;
		return;
	}
	
		
	
	CString title=this->GetTitle();
		
	if(title=="瑞森声卡设置")
	{
		/*int res=AfxMessageBox(_T("确认修改吗？"),MB_OKCANCEL);
		if(res==2)
			return;*/

		 CMDIChildWnd* pChild=(CMDIChildWnd*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveFrame();
		 MyDlg* pdlg=(MyDlg*)pChild->GetActiveView();
		 if(!pdlg->SaveData())
		     AfxMessageBox(_T("保存失败！"));
		m_docId=0;
	}   
	else if(title=="灵敏度测试")
    {
        m_docId=1;
	}else if(title=="灵敏度测试_C公司")
    {
        m_docId=2;
	}else if(title=="设备校准")
	{
	     m_docId=3;
	}else if(title=="一体箱隔音测试")
	{
	     m_docId=4;
	}else 
	//if(title=="激励设置")
	////{
	////	m_docId = 5;
	////	if (!jiliset.isdrawrun)//等待绘图状态停止
	////	{
	////		return;
	////	}

	////     
	////}
	//else 
	if (title == "蓝牙串口调试")
	{
		m_docId = 6;
		blueComm.setIndex(0);
		blueComm.OnBnClickedBtnclose();

	}
	else if (title == "RME声卡测试")
	{
		m_docId = 7;
	}
	else if (title == "采集卡(4022)测试")
	{
		m_docId = 8;
	}
	else if (title == "蓝牙耳机测试")
	{
		m_docId = 9;
		blueEreTest.destroyRes();

	}
	else if (title == "激励设置")
	{
		m_docId = 10;
	}
	else if (title == "设备管理")
	{

		/*int i = 0;
		CString s1, s2, s3;
		for (list<MyControldata*>::iterator it = devicemanage.inlist.begin(); it != devicemanage.inlist.end(); it++)
		{

			
			s1 = devicemanage.data[i]->getInnerName();
			s2 = devicemanage.data[i]->getDriverType();
			s3 = devicemanage.data[i]->getdeviceChsName();
			
			(*it)->setdata(s1, s2,s3);

			i++;

		}*/


		devicemanage.setInitFinish(false);
		m_docId = 11;
	}
	else if (title == "仪器校准")
	{
		m_docId = 12;
	}
	else if (title == "上下限设置")
	{
		m_docId = 13;
	}
	else if (title == "算法设置")
	{
		m_docId = 14;
	}
	else if (title == "播放与采集设置")
	{
		m_docId = 15;
	}
	

	for(vector<int>::iterator var = docIdList.begin();var!=docIdList.end();var++)
	{
		if(*var==m_docId)
		{  
		    docIdList.erase(var);
		    break;
		}
	}

	if (docIdList.size() == 0)
	{
		((CMainFrame*)theApp.GetMainWnd())->setBackGroundShow(true);
	}

	CDocument::OnCloseDocument();
}

void CLSAppDoc::SetDocTitile()
{
	AfxMessageBox(_T("ok"));
}




// CLSAppDoc 序列化

void CLSAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}

#ifdef SHARED_HANDLERS

// 缩略图的支持
void CLSAppDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// 修改此代码以绘制文档数据
	dc.FillSolidRect(lprcBounds, RGB(255, 255, 255));

	CString strText = _T("TODO: implement thumbnail drawing here");
	LOGFONT lf;

	CFont* pDefaultGUIFont = CFont::FromHandle((HFONT) GetStockObject(DEFAULT_GUI_FONT));
	pDefaultGUIFont->GetLogFont(&lf);
	lf.lfHeight = 36;

	CFont fontDraw;
	fontDraw.CreateFontIndirect(&lf);

	CFont* pOldFont = dc.SelectObject(&fontDraw);
	dc.DrawText(strText, lprcBounds, DT_CENTER | DT_WORDBREAK);
	dc.SelectObject(pOldFont);
}

// 搜索处理程序的支持
void CLSAppDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// 从文档数据设置搜索内容。
	// 内容部分应由“;”分隔

	// 例如:  strSearchContent = _T("point;rectangle;circle;ole object;")；
	SetSearchContent(strSearchContent);
}

void CLSAppDoc::SetSearchContent(const CString& value)
{
	if (value.IsEmpty())
	{
		RemoveChunk(PKEY_Search_Contents.fmtid, PKEY_Search_Contents.pid);
	}
	else
	{
		CMFCFilterChunkValueImpl *pChunk = NULL;
		ATLTRY(pChunk = new CMFCFilterChunkValueImpl);
		if (pChunk != NULL)
		{
			pChunk->SetTextValue(PKEY_Search_Contents, value, CHUNK_TEXT);
			SetChunkValue(pChunk);
		}
	}
}

#endif // SHARED_HANDLERS

// CLSAppDoc 诊断

#ifdef _DEBUG
void CLSAppDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CLSAppDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CLSAppDoc 命令
