#include "stdafx.h"
#include "HttpClient.h"
#include "curl/curl.h"
#include <stdio.h>
#include <sstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"
#include <iostream>
#include "LogUnit.h"
#include "Public.h"

using namespace std;
using namespace rapidjson;

#ifdef _DEBUG
#pragma comment(lib,"libcurld_x64.lib")
#ifdef UNICODE
typedef GenericDocument<UTF16<> > Document;
typedef GenericValue<UTF16<> > Value;
#else
typedef GenericDocument<UTF8<> > Document;
typedef GenericValue<UTF8<> > Value;
#endif
#else
#pragma comment(lib,"libcurl_x64.lib")
#ifdef UNICODE
typedef GenericDocument<UTF16<> > Document;
typedef GenericValue<UTF16<> > Value;
#else
typedef GenericDocument<UTF8<> > Document;
typedef GenericValue<UTF8<> > Value;
#endif
#endif

typedef struct ST_POST_RESPONSE_DATA
{
	char* pBuff;
	size_t nSize;
}resData;

const CString g_sHttp = _T("Http");

void UtfToGbk(const char* pUtf, LPTSTR lpGbk)
{
	int nLen = MultiByteToWideChar(CP_UTF8, 0, pUtf, -1, NULL, 0);	
	wchar_t* pWStr = new wchar_t[nLen + 1];
	memset(pWStr, 0, nLen + 1);
	MultiByteToWideChar(CP_UTF8, 0, pUtf, -1, pWStr, nLen);
	nLen = WideCharToMultiByte(CP_ACP, 0, pWStr, -1, NULL, 0, NULL, NULL);
	char* pStr = new char[nLen + 1];
	memset(pStr, 0, nLen + 1);
	WideCharToMultiByte(CP_ACP, 0, pWStr, -1, pStr, nLen, NULL, NULL);
	StrCpy(lpGbk, CString(pStr));

	if (pWStr)
	{
		delete[] pWStr;
	}

	if (pStr)
	{
		delete[] pStr;
	}
}

CHttpClient::CHttpClient(LPCTSTR lpServer, LPCTSTR lpStationCode, LPCTSTR lpWorkGroup,
	LPCTSTR lpSoftVer, bool bSaveRespData)
{
	CURLcode nRes = curl_global_init(CURL_GLOBAL_ALL);
	if (nRes != CURLE_OK)
	{
		WriteErrorLogEx(g_sHttp, _T("curl_global_init() failed: %s"), curl_easy_strerror(nRes));
		return;
	}

	m_sToken = _T("");
	m_sLastErrMsg = _T("");
	m_sServer = CString(lpServer);
	m_sWorkGroup = CString(lpWorkGroup);
	m_sLoginMark = _T("ATE_") + CString(lpStationCode);
	m_sSoftVer = CString(lpSoftVer);
	m_bSaveRespData = bSaveRespData;

	if (m_sWorkGroup.IsEmpty())
	{
		m_sWorkGroup = _T("AUD_CS");
	}

	WriteDebugLogEx(g_sHttp, _T("Server: %s, loginMark: %s, WorkGroup: %s, SoftVersion: %s"),
		m_sServer.GetBuffer(0), m_sLoginMark, m_sWorkGroup, m_sSoftVer);

	if (m_sServer.Find(_T("http://")) < 0)
	{
		m_sServer = _T("http://") + m_sServer;
	}
}
    
CHttpClient::~CHttpClient()
{
}

CString CHttpClient::GetLastErrMsg()
{
	return m_sLastErrMsg;
}

static size_t WriteCallback(void* pBuff, size_t nSize, size_t nNum, void* pUserData)
{
	size_t nLen = nSize * nNum;
	resData* pData = (resData *)pUserData;

	char* pMem = (char*)realloc(pData->pBuff, pData->nSize + nLen + 1);
	if (!pMem) {
		WriteErrorLogEx(g_sHttp, _T("not enough memory (realloc returned NULL)"));
		return 0;
	}

	pData->pBuff = pMem;
	memcpy(&(pData->pBuff[pData->nSize]), pBuff, nLen);
	pData->nSize += nLen;
	pData->pBuff[pData->nSize] = 0;

	return nLen;
}

