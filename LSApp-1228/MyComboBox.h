#pragma once
#include "afxwin.h"
class MyComboBox :
	public CComboBox
{
public:
	MyComboBox();
	~MyComboBox();
	BOOL Attach(int nID, CWnd* pParent);//������ԴID�����Զ��尴ť

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	

	


private:
	CWnd* cWndParent;


	DECLARE_MESSAGE_MAP()

};

