#pragma once
#include "afxwin.h"
#include "StartPage.h"
class SplashThread :
	public CWinThread
{
	DECLARE_DYNCREATE(SplashThread)
protected:
	SplashThread();
	~SplashThread();
public:
	
	

	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void hidewindow();

	DECLARE_MESSAGE_MAP()

protected:
	CStartPage* startpage;
};

