#pragma once
#include "afxwin.h"
#include <vector>
#include <map>
using namespace std;
class MyListBox :
	public CListBox
{
public:
	MyListBox();
	~MyListBox();
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual void OnPaint();
	


	void setResStr(const vector<CString>& strlist);
	void serRes( map<CString, bool> res_map);

	void setCurIndex(int index);


private:

	vector<CString> m_res_str;

	map<CString, bool> m_res_map;

	int m_cursel;
	


public:
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	
};

