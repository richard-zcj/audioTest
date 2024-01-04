
#include "stdafx.h"
#include "ViewTree.h"
#include "MainFrm.h"
#include "LSAppView.h"
#include "LSApp.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewTree




extern int m_docId;
extern vector<int> docIdList;

CViewTree::CViewTree()
{
}

CViewTree::~CViewTree()
{
}

BEGIN_MESSAGE_MAP(CViewTree, CTreeCtrl)
	ON_NOTIFY_REFLECT(NM_CLICK,&CViewTree::OnClkTree)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewTree ��Ϣ�������

BOOL CViewTree::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
{
	BOOL bRes = CTreeCtrl::OnNotify(wParam, lParam, pResult);

	NMHDR* pNMHDR = (NMHDR*)lParam;
	ASSERT(pNMHDR != NULL);

	if (pNMHDR && pNMHDR->code == TTN_SHOW && GetToolTips() != NULL)
	{
		GetToolTips()->SetWindowPos(&wndTop, -1, -1, -1, -1, SWP_NOMOVE | SWP_NOACTIVATE | SWP_NOSIZE);
	}


	return bRes;
}

//max m_docId=15
void CViewTree::OnClkTree(NMHDR* pNMHDR,LRESULT* PResult)
{	
	   CPoint pt;
	   GetCursorPos(&pt);
	   ScreenToClient(&pt);
	   UINT uFlags=0;
	   HTREEITEM hItem= HitTest(pt,&uFlags); 
	   

	   HTREEITEM hParentItem=GetParentItem(hItem);

	   if(GetItemText(hParentItem)=="A��˾")
	   {
	   
		   if(GetItemText(hItem)=="�����Ȳ���")
	       {	      
			m_docId=1;
			AfxGetApp()->m_pDocManager->OnFileNew();
			for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
			{
				if (*var == 1)
				{
					docIdList.erase(var);
					docIdList.push_back(1);
					break;
				}
			}
		   
	       }
	   
	   }else if(GetItemText(hParentItem)=="����(����)��˾")
	   {
		  if(GetItemText(hItem)=="һ�����������")
	      {
	          m_docId=4;
		      AfxGetApp()->m_pDocManager->OnFileNew();
			  for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
			  {
				  if (*var == 4)
				  {
					  docIdList.erase(var);
					  docIdList.push_back(4);
					  break;
				  }
			  }
	      }

	   }else if(GetItemText(hParentItem)=="C��˾")
	   {
		    if(GetItemText(hItem)=="�����Ȳ���")
	       {	      
			m_docId=2;
			AfxGetApp()->m_pDocManager->OnFileNew();
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

	   }else if(GetItemText(hParentItem)=="D��˾")
	   {

		   if (GetItemText(hItem) == "��������")
		   {
			  

		   }

	   }
	   else if (GetItemText(hParentItem) == "����Ƽ�")
	   {

		   if (GetItemText(hItem) == "�ɼ���(4022)����")
		   {
			   m_docId = 8;
			   AfxGetApp()->m_pDocManager->OnFileNew();
			   for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
			   {
				   if (*var == 8)
				   {
					   docIdList.erase(var);
					   docIdList.push_back(8);
					   break;
				   }
			   }

		   }

	   }

	   //���²�����˾����ж���Ϊȫ�ֲ������
	   if(GetItemText(hItem)=="��ɭ��������")
	   {
		 
		  m_docId=0;
		  AfxGetApp()->m_pDocManager->OnFileNew();
		  for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		  {
			  if (*var == 0)
			  {
				  docIdList.erase(var);
				  docIdList.push_back(0);
				  break;
			  }
		  }
		  
	   }else if(GetItemText(hItem)=="�豸У׼")
	   {
	         m_docId=3;
		  AfxGetApp()->m_pDocManager->OnFileNew();
	   }else /*if(GetItemText(hItem)=="��������")
	   {
	         m_docId=5;
		  AfxGetApp()->m_pDocManager->OnFileNew();
		  for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		  {
			  if (*var == 5)
			  {
				  docIdList.erase(var);
				 docIdList.push_back(5);
				  break;
			  }
		  }
	   }
	   else */if (GetItemText(hItem) == "�������ڵ���")
	   {
		   m_docId = 6;
		   AfxGetApp()->m_pDocManager->OnFileNew();
		   for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		   {
			   if (*var == 6)
			   {
				   docIdList.erase(var);
				   docIdList.push_back(6);
				   break;
			   }
		   }


	   }
	   else if (GetItemText(hItem) == "RME��������")
	   {
		   m_docId = 7;
		   AfxGetApp()->m_pDocManager->OnFileNew();
		   for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		   {
			   if (*var == 7)
			   {
				   docIdList.erase(var);
				   docIdList.push_back(7);
				   break;
			   }
		   }


	   }
	   else if (GetItemText(hItem) == "��������")
	   {
		   m_docId = 9;
		   AfxGetApp()->m_pDocManager->OnFileNew();
		   for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		   {
			   if (*var == 9)
			   {
				   docIdList.erase(var);
				   docIdList.push_back(9);
				   break;
			   }
		   }


	   }
	   else if (GetItemText(hItem) == "��������")
	   {
		   m_docId = 10;
		   AfxGetApp()->m_pDocManager->OnFileNew();
		   for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		   {
			   if (*var == 10)
			   {
				   docIdList.erase(var);
				   docIdList.push_back(10);
				   break;
			   }
		   }


	   }
	   else if (GetItemText(hItem) == "�豸����")
	   {
		   m_docId = 11;
		   AfxGetApp()->m_pDocManager->OnFileNew();
		   for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		   {
			   if (*var == 11)
			   {
				   docIdList.erase(var);
				   docIdList.push_back(11);
				   break;
			   }
		   }


	   }
	   else if (GetItemText(hItem) == "������ɼ�����")
	   {
		   m_docId = 15;
		   AfxGetApp()->m_pDocManager->OnFileNew();
		   for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		   {
			   if (*var == 15)
			   {
				   docIdList.erase(var);
				   docIdList.push_back(15);
				   break;
			   }
		   }


	   }
	   else if (GetItemText(hItem) == "����У׼")
	   {
		   m_docId = 12;
		   AfxGetApp()->m_pDocManager->OnFileNew();
		   for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		   {
			   if (*var == 12)
			   {
				   docIdList.erase(var);
				   docIdList.push_back(12);
				   break;
			   }
		   }
	   }
	   else if (GetItemText(hItem) == "����������")
	   {
		   m_docId = 13;
		   AfxGetApp()->m_pDocManager->OnFileNew();
		   for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		   {
			   if (*var == 13)
			   {
				   docIdList.erase(var);
				   docIdList.push_back(13);
				   break;
			   }
		   }
	   }
	   else if (GetItemText(hItem) == "�㷨����")
	   {
		   m_docId = 14;
		   AfxGetApp()->m_pDocManager->OnFileNew();
		   for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		   {
			   if (*var == 14)
			   {
				   docIdList.erase(var);
				   docIdList.push_back(14);
				   break;
			   }
		   }
	   }
	   /*else if (GetItemText(hItem) == "������")
	   {
		   
		   m_docId = 1301;
		   AfxGetApp()->m_pDocManager->OnFileNew();
		   for (vector<int>::iterator var = docIdList.begin(); var != docIdList.end(); var++)
		   {
			   if (*var == 1301)
			   {
				   docIdList.erase(var);
				   docIdList.push_back(1301);
				   break;
			   }
		   }

	   }*/
	   
}