bool CHttpClient::HttpPost(LPCTSTR lpUrlData, LPTSTR lpResData, int nTimeout)
{
	CURL *pUrl;
	CURLcode nRes;
	FILE *pSaveFile = NULL;
	m_sLastErrMsg = _T("");

	char* pUrlData = NULL;
	bool bRet = true;

	resData stData;
	stData.pBuff = (char*)malloc(1);
	stData.nSize = 0;

	pUrl = curl_easy_init();
	if (pUrl)
	{
		int nSize = (int)_tcslen(lpUrlData) * (int)sizeof(TCHAR) + 2;
		pUrlData = new char[nSize];
		memset(pUrlData, 0, nSize);
		WCharToChar(lpUrlData, pUrlData, nSize);

		// 指定url
		curl_easy_setopt(pUrl, CURLOPT_URL, pUrlData);
		curl_easy_setopt(pUrl, CURLOPT_POST, 1L);

		/* send all data to this function  */
		curl_easy_setopt(pUrl, CURLOPT_WRITEFUNCTION, WriteCallback);
		curl_easy_setopt(pUrl, CURLOPT_WRITEDATA, (void *)&stData);

		curl_easy_setopt(pUrl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

		curl_easy_setopt(pUrl, CURLOPT_TIMEOUT, nTimeout);
		nRes = curl_easy_perform(pUrl);
		curl_easy_cleanup(pUrl);

		if (pUrlData)
		{
			delete pUrlData;
			pUrlData = NULL;
		}

		if (nRes != CURLE_OK)
		{
			switch (nRes)
			{
			case CURLE_UNSUPPORTED_PROTOCOL:
				m_sLastErrMsg.Format(_T("不支持的协议，有URL的头部指定，错误码：%d"), nRes);
				break;
			case CURLE_COULDNT_RESOLVE_HOST:
			case CURLE_COULDNT_CONNECT:
				m_sLastErrMsg.Format(_T("不能连接到remote主机或代理，错误码：%d"), nRes);
				break;
			case CURLE_HTTP_RETURNED_ERROR:
				m_sLastErrMsg.Format(_T("http返回错误，错误码：%d"), nRes);
				break;
			case CURLE_READ_ERROR:
				m_sLastErrMsg.Format(_T("读本地文件错误，错误码：%d"), nRes);
				break;
			case CURLE_OPERATION_TIMEDOUT:
				m_sLastErrMsg.Format(_T("发送超时，错误码：%d"), nRes);
				break;
			default:
				m_sLastErrMsg.Format(_T("其它错误，错误码：%d"), nRes);
				break;
			}

			bRet = false;
		}
	}

	if (stData.nSize > 0)
	{
		UtfToGbk(stData.pBuff, lpResData);
	}
	else
	{
		m_sLastErrMsg = _T("响应数据为空");
		bRet = false;
	}

	if (bRet)
	{
		WriteInfoLogEx(g_sHttp, _T("Post成功"));
	}
	else
	{
		WriteErrorLogEx(g_sHttp, _T("Post失败：%s"), m_sLastErrMsg);
	}

	return bRet;
}

bool CHttpClient::ParseLoginJson(LPCTSTR lpJsonData)
{
	WriteDebugLogEx(g_sHttp, _T("开始解析登录响应数据(%s)..."), lpJsonData);
	int nRetCode = -1;
	CString sRetInfo(_T("")), sRealName(_T("")), sLoginMark(_T(""));
	Document domParse;
	bool bRet = domParse.Parse(lpJsonData).HasParseError();
	if (bRet)
	{
		m_sLastErrMsg = _T("登录响应Json数据无法解析");
		WriteErrorLogEx(g_sHttp, _T("无法解析Json数据：%s"), lpJsonData);
		return false;
	}

	if (domParse.HasMember(_T("code")) && domParse[_T("code")].IsInt())
	{
		nRetCode = domParse[_T("code")].GetInt();
	}

	if (domParse.HasMember(_T("info")) && domParse[_T("info")].IsString())
	{
		sRetInfo = CString(domParse[_T("info")].GetString());
	}

	WriteDebugLogEx(g_sHttp, _T("code: %d, info: %s"), nRetCode, sRetInfo);
	bRet = true;
	if (nRetCode != 200)
	{
		m_sLastErrMsg.Format(_T("登录响应错误：%s(状态码：%d)"), sRetInfo.GetBuffer(0), nRetCode);
		bRet = false;
	}

	if (domParse.HasMember(_T("data")) && domParse[_T("data")].IsObject())
	{
		const Value& objData = domParse[_T("data")];
		if (objData.HasMember(_T("userinfo")) && objData[_T("userinfo")].IsObject())
		{
			const Value& objUser = objData[_T("userinfo")];
			if (objUser.HasMember(_T("RealName")) && objUser[_T("RealName")].IsString())
			{
				sRealName = CString(objUser[_T("RealName")].GetString());
			}

			if (objUser.HasMember(_T("token")) && objUser[_T("token")].IsString())
			{
				m_sToken = CString(objUser[_T("token")].GetString());
			}

			if (objUser.HasMember(_T("loginMark")) && objUser[_T("loginMark")].IsString())
			{
				sLoginMark = CString(objUser[_T("loginMark")].GetString());
			}

			WriteDebugLogEx(g_sHttp, _T("data->userinfo->RealName: %s, token: %s, loginMark: %s"),
				sRealName, m_sToken, sLoginMark);
		}
	}

	if (bRet)
	{
		WriteInfoLogEx(g_sHttp, _T("解析登录响应数据成功"));
	}
	else
	{
		WriteErrorLogEx(g_sHttp, _T("%s"), m_sLastErrMsg);

	}

	return bRet;
}

bool CHttpClient::ParseRouteJson(LPCTSTR lpJsonData)
{
	WriteDebugLogEx(g_sHttp, _T("开始解析路由检查响应数据(%s)..."), lpJsonData);
	int nRetCode = -1;
	float nRemainQty = 0;
	CString sRetInfo(_T("")), sWorkNo(_T("")), sPartID(_T("")), sPartName(_T("")), sPartSpec(_T(""));
	Document domParse;
	bool bRet = domParse.Parse(lpJsonData).HasParseError();
	if (bRet)
	{
		m_sLastErrMsg = _T("响应Json数据无法解析");
		WriteErrorLogEx(g_sHttp, _T("无法解析Json数据：%s"), lpJsonData);
		return false;
	}

	if (domParse.HasMember(_T("code")) && domParse[_T("code")].IsInt())
	{
		nRetCode = domParse[_T("code")].GetInt();
	}

	if (domParse.HasMember(_T("info")) && domParse[_T("info")].IsString())
	{
		sRetInfo = CString(domParse[_T("info")].GetString());
	}

	WriteDebugLogEx(g_sHttp, _T("code: %d, info: %s"), nRetCode, sRetInfo);
	bRet = true;
	if (nRetCode != 200)
	{
		m_sLastErrMsg.Format(_T("路由检查响应错误：%s(状态码：%d)"), sRetInfo.GetBuffer(0), nRetCode);
		bRet = false;
	}

	if (domParse.HasMember(_T("data")) && domParse[_T("data")].IsArray())
	{
		const Value& arrData = domParse[_T("data")];

		for (int i = 0; i < (int)arrData.Size(); ++i)
		{
			const Value& objData = arrData[i];

			if (objData.HasMember(_T("mo_no")) && objData[_T("mo_no")].IsString())
			{
				sWorkNo = CString(objData[_T("mo_no")].GetString());
			}

			if (objData.HasMember(_T("keypart_id")) && objData[_T("keypart_id")].IsString())
			{
				sPartID = CString(objData[_T("keypart_id")].GetString());
			}

			if (objData.HasMember(_T("remain_qty")) && objData[_T("remain_qty")].IsFloat())
			{
				nRemainQty = objData[_T("remain_qty")].GetFloat();
			}

			if (objData.HasMember(_T("part_name")) && objData[_T("part_name")].IsString())
			{
				sPartName = CString(objData[_T("part_name")].GetString());
			}

			if (objData.HasMember(_T("part_spec")) && objData[_T("part_spec")].IsString())
			{
				sPartSpec = CString(objData[_T("part_spec")].GetString());
			}

			WriteDebugLogEx(g_sHttp, _T("data[%d]->mo_no: %s, keypart_id: %s, remain_qty: %.1f, part_name: %s, part_spec: %s"),
				sWorkNo, sPartID, nRemainQty, sPartName, sPartSpec);
		}
	}

	if (bRet)
	{
		WriteInfoLogEx(g_sHttp, _T("解析路由检查响应数据成功"));
	}
	else
	{
		WriteErrorLogEx(g_sHttp, _T("%s"), m_sLastErrMsg);
	}

	return bRet;
}

bool CHttpClient::ParseUploadJson(LPCTSTR lpJsonData)
{
	WriteDebugLogEx(g_sHttp, _T("开始解析上传响应数据(%s)..."), lpJsonData);
	int nRetCode = -1;
	CString sRetInfo(_T(""));
	Document domParse;
	bool bRet = domParse.Parse(lpJsonData).HasParseError();
	if (bRet)
	{
		m_sLastErrMsg = _T("响应Json数据无法解析");
		WriteErrorLogEx(g_sHttp, _T("无法解析Json数据：%s"), lpJsonData);
		return false;
	}

	if (domParse.HasMember(_T("code")) && domParse[_T("code")].IsInt())
	{
		nRetCode = domParse[_T("code")].GetInt();
	}

	if (domParse.HasMember(_T("info")) && domParse[_T("info")].IsString())
	{
		sRetInfo = CString(domParse[_T("info")].GetString());
	}

	WriteDebugLogEx(g_sHttp, _T("code: %d, info: %s"), nRetCode, sRetInfo);
	bRet = true;
	if (nRetCode != 200)
	{
		m_sLastErrMsg.Format(_T("上传响应错误：%s(状态码：%d)"), sRetInfo.GetBuffer(0), nRetCode);
		bRet = false;
	}

	if (bRet)
	{
		WriteInfoLogEx(g_sHttp, _T("解析上传响应数据成功"));
	}
	else
	{
		WriteErrorLogEx(g_sHttp, _T("%s"), m_sLastErrMsg);
	}

	return bRet;
}

bool CHttpClient::MesLogin(LPCTSTR lpUser, LPCTSTR lpPwd)
{
	bool bRet = true;
	m_sLastErrMsg = _T("");
	WriteDebugLogEx(g_sHttp, _T("MES Login Start..."));

	if (m_sServer.IsEmpty())
	{
		m_sLastErrMsg += _T("服务器IP为空");
		bRet = false;
	}

	if (m_sLoginMark.IsEmpty())
	{
		m_sLastErrMsg += _T("loginMark为空");
		bRet = false;
	}

	if (lpUser == NULL)
	{
		m_sLastErrMsg += _T("登录账号未设置");
		bRet = false;
	}

	if (lpPwd == NULL)
	{
		m_sLastErrMsg += _T("登录密码未设置");
		bRet = false;
	}

	if (bRet)
	{
		TCHAR szResData[1024] = _T("\0");
		CString sUrl(_T(""));
		sUrl.Format(_T("%s/api/login?data={\"username\":\"%s\",\"password\":\"%s\"}&loginmark=%s"),
			m_sServer.GetBuffer(0), lpUser, lpPwd, m_sLoginMark.GetBuffer(0));
		WriteDebugLogEx(g_sHttp, _T("MES Login Url: %s"), sUrl);

		bRet = HttpPost(sUrl.GetBuffer(0), szResData);
		if (bRet)
		{
			WriteDebugLogEx(g_sHttp, _T("MES登录接口响应数据: %s"), szResData);
			bRet = ParseLoginJson(szResData);
			if (bRet)
			{
				WriteInfoLogEx(g_sHttp, _T("MES登录接口执行成功"));
			}
		}
	}
	else
	{
		WriteErrorLogEx(g_sHttp, _T("MES登录接口异常：%s"), m_sLastErrMsg);
	}

	WriteDebugLogEx(g_sHttp, _T("MES Login End."));
	return bRet;
}

bool CHttpClient::MesCheckRoute(LPCTSTR lpStationCode, LPCTSTR lpSN)
{
	bool bRet = true;
	m_sLastErrMsg = _T("");
	WriteDebugLogEx(g_sHttp, _T("MES Check Route Start..."));

	if (m_sServer.IsEmpty())
	{
		m_sLastErrMsg = _T("服务器IP为空");
		bRet = false;
	}

	if (m_sLoginMark.IsEmpty())
	{
		m_sLastErrMsg += _T("loginMark为空");
		bRet = false;
	}

	if (m_sToken.IsEmpty())
	{
		m_sLastErrMsg += _T("token为空");
		bRet = false;
	}

	// AUD_CS
	if (m_sWorkGroup.IsEmpty())
	{
		m_sWorkGroup = _T("AUD_CS");
	}

	if (lpStationCode == NULL)
	{
		m_sLastErrMsg += _T("路由检查，机台编号未设置");
		bRet = false;
	}

	if (lpSN == NULL)
	{
		m_sLastErrMsg += _T("路由检查，SN未设置");
		bRet = false;
	}

	if (bRet)
	{
		TCHAR szResData[1024] = _T("\0");
		CString sUrl(_T(""));
		sUrl.Format(_T("%s/api/checkroute?data={\"StationCode\":\"%s\",\"WorkGroup\":\"%s\",\"SN\":\"%s\"}&loginmark=%s&token=%s"),
			m_sServer.GetBuffer(0), lpStationCode, m_sWorkGroup.GetBuffer(0), lpSN, m_sLoginMark.GetBuffer(0), m_sToken.GetBuffer(0));
		WriteDebugLogEx(g_sHttp, _T("MES Check Route Url: %s"), sUrl);

		bRet = HttpPost(sUrl.GetBuffer(0), szResData);
		if (bRet)
		{
			WriteDebugLogEx(g_sHttp, _T("MES路由检查接口响应数据: %s"), szResData);
			bRet = ParseRouteJson(szResData);
			if (bRet)
			{
				WriteInfoLogEx(g_sHttp, _T("MES路由检查接口执行成功"));
			}
		}
	}
	else
	{
		WriteErrorLogEx(g_sHttp, _T("MES路由检查接口异常：%s"), m_sLastErrMsg);
	}

	WriteDebugLogEx(g_sHttp, _T("MES Check Route End."));
	return bRet;
}

bool CHttpClient::MesUploadData(LPCTSTR lpStationCode, LPCTSTR lpSN, const testData* pData)
{
	bool bRet = true;
	WriteDebugLogEx(g_sHttp, _T("MES Upload Data Start..."));
	m_sLastErrMsg = _T("");

	if (m_sServer.IsEmpty())
	{
		m_sLastErrMsg = _T("服务器IP为空");
		bRet = false;
	}

	if (m_sLoginMark.IsEmpty())
	{
		m_sLastErrMsg += _T("loginMark为空");
		bRet = false;
	}

	if (m_sToken.IsEmpty())
	{
		m_sLastErrMsg += _T("token为空");
		bRet = false;
	}

	// AUD_CS
	if (m_sWorkGroup.IsEmpty())
	{
		m_sWorkGroup = _T("AUD_CS");
	}

	if (lpStationCode == NULL)
	{
		m_sLastErrMsg += _T("路由检查，机台编号未设置");
		bRet = false;
	}

	if (lpSN == NULL)
	{
		m_sLastErrMsg += _T("路由检查，SN未设置");
		bRet = false;
	}

	if (bRet)
	{
		CString sUrl(_T(""));
		CString sItems(_T(""));
		CString sTempData(_T(""));
		TCHAR szResData[1024] = _T("\0");

		for (int i = 0; i < (int)pData->vItems.size(); i++)
		{
			if (i > 0)
			{
				sItems += _T(",");
			}

			sTempData = _T("");
			sTempData.Format(_T("\"item%d\":\"%s\""), (i + 1), pData->vItems[i]);
			sItems += sTempData;
		}

		WriteDebugLogEx(g_sHttp, _T("Items: %s"), sItems);

		sUrl.Format(_T("%s/api/testresultupload?data={\"StationCode\":\"%s\",\"WorkGroup\":\"%s\",\"SN\":\"%s\",\"TestResult\":\"%s\",\"ErrorCode\":\"%s\",\"TestItem\":\"%s\",\"SoftRev\":\"%s\",\"ItemResult\":{%s}}&loginmark=%s&token=%s"),
			m_sServer.GetBuffer(0), lpStationCode, m_sWorkGroup.GetBuffer(0), lpSN,
			pData->sResult, pData->sErrCode, pData->sErrItem, m_sSoftVer.GetBuffer(0),
			sItems.GetBuffer(0), m_sLoginMark.GetBuffer(0), m_sToken.GetBuffer(0));
		WriteDebugLogEx(g_sHttp, _T("MES Upload Data Url: %s"), sUrl);

		bRet = HttpPost(sUrl.GetBuffer(0), szResData);
		if (bRet)
		{
			WriteDebugLogEx(g_sHttp, _T("MES上传数据接口响应数据: %s"), szResData);
			bRet = ParseUploadJson(szResData);
			if (bRet)
			{
				WriteInfoLogEx(g_sHttp, _T("MES上传数据接口执行成功"));
			}
		}
	}
	else
	{
		WriteErrorLogEx(g_sHttp, _T("MES上传数据接口异常：%s"), m_sLastErrMsg);
	}

	WriteDebugLogEx(g_sHttp, _T("MES Update Data End."));
	return bRet;
}
