#include "stdafx.h"
#include "CardSeting.h"


CCardSeting::CCardSeting(void)
{
}


CCardSeting::~CCardSeting(void)
{
}

void CCardSeting::OnDraw(CDC* pDC)
{
	pDC->TextOutW(100,100,_T("hello"));
}
