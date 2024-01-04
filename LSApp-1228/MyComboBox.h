#pragma once
#include "afxwin.h"
class MyComboBox :
	public CComboBox
{
public:
	MyComboBox();
	~MyComboBox();
	BOOL Attach(int nID, CWnd* pParent);//根据资源ID创建自定义按钮

	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	

	


private:
	CWnd* cWndParent;


	DECLARE_MESSAGE_MAP()

};

