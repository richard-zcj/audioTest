
// LSAppDoc.cpp : CLSAppDoc ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
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
// CLSAppDoc ����/����

CLSAppDoc::CLSAppDoc()
{

	bExtDoc=false;
	// TODO: �ڴ����һ���Թ������

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
	    int res=AfxMessageBox(_T("ȷ���޸���"),MB_OKCANCEL);
		if(res==2)
			return TRUE;
	}*/
	

	if (!CDocument::OnNewDocument())
		return FALSE;


	

	// TODO: �ڴ�������³�ʼ������
	// (SDI �ĵ������ø��ĵ�)
	if(m_docId==0)
	{
		
		this->SetTitle(_T("��ɭ��������"));//
		

	}
	else if(m_docId==1)
    {
	   this->SetTitle(_T("�����Ȳ���"));
	}else if(m_docId==2)
    {
	   this->SetTitle(_T("�����Ȳ���_C��˾"));
	}else if(m_docId==3)
	{
	   this->SetTitle(_T("�豸У׼"));
	}else if(m_docId==4)
	{
	   this->SetTitle(_T("һ�����������"));
	}else if(m_docId==5)
	{
	   //this->SetTitle(_T("��������"));
	}
	else if (m_docId == 6)
	{
		this->SetTitle(_T("�������ڵ���"));
	}
	else if (m_docId == 7)
	{
		this->SetTitle(_T("RME��������"));
	}
	else if (m_docId == 8)
	{
		this->SetTitle(_T("�ɼ���(4022)����"));
	}
	else if (m_docId == 9)
	{
		this->SetTitle(_T("������������"));
	}
	else if (m_docId == 10)
	{
		this->SetTitle(_T("��������"));
	}
	else if (m_docId == 11)
	{
		this->SetTitle(_T("�豸����"));
	}
	else if (m_docId == 12)
	{
		this->SetTitle(_T("����У׼"));
	}
	else if (m_docId == 13 )
	{
		this->SetTitle(_T("����������"));
	}
	else if (m_docId == 14)
	{
		this->SetTitle(_T("�㷨����"));
	}
	else if (m_docId == 15)
	{
		this->SetTitle(_T("������ɼ�����"));
	}

	
	docIdList.push_back(m_docId);
	

	return TRUE;
}


void CLSAppDoc::OnCloseDocument()
{


	//return;//��ֹ�ر�

	if(bExtDoc)
	{
		bExtDoc=false;
		return;
	}
	
		
	
	CString title=this->GetTitle();
		
	if(title=="��ɭ��������")
	{
		/*int res=AfxMessageBox(_T("ȷ���޸���"),MB_OKCANCEL);
		if(res==2)
			return;*/

		 CMDIChildWnd* pChild=(CMDIChildWnd*)((CFrameWnd*)AfxGetApp()->m_pMainWnd)->GetActiveFrame();
		 MyDlg* pdlg=(MyDlg*)pChild->GetActiveView();
		 if(!pdlg->SaveData())
		     AfxMessageBox(_T("����ʧ�ܣ�"));
		m_docId=0;
	}   
	else if(title=="�����Ȳ���")
    {
        m_docId=1;
	}else if(title=="�����Ȳ���_C��˾")
    {
        m_docId=2;
	}else if(title=="�豸У׼")
	{
	     m_docId=3;
	}else if(title=="һ�����������")
	{
	     m_docId=4;
	}else 
	//if(title=="��������")
	////{
	////	m_docId = 5;
	////	if (!jiliset.isdrawrun)//�ȴ���ͼ״ֹ̬ͣ
	////	{
	////		return;
	////	}

	////     
	////}
	//else 
	if (title == "�������ڵ���")
	{
		m_docId = 6;
		blueComm.setIndex(0);
		blueComm.OnBnClickedBtnclose();

	}
	else if (title == "RME��������")
	{
		m_docId = 7;
	}
	else if (title == "�ɼ���(4022)����")
	{
		m_docId = 8;
	}
	else if (title == "������������")
	{
		m_docId = 9;
		blueEreTest.destroyRes();

	}
	else if (title == "��������")
	{
		m_docId = 10;
	}
	else if (title == "�豸����")
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
	else if (title == "����У׼")
	{
		m_docId = 12;
	}
	else if (title == "����������")
	{
		m_docId = 13;
	}
	else if (title == "�㷨����")
	{
		m_docId = 14;
	}
	else if (title == "������ɼ�����")
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




// CLSAppDoc ���л�

void CLSAppDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: �ڴ���Ӵ洢����
	}
	else
	{
		// TODO: �ڴ���Ӽ��ش���
	}
}

#ifdef SHARED_HANDLERS

// ����ͼ��֧��
void CLSAppDoc::OnDrawThumbnail(CDC& dc, LPRECT lprcBounds)
{
	// �޸Ĵ˴����Ի����ĵ�����
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

// ������������֧��
void CLSAppDoc::InitializeSearchContent()
{
	CString strSearchContent;
	// ���ĵ����������������ݡ�
	// ���ݲ���Ӧ�ɡ�;���ָ�

	// ����:  strSearchContent = _T("point;rectangle;circle;ole object;")��
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

// CLSAppDoc ���

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


// CLSAppDoc ����
