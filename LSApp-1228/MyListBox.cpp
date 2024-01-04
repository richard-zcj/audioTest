#include "stdafx.h"
#include "MyListBox.h"
#include <mutex>


MyListBox::MyListBox()
{
}

void MyListBox::OnPaint()
{

	
}


MyListBox::~MyListBox()
{
}

void MyListBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{


	mutex mutx;
	mutx.lock();
	// TODO: Add your message handler code here
	ASSERT(lpDrawItemStruct->CtlType == ODT_LISTBOX);
	//LPCTSTR lpszText = (LPCTSTR)lpDrawItemStruct->itemData;

	CString text;
	GetText(lpDrawItemStruct->itemID, text);
	ASSERT(text != "");
	COLORREF clr = RGB(255,0,0);// GetItemData(lpDrawItemStruct->itemID);//itemID 是当前绘制那行的索引
	COLORREF clr2 = RGB(0, 255, 0);
	CDC dc;

	for (size_t i = 0; i < GetCount(); i++)
	{
		SetItemHeight(i, 50);
	}

	dc.Attach(lpDrawItemStruct->hDC);

	CRect rc;
	GetItemRect(m_cursel, rc);

	CBrush b;
	b.CreateSolidBrush(RGB(255, 255, 0));
	dc.DrawFocusRect(rc);

	CFont f;
	f.CreatePointFont(100,_T("宋体"));
	dc.SelectObject(&f);


	
	
	if (m_res_map[text])
	{
		
		   dc.SetTextColor(clr2);		
		
	}
	else
	{
		dc.SetTextColor(clr);
	}
	
	
	
	if (text != _T(""))
	{
		dc.DrawText(
			_T("         "),
			text.GetLength(),
			&lpDrawItemStruct->rcItem,
			DT_CENTER | DT_SINGLELINE | DT_VCENTER);
		dc.DrawText(
			text,
			text.GetLength(),
			&lpDrawItemStruct->rcItem,
			DT_CENTER | DT_SINGLELINE | DT_VCENTER);

	}
	dc.Detach();
	mutx.unlock();
}

void MyListBox::setCurIndex(int index)
{
	m_cursel = index;
}


void MyListBox::setResStr(const vector<CString>& strlist)
{

	for (size_t i = 0; i < strlist.size(); i++)
	{
		m_res_str.push_back(strlist[i]);

	}

}

void MyListBox::serRes(map<CString, bool> res_map)
{
	map<CString, bool>::iterator it = res_map.begin();
	for ( ; it != res_map.end(); it++)
	{
		m_res_map[it->first] = it->second;
	}
}


