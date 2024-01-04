//   HttpClient.h:   interface   for   the   CHttpClient   class.   
  //   
  //   
    
#ifndef HTTP_CLIENT_H_
#define HTTP_CLIENT_H_
 
#include "wininet.h"
#include "afxinet.h"
#include <vector>

typedef struct ST_TEST_DATA
{
	CString sResult;
	CString sErrCode;
	CString sErrItem;
	std::vector<CString> vItems;
}testData;

class CHttpClient
{
public:
	CHttpClient(LPCTSTR lpServer, LPCTSTR lpStationCode, LPCTSTR lpWorkGroup,
		LPCTSTR lpSoftVer, bool bSaveRespData = false);
	virtual ~CHttpClient();

	CString GetLastErrMsg();

	bool MesLogin(LPCTSTR lpUser, LPCTSTR lpPwd);
	bool MesCheckRoute(LPCTSTR lpStationCode, LPCTSTR lpSN);
	bool MesUploadData(LPCTSTR lpStationCode, LPCTSTR lpSN, const testData* pData);

private:
	bool m_bSaveRespData;
	CString m_sServer;
	CString m_sLoginMark;
	CString m_sToken;
	CString m_sWorkGroup;
	CString m_sSoftVer;
	CString m_sLastErrMsg;

	bool HttpPost(LPCTSTR lpUrlData, LPTSTR lpResData, int nTimeout = 5);

	bool ParseLoginJson(LPCTSTR lpJsonData);
	bool ParseRouteJson(LPCTSTR lpJsonData);
	bool ParseUploadJson(LPCTSTR lpJsonData);
};
    
#endif	// HTTP_CLIENT_H_ 
