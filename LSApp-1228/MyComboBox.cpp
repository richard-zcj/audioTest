#include "stdafx.h"
#include "MyComboBox.h"


MyComboBox::MyComboBox()
{

}


MyComboBox::~MyComboBox()
{

}

void MyComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	CRect rc;
	GetWindowRect(rc);
	ScreenToClient(rc);
	CDC dc;

	dc.Attach(lpDrawItemStruct->hDC);//得到绘制的设备环境CDC

	VERIFY(lpDrawItemStruct->CtlType == ODT_COMBOBOX);


	dc.FillSolidRect(rc, RGB(102, 205, 170));
	dc.FillPath();
}

BOOL MyComboBox::Attach(int nID, CWnd* pParent){

	cWndParent = pParent->GetDlgItem(nID);

	BOOL b=cWndParent->ModifyStyle(0, BS_OWNERDRAW, 0);

	if (!SubclassDlgItem(nID, pParent))

		return FALSE;

	return TRUE;

}

BEGIN_MESSAGE_MAP(MyComboBox, CComboBox)


END_MESSAGE_MAP()