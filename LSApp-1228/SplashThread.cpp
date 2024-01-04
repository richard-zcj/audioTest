#include "stdafx.h"
#include "SplashThread.h"


IMPLEMENT_DYNAMIC(SplashThread, CWinThread)
SplashThread::SplashThread()
{
	
}


SplashThread::~SplashThread()
{
}


BEGIN_MESSAGE_MAP(SplashThread, CWinThread)

END_MESSAGE_MAP()



BOOL SplashThread::InitInstance()
{
	//::AttachThreadInput(m_nThreadID, AfxGetApp()->m_nThreadID, TRUE);
	startpage = new CStartPage;
	startpage->Create(CStartPage::IDD);
	startpage->ShowWindow(SW_SHOW);
	return TRUE;
}

void SplashThread::hidewindow()
{
	startpage->SendMessage(WM_CLOSE);
}

int SplashThread::ExitInstance()
{
	startpage->DestroyWindow();
	if (startpage != NULL)
	{
		delete startpage;
		startpage = NULL;
	}
	return CWinThread::ExitInstance();
}
