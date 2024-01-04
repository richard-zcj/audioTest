#include "stdafx.h"
#include "Public.h"

CString g_sAppPath = _T("");
CString g_sConfigFile = _T("");
CString g_sScriptFile= _T("");
vector<devParam> g_vInputDevs;
vector<devParam> g_vOutputDevs;

CString IntToStr(int nValue)
{
	CString sRet(_T(""));
	sRet.Format(_T("%d"), nValue);
	return sRet;
}

bool FileExists(LPCTSTR pFileName)
{
	WIN32_FIND_DATA FindFileData;
	HANDLE hFind;

	hFind = FindFirstFile(pFileName, &FindFileData);

	if (hFind != INVALID_HANDLE_VALUE)
	{
		FindClose(hFind);
		return true;
	}

	return false;
}

CString CfgReadStr(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault)
{
	CString sRet = CString(lpDefault);
	if (!FileExists(g_sConfigFile))
	{
		return sRet;
	}

	TCHAR szData[MAX_PATH];
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(lpSection, lpKey, lpDefault, szData, sizeof(szData), g_sConfigFile.GetBuffer(0));
	return CString(szData);
}

int CfgReadInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault)
{
	if (!FileExists(g_sConfigFile))
	{
		return nDefault;
	}

	return GetPrivateProfileInt(lpSection, lpKey, nDefault, g_sConfigFile.GetBuffer(0));
}

bool CfgReadBool(LPCTSTR lpSection, LPCTSTR lpKey, bool bDefault)
{
	CString sRet = CfgReadStr(lpSection, lpKey, _T(""));
	if (sRet.IsEmpty())
	{
		return bDefault;
	}
	
	if (sRet.CompareNoCase(_T("true")) == 0)
	{
		return true;
	}
	else if (sRet.CompareNoCase(_T("false")) == 0)
	{
		return false;
	}
	else if (sRet.Compare(_T("0")) == 0)
	{
		return false;
	}

	return true;
}

double CfgReadDouble(LPCTSTR lpSection, LPCTSTR lpKey, double nDefault)
{
	CString sRet = CfgReadStr(lpSection, lpKey, _T(""));
	if (sRet.IsEmpty())
	{
		return nDefault;
	}

	return _ttof(sRet);
}

CString ScpReadStr(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault)
{
	CString sRet = CString(lpDefault);
	if (!FileExists(g_sConfigFile))
	{
		return sRet;
	}

	TCHAR szData[MAX_PATH];
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(lpSection, lpKey, lpDefault, szData, sizeof(szData), g_sScriptFile.GetBuffer(0));
	return CString(szData);
}

int ScpReadInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault)
{
	if (!FileExists(g_sConfigFile))
	{
		return nDefault;
	}

	return GetPrivateProfileInt(lpSection, lpKey, nDefault, g_sScriptFile.GetBuffer(0));
}

bool ScpReadBool(LPCTSTR lpSection, LPCTSTR lpKey, bool bDefault)
{
	CString sRet = CfgReadStr(lpSection, lpKey, _T(""));
	if (sRet.IsEmpty())
	{
		return bDefault;
	}

	if (sRet.CompareNoCase(_T("true")) == 0)
	{
		return true;
	}
	else if (sRet.CompareNoCase(_T("false")) == 0)
	{
		return false;
	}
	else if (sRet.Compare(_T("0")) == 0)
	{
		return false;
	}

	return true;
}

double ScpReadDouble(LPCTSTR lpSection, LPCTSTR lpKey, double nDefault)
{
	CString sRet = CfgReadStr(lpSection, lpKey, _T(""));
	if (sRet.IsEmpty())
	{
		return nDefault;
	}

	return _ttof(sRet);
}

bool Split(LPCTSTR lpSrcStr, LPCTSTR lpPattern, vector<CString>& vList)
{
	if (lpSrcStr == NULL || lpPattern == NULL)
	{
		return false;
	}

	CString sSrcStr(lpSrcStr);
	CString sPattern(lpPattern);
	if (sSrcStr.IsEmpty() || sSrcStr.GetLength() < sPattern.GetLength())
	{
		return false;
	}

	CString sTemp = _T("");
	int nPos = sSrcStr.Find(sPattern);

	while (nPos >= 0)
	{
		if (0 == nPos)
		{
			sSrcStr.Delete(0, sPattern.GetLength());
		}
		else
		{
			sTemp = sSrcStr.Left(nPos);
			vList.push_back(sTemp);
			if (!sTemp.IsEmpty())
			{
				sSrcStr.Delete(0, sPattern.GetLength() + nPos);
			}
			else
			{
				sSrcStr.Delete(0, sPattern.GetLength());
			}
		}

		if (sSrcStr.GetLength() > 0)
		{
			nPos = sSrcStr.Find(sPattern);
		}
		else
		{
			break;
		}
	}

	vList.push_back(sSrcStr);
	return true;
}

void LoadChannelData()
{
	g_vInputDevs.clear();
	g_vOutputDevs.clear();
	CString sData;
	TCHAR szItem[4];
	vector<CString> vList;
	devParam stDev;
	int nItems = ScpReadInt(_T("InputChannels"), _T("Items"), 0);
	for (int i = 0; i < nItems; i++)
	{
		memset(szItem, 0, sizeof(szItem));
		_stprintf_s(szItem, sizeof(szItem), _T("%d"), i);
		sData = ScpReadStr(_T("InputChannels"), szItem, _T(""));
		vList.clear();
		Split(sData.GetBuffer(0), _T(","), vList);
		if (vList.size() < 8)
		{
			continue;
		}

		memset(stDev.szName, 0, sizeof(stDev.szName));
		StrCpy(stDev.szName, vList[0].GetBuffer(0));
		memset(stDev.szDriver, 0, sizeof(stDev.szDriver));
		StrCpy(stDev.szDriver, vList[1].GetBuffer(0));
		memset(stDev.szDevice, 0, sizeof(stDev.szDevice));
		StrCpy(stDev.szDevice, vList[2].GetBuffer(0));
		stDev.nChannel = StrToInt(vList[3].GetBuffer(0));
		stDev.nVp = StrToInt(vList[4].GetBuffer(0));
		stDev.nSamplingRate = StrToInt(vList[5].GetBuffer(0));
		stDev.nBitDepth = StrToInt(vList[6].GetBuffer(0));
		stDev.nSensitivity = StrToInt(vList[7].GetBuffer(0));
		g_vInputDevs.push_back(stDev);
	}

	nItems = ScpReadInt(_T("OutputChannels"), _T("Items"), 0);
	for (int i = 0; i < nItems; i++)
	{
		memset(szItem, 0, sizeof(szItem));
		_stprintf_s(szItem, sizeof(szItem), _T("%d"), i);
		sData = ScpReadStr(_T("OutputChannels"), szItem, _T(""));
		vList.clear();
		Split(sData.GetBuffer(0), _T(","), vList);
		if (vList.size() < 8)
		{
			continue;
		}

		memset(stDev.szName, 0, sizeof(stDev.szName));
		StrCpy(stDev.szName, vList[0].GetBuffer(0));
		memset(stDev.szDriver, 0, sizeof(stDev.szDriver));
		StrCpy(stDev.szDriver, vList[1].GetBuffer(0));
		memset(stDev.szDevice, 0, sizeof(stDev.szDevice));
		StrCpy(stDev.szDevice, vList[2].GetBuffer(0));
		stDev.nChannel = StrToInt(vList[3].GetBuffer(0));
		stDev.nVp = StrToInt(vList[4].GetBuffer(0));
		stDev.nSamplingRate = StrToInt(vList[5].GetBuffer(0));
		stDev.nBitDepth = StrToInt(vList[6].GetBuffer(0));
		stDev.nGain = StrToInt(vList[7].GetBuffer(0));
		g_vOutputDevs.push_back(stDev);
	}
}

int GetInputChannelIndex(LPCTSTR lpName)
{
	int nRet = -1;

	for (int i = 1; i < g_vInputDevs.size(); i++)
	{
		if (StrCmp(g_vInputDevs[i].szName, lpName) == 0)
		{
			nRet = i;
			break;
		}
	}

	return nRet;
}

int GetOutputChannelIndex(LPCTSTR lpName)
{
	int nRet = -1;

	for (int i = 1; i < g_vOutputDevs.size(); i++)
	{
		if (StrCmp(g_vOutputDevs[i].szName, lpName) == 0)
		{
			nRet = i;
			break;
		}
	}

	return nRet;
}

bool EditInputChannel(const devParam& stDev)
{
	bool bRet = false;

	for (int i = 1; i < g_vInputDevs.size(); i++)
	{
		if (StrCmp(g_vInputDevs[i].szName, stDev.szName) == 0)
		{
			memset(g_vInputDevs[i].szName, 0, sizeof(g_vInputDevs[i].szName));
			StrCpy(g_vInputDevs[i].szName, stDev.szName);
			memset(g_vInputDevs[i].szDriver, 0, sizeof(g_vInputDevs[i].szDriver));
			StrCpy(g_vInputDevs[i].szDriver, stDev.szDriver);
			memset(g_vInputDevs[i].szDevice, 0, sizeof(g_vInputDevs[i].szDevice));
			StrCpy(g_vInputDevs[i].szDevice, stDev.szDevice);
			g_vInputDevs[i].nChannel = stDev.nChannel;
			g_vInputDevs[i].nVp = stDev.nVp;
			g_vInputDevs[i].nSamplingRate = stDev.nSamplingRate;
			g_vInputDevs[i].nBitDepth = stDev.nBitDepth;
			g_vInputDevs[i].nSensitivity = stDev.nSensitivity;
			bRet = true;
			break;
		}
	}

	return bRet;
}

bool EditOutputChannel(const devParam& stDev)
{
	bool bRet = false;

	for (int i = 1; i < g_vOutputDevs.size(); i++)
	{
		if (StrCmp(g_vOutputDevs[i].szName, stDev.szName) == 0)
		{
			memset(g_vOutputDevs[i].szName, 0, sizeof(g_vOutputDevs[i].szName));
			StrCpy(g_vOutputDevs[i].szName, stDev.szName);
			memset(g_vOutputDevs[i].szDriver, 0, sizeof(g_vOutputDevs[i].szDriver));
			StrCpy(g_vOutputDevs[i].szDriver, stDev.szDriver);
			memset(g_vOutputDevs[i].szDevice, 0, sizeof(g_vOutputDevs[i].szDevice));
			StrCpy(g_vOutputDevs[i].szDevice, stDev.szDevice);
			g_vOutputDevs[i].nChannel = stDev.nChannel;
			g_vOutputDevs[i].nVp = stDev.nVp;
			g_vOutputDevs[i].nSamplingRate = stDev.nSamplingRate;
			g_vOutputDevs[i].nBitDepth = stDev.nBitDepth;
			g_vOutputDevs[i].nSensitivity = stDev.nSensitivity;
			bRet = true;
			break;
		}
	}

	return bRet;
}
