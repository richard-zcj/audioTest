#include "stdafx.h"
#include "Public.h"
#include "TestData.h"
#include <fstream>

CString IntToStr(int nValue)
{
	CString sRet(_T(""));
	sRet.Format(_T("%d"), nValue);
	return sRet;
}

char* WCharToChar(LPCTSTR lpData, char* pBuff, int nSize)
{
#ifdef UNICODE
	int nLen = WideCharToMultiByte(CP_ACP, 0, lpData, -1, NULL, 0, NULL, NULL);
	if (nLen >= nSize)
	{
		nLen = nSize - 1;
	}

	memset(pBuff, 0, nSize);
	WideCharToMultiByte(CP_ACP, 0, lpData, -1, pBuff, nLen, NULL, NULL);
#else
	memset(pBuff, 0, nSize;);
	if (strlen(lpData) < nSize)
	{
		nSize = nLen;
	}
	else
	{
		nSize -= 1;
	}

	memcpy(pBuff, lpData, nSize);
#endif

	return pBuff;
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

void ForceCreateDirectory(LPCTSTR pPath)
{
	int nIndex = 0;
	CString sPath = pPath;
	CString sTemp;

	sPath.Replace(_T("//"), _T("/"));
	sPath.Replace('/', '\\');
	sPath.Replace(_T("\\\\"), _T("\\"));

	if (sPath.ReverseFind('\\') != sPath.GetLength() - 1)
	{
		sPath += _T("\\");
	}

	while (true)
	{
		nIndex = sPath.Find('\\', nIndex + 1);
		if (nIndex < 0)
		{
			break;
		}

		sTemp = sPath.Left(nIndex);

		if (!PathFileExists(sTemp))
		{
			CreateDirectory(sTemp, NULL);
		}
	}
}

CString GetFilePath(LPCTSTR lpFile)
{
	CString sFile = CString(lpFile);
	if (sFile.IsEmpty())
	{
		return _T("");
	}
	else
	{
		if (sFile.Find(_T(":")) < 0)
		{
			sFile = GetAppPath() + sFile;
		}
	}

	sFile.Replace(_T("/"), _T("\\"));
	sFile.Replace(_T("\\\\"), _T("\\"));

	return sFile;
}

CString GetFileVersion(LPCTSTR lpFileName, int nNum)
{
	CString sFilePath(lpFileName);
	CString sResult(_T("0.0.0.0"));

	if (sFilePath.GetLength() > 0 && PathFileExists(sFilePath))
	{
		VS_FIXEDFILEINFO *pVerInfo = NULL;
		DWORD dwTemp, dwSize;
		BYTE *pData = NULL;
		UINT uLen;
		dwSize = GetFileVersionInfoSize(lpFileName, &dwTemp);

		if (dwSize == 0)
		{
			return sResult;
		}

		pData = new BYTE[dwSize + 1];
		if (pData == NULL)
		{
			return sResult;
		}

		if (!GetFileVersionInfo(lpFileName, 0, dwSize, pData))
		{
			delete[] pData;
			return sResult;
		}

		if (!VerQueryValue(pData, _T("\\"), (void **)&pVerInfo, &uLen))
		{
			delete[] pData;
			return sResult;
		}

		DWORD verMS = pVerInfo->dwFileVersionMS;
		DWORD verLS = pVerInfo->dwFileVersionLS;
		DWORD major = HIWORD(verMS);
		DWORD minor = LOWORD(verMS);
		DWORD build = HIWORD(verLS);
		DWORD revision = LOWORD(verLS);

		delete[] pData;
		if (nNum == 1)
		{
			sResult.Format(_T("%d"), major);
		}
		else if (nNum == 2)
		{
			sResult.Format(_T("%d.%d"), major, minor);
		}
		else if (nNum == 3)
		{
			sResult.Format(_T("%d.%d.%d"), major, minor, build);
		}
		else
		{
			sResult.Format(_T("%d.%d.%d.%d"), major, minor, build, revision);
		}
	}

	return sResult;
}

CString ReadStr(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault, LPCTSTR lpIniFile)
{
	CString sRet = CString(lpDefault);
	if (!FileExists(lpIniFile))
	{
		return sRet;
	}

	TCHAR szData[MAX_PATH];
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(lpSection, lpKey, lpDefault, szData, sizeof(szData), lpIniFile);
	return CString(szData);
}

int ReadInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault, LPCTSTR lpIniFile)
{
	if (!FileExists(lpIniFile))
	{
		return nDefault;
	}

	return GetPrivateProfileInt(lpSection, lpKey, nDefault, lpIniFile);
}

bool ReadBool(LPCTSTR lpSection, LPCTSTR lpKey, bool bDefault, LPCTSTR lpIniFile)
{
	CString sRet = ReadStr(lpSection, lpKey, _T(""), lpIniFile);
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

double ReadDouble(LPCTSTR lpSection, LPCTSTR lpKey, double nDefault, LPCTSTR lpIniFile)
{
	CString sRet = ReadStr(lpSection, lpKey, _T(""), lpIniFile);
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

CTestData* g_pTestData = NULL;

CTestData* InitTestData(LPCTSTR lpConfig)
{
	if (g_pTestData)
	{
		delete g_pTestData;
		g_pTestData = NULL;
	}

	g_pTestData = new CTestData(lpConfig);
	return g_pTestData;
}

void DestroyTestData()
{
	if (g_pTestData)
	{
		delete g_pTestData;
		g_pTestData = NULL;
	}
}

CString GetAppPath()
{
	if (g_pTestData)
	{
		return g_pTestData->GetWorkPath();
	}

	return _T("");
}

CString GetConfigIniFile()
{
	if (g_pTestData)
	{
		return g_pTestData->GetConfigFile();
	}

	return _T("");
}

CString GetScriptIniFile()
{
	if (g_pTestData)
	{
		return g_pTestData->GetScriptFile();
	}

	return _T("");
}

CString GetFileFromCfg(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault)
{
	if (g_pTestData)
	{
		return g_pTestData->GetFileFromConfig(lpSection, lpKey, lpDefault);
	}

	return _T("");
}

CString GetPathFromCfg(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault)
{
	if (g_pTestData)
	{
		return g_pTestData->GetPathFromConfig(lpSection, lpKey, lpDefault);
	}

	return _T("");
}

int GetAllFrPointCount()
{
	if (g_pTestData)
	{
		return g_pTestData->m_vAllPoints.size();
	}

	return 0;
}

double GetIndexOfAllFrPoint(int nIndex)
{
	if (g_pTestData)
	{
		if (nIndex >= g_pTestData->m_vAllPoints.size())
		{
			return 0;
		}

		return g_pTestData->m_vAllPoints[nIndex];
	}

	return 0;
}

int GetDevCount(bool bInput)
{
	if (g_pTestData)
	{
		if (bInput)
		{
			return g_pTestData->m_vInputDevs.size();
		}
		else
		{
			return g_pTestData->m_vOutputDevs.size();
		}
	}

	return 0;
}

devParam* GetDevParam(int nIndex, bool bInput)
{
	if (g_pTestData)
	{
		if (bInput)
		{
			if (nIndex >= g_pTestData->m_vInputDevs.size())
			{
				return NULL;
			}

			return &g_pTestData->m_vInputDevs[nIndex];
		}
		else
		{
			if (nIndex >= g_pTestData->m_vOutputDevs.size())
			{
				return NULL;
			}

			return &g_pTestData->m_vOutputDevs[nIndex];
		}
	}

	return NULL;
}

int GetDevIndex(LPCTSTR lpName, bool bInput)
{
	if (g_pTestData)
	{
		return g_pTestData->GetDeviceIndex(lpName, bInput);
	}

	return -1;
}

bool AddChannelDev(LPCTSTR lpName, bool bInput)
{
	if (g_pTestData)
	{
		return g_pTestData->AddDeviceData(lpName, bInput);
	}

	return false;
}

bool EditChannelDev(LPCTSTR lpName, LPCTSTR lpValue, int nIndex, bool bInput)
{
	if (g_pTestData)
	{
		return g_pTestData->EditDeviceData(lpName, lpValue, nIndex, bInput);
	}

	return false;
}

int GetStimCount()
{
	if (g_pTestData)
	{
		return g_pTestData->m_vStimParam.size();
	}

	return 0;
}

stimParam* GetStimParam(int nIndex)
{
	if (g_pTestData)
	{
		if (nIndex >= g_pTestData->m_vStimParam.size())
		{
			return NULL;
		}

		return &g_pTestData->m_vStimParam[nIndex];
	}

	return NULL;
}

int GetStimIndex(LPCTSTR lpName)
{
	if (g_pTestData)
	{
		return g_pTestData->GetStimulusIndex(lpName);
	}

	return -1;
}

bool AddStimParam(LPCTSTR lpName)
{
	if (g_pTestData)
	{
		return g_pTestData->AddStimulusParam(lpName);
	}

	return false;
}

bool EditStimParam(LPCTSTR lpItem, LPCTSTR lpValue, int nIndex)
{
	if (g_pTestData)
	{
		return g_pTestData->EditStimulusParam(lpItem, lpValue, nIndex);
	}

	return false;
}

bool DeleteStimParam(int nIndex)
{
	/*if (g_pTestData)
	{
		return g_pTestData->DeleteStimulusParam(nIndex);
	}*/

	return false;
}

int GetStimDataCount(int nIndex, bool bSpkMode)
{
	if (g_pTestData)
	{
		if (bSpkMode)
		{
			return g_pTestData->m_vSpkPlayRecord[nIndex].vDatas.size();
		}
		else
		{
			return g_pTestData->m_vMicPlayRecord[nIndex].vDatas.size();
		}
	}

	return 0;
}

void CalcStimData(int nPlayRecordIndex, bool bSpk)
{
	if (g_pTestData)
	{
		return g_pTestData->CalcStimulusData(nPlayRecordIndex, bSpk);
	}
}

int GetPlayRecordCount(bool bSpkMode)
{
	if (g_pTestData)
	{
		if (bSpkMode)
		{
			return g_pTestData->m_vSpkPlayRecord.size();
		}
		else
		{
			return g_pTestData->m_vMicPlayRecord.size();
		}
	}

	return 0;
}

prParam* GetPlayRecord(int nIndex, bool bSpkMode)
{
	if (g_pTestData)
	{
		if (bSpkMode)
		{
			if (nIndex >= g_pTestData->m_vSpkPlayRecord.size())
			{
				return NULL;
			}

			return &g_pTestData->m_vSpkPlayRecord[nIndex];
		}
		else
		{
			if (nIndex >= g_pTestData->m_vMicPlayRecord.size())
			{
				return NULL;
			}

			return &g_pTestData->m_vMicPlayRecord[nIndex];
		}
	}

	return NULL;
}

bool AddPlayRecord(const prParam* pPR, bool bSpkMode)
{
	if (g_pTestData)
	{
		return g_pTestData->AddPlayRecordItem(pPR, bSpkMode);
	}

	return false;
}

bool EditPlayRecord(LPCTSTR lpItem, LPCTSTR lpValue, int nIndex, bool bSpkMode)
{
	if (g_pTestData)
	{
		return g_pTestData->EditPlayRecordItem(lpItem, lpValue, nIndex, bSpkMode);
	}

	return false;
}

bool DeletePlayRecord(int nIndex, bool bSpkMode)
{
	/*if (g_pTestData)
	{
	return g_pTestData->DeletePlayRecordItem(nIndex, bSpkMode);
	}*/

	return false;
}

int GetSlotCount()
{
	if (g_pTestData)
	{
		return g_pTestData->m_vSlotParam.size();
	}

	return 0;
}

slotParam* GetSlotParam(int nIndex)
{
	if (g_pTestData)
	{
		if (nIndex >= g_pTestData->m_vSlotParam.size())
		{
			return NULL;
		}

		return &g_pTestData->m_vSlotParam[nIndex];
	}

	return NULL;
}

bool SetSlotEnable(BOOL bValue, int nIndex)
{
	if (g_pTestData)
	{
		g_pTestData->m_vSlotParam[nIndex].bEnable = bValue;
		return true;
	}

	return false;
}

bool SetSlotBTConnect(BOOL bValue, int nIndex)
{
	if (g_pTestData)
	{
		g_pTestData->m_vSlotParam[nIndex].bBTConnected = (bool)(bValue == TRUE);
		return true;
	}

	return false;
}

bool SetSlotItem(LPCTSTR lpItem, LPCTSTR lpValue, int nIndex)
{
	if (g_pTestData)
	{
		return g_pTestData->SetTestSlotItem(lpItem, lpValue, nIndex);
	}

	return false;
}

bool AddSlotItemData(bool bResult, vector<double>& vItemData, LPCTSTR lpItemName, bool bSpkMode, int nIndex)
{
	if (g_pTestData)
	{
		return g_pTestData->AddTestItemData(bResult, vItemData, lpItemName, bSpkMode, nIndex);
	}

	return false;
}

bool ClearAllSlotItemData()
{
	if (g_pTestData)
	{
		return g_pTestData->ClearAllTestItemData();
	}

	return false;
}

testItem* GetItemParam(LPCTSTR lpItem, bool bSpkMode, int nIndex)
{
	if (g_pTestData)
	{
		return g_pTestData->GetTestItemParam(lpItem, bSpkMode, nIndex);
	}

	return nullptr;
}

itemLimit* GetLimitParam(LPCTSTR lpItem, bool bSpkMode, double nPoint, int nIndex)
{
	if (g_pTestData)
	{
		return g_pTestData->GetPointLimitParam(lpItem, bSpkMode, nPoint, nIndex);
	}

	return nullptr;
}

// 测试数据类
CTestData::CTestData(LPCTSTR lpConfig)
{
	TCHAR szFile[MAX_PATH + 2];
	memset(szFile, '\0', MAX_PATH + 2);
	GetModuleFileName(NULL, szFile, MAX_PATH);
	(_tcsrchr(szFile, _T('\\')))[1] = 0;
	m_sAppPath = CString(szFile);//.Format(_T("%s"), szFile);

	m_sConfigFile.Format(_T("%sConfig.ini"), szFile);

	m_sScriptFile.Format(_T("%sScript\\"), m_sAppPath.GetBuffer(0));
	m_sScriptFile += CfgReadStr(_T("FilePath"), _T("ScriptFile"), _T(""));

	LoadAllFrequencyPoints();
	LoadDeviceData();
	LoadStimulusParam();
	LoadPlayAndRecord();
	LoadTestSlotParam();
}

CTestData::~CTestData()
{
	m_vInputDevs.clear();
	m_vOutputDevs.clear();
	m_vStimParam.clear();
	m_vSpkPlayRecord.clear();
	m_vMicPlayRecord.clear();
}

CString CTestData::CfgReadStr(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault)
{
	CString sRet = CString(lpDefault);
	if (!FileExists(m_sConfigFile))
	{
		return sRet;
	}

	TCHAR szData[MAX_PATH];
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(lpSection, lpKey, lpDefault, szData, sizeof(szData), m_sConfigFile);
	return CString(szData);
}

int CTestData::CfgReadInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault)
{
	if (!FileExists(m_sConfigFile))
	{
		return nDefault;
	}

	return GetPrivateProfileInt(lpSection, lpKey, nDefault, m_sConfigFile);
}

bool CTestData::CfgReadBool(LPCTSTR lpSection, LPCTSTR lpKey, bool bDefault)
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

double CTestData::CfgReadDouble(LPCTSTR lpSection, LPCTSTR lpKey, double nDefault)
{
	CString sRet = CfgReadStr(lpSection, lpKey, _T(""));
	if (sRet.IsEmpty())
	{
		return nDefault;
	}

	return _ttof(sRet);
}

bool CTestData::CfgWriteStr(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpValue)
{
	return WritePrivateProfileString(lpSection, lpKey, lpValue, m_sConfigFile) == TRUE;
}

CString CTestData::ScpReadStr(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault)
{
	CString sRet = CString(lpDefault);
	if (!FileExists(m_sScriptFile))
	{
		return sRet;
	}

	TCHAR szData[MAX_PATH];
	memset(szData, 0, sizeof(szData));
	GetPrivateProfileString(lpSection, lpKey, lpDefault, szData, sizeof(szData), m_sScriptFile);
	return CString(szData);
}

int CTestData::ScpReadInt(LPCTSTR lpSection, LPCTSTR lpKey, int nDefault)
{
	if (!FileExists(m_sScriptFile))
	{
		return nDefault;
	}

	return GetPrivateProfileInt(lpSection, lpKey, nDefault, m_sScriptFile);
}

bool CTestData::ScpReadBool(LPCTSTR lpSection, LPCTSTR lpKey, bool bDefault)
{
	CString sRet = ScpReadStr(lpSection, lpKey, _T(""));
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

double CTestData::ScpReadDouble(LPCTSTR lpSection, LPCTSTR lpKey, double nDefault)
{
	CString sRet = ScpReadStr(lpSection, lpKey, _T(""));
	if (sRet.IsEmpty())
	{
		return nDefault;
	}

	return _ttof(sRet);
}

bool CTestData::ScpWriteStr(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpValue)
{
	return WritePrivateProfileString(lpSection, lpKey, lpValue, m_sScriptFile) == TRUE;
}

CString CTestData::GetChannelName(int nIndex)
{
	CString sRet(_T(""));
	switch (nIndex)
	{
	case 1:
		sRet = _T("Mono");
		break;
	case 2:
		sRet = _T("Stereo");
		break;
	default:
		break;
	}

	return sRet;
}

int CTestData::GetChannelIndex(LPCTSTR lpName)
{
	int nRet = 0;
	if (StrCmpI(lpName, _T("Mono")) == 0)
	{
		nRet = 1;
	}
	else if (StrCmpI(lpName, _T("Stereo")) == 0)
	{
		nRet = 2;
	}

	return nRet;
}

CString CTestData::GetWorkPath()
{
	return m_sAppPath;
}

CString CTestData::GetConfigFile()
{
	return m_sConfigFile;
}

CString CTestData::GetScriptFile()
{
	return m_sScriptFile;
}

CString CTestData::GetFileFromConfig(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault)
{
	CString sFile = CfgReadStr(lpSection, lpKey, lpDefault);
	if (sFile.IsEmpty())
	{
		return _T("");
	}
	else
	{
		if (sFile.Find(_T(":")) < 0)
		{
			sFile = GetAppPath() + sFile;
		}
	}

	sFile.Replace(_T("/"), _T("\\"));
	sFile.Replace(_T("\\\\"), _T("\\"));

	return sFile;
}

CString CTestData::GetPathFromConfig(LPCTSTR lpSection, LPCTSTR lpKey, LPCTSTR lpDefault)
{
	CString sPath = CfgReadStr(lpSection, lpKey, lpDefault);
	if (sPath.IsEmpty())
	{
		return _T("");
	}
	else
	{
		if (sPath.Find(_T(":")) < 0)
		{
			sPath = GetAppPath() + sPath;
		}

		if (sPath[sPath.GetLength() - 1] != '\\' && sPath[sPath.GetLength() - 1] != '/')
		{
			sPath += _T("\\");
		}
	}

	sPath.Replace(_T("/"), _T("\\"));
	sPath.Replace(_T("\\\\"), _T("\\"));

	ForceCreateDirectory(sPath);
	return sPath;
}

void CTestData::LoadDeviceData()
{
	CString sData, sSection;
	vector<CString> vList;
	devParam stDev;

	m_vInputDevs.clear();
	m_vOutputDevs.clear();

	for (int n = 0; n < 2; n++)
	{
		sSection = _T("InputChannels");
		if (n == 1)
		{
			sSection = _T("OutputChannels");
		}

		int nItems = ScpReadInt(sSection, _T("Items"), 0);
		for (int i = 0; i < nItems; i++)
		{
			sData = ScpReadStr(sSection, IntToStr(i), _T(""));
			vList.clear();
			Split(sData.GetBuffer(0), _T(","), vList);
			if (vList.size() < 8)
			{
				continue;
			}

			stDev.sName = vList[0];
			stDev.sDriver = vList[1];
			stDev.sDevice = vList[2];
			stDev.nChannel = StrToInt(vList[3]);
			stDev.nVp = _ttof(vList[4].GetBuffer(0));
			stDev.nSamplingRate = _ttof(vList[5].GetBuffer(0));
			stDev.nBitDepth = StrToInt(vList[6].GetBuffer(0));
			stDev.nDevIndex = StrToInt(vList[8].GetBuffer(0));

			if (n == 0)
			{
				stDev.nSensitivity = _ttof(vList[7].GetBuffer(0));
				m_vInputDevs.push_back(stDev);
			}
			else
			{
				stDev.nGain = _ttof(vList[7].GetBuffer(0));
				m_vOutputDevs.push_back(stDev);
			}
		}
	}
}

int CTestData::GetDeviceIndex(LPCTSTR lpName, bool bInput)
{
	int nRet = -1;

	int nSize = m_vOutputDevs.size();
	if (bInput)
	{
		nSize = m_vInputDevs.size();
	}

	for (int i = 0; i < nSize; i++)
	{
		CString sName(_T(""));
		if (bInput)
		{
			sName = m_vInputDevs[i].sName;
		}
		else
		{
			sName = m_vOutputDevs[i].sName;
		}

		if (StrCmp(sName, lpName) == 0)
		{
			nRet = i;
			break;
		}
	}

	return nRet;
}

bool CTestData::AddDeviceData(LPCTSTR lpName, bool bInput)
{
	CString sChannel(_T("OutputChannels"));
	if (bInput)
	{
		sChannel = _T("InputChannels");
	}

	devParam stDev;
	stDev.sName = CString(lpName);
	int nItems = ScpReadInt(sChannel, _T("Items"), 0);
	if (bInput)
	{
		m_vInputDevs.push_back(stDev);
	}
	else
	{
		m_vOutputDevs.push_back(stDev);
	}

	CString sData(_T(""));
	sData.Format(_T("%s,%s,%s,%d,%.3f,%.3f,%d,%.3f,%d"), stDev.sName, stDev.sDriver,
		stDev.sDevice, stDev.nChannel, stDev.nVp, stDev.nSamplingRate,
		stDev.nBitDepth, stDev.nSensitivity, stDev.nDevIndex);
	ScpWriteStr(sChannel, IntToStr(nItems), sData);
	ScpWriteStr(sChannel, _T("Items"), IntToStr(nItems + 1));
	return true;
}

bool CTestData::EditDeviceData(LPCTSTR lpName, LPCTSTR lpValue, int nIndex, bool bInput)
{
	devParam* pDev = NULL;
	if (bInput)
	{
		pDev = &m_vInputDevs[nIndex];
	}
	else
	{
		pDev = &m_vOutputDevs[nIndex];
	}

	if (StrCmpI(lpName, _T("name")) == 0)
	{
		pDev->sName = CString(lpValue);
		if (pDev->sName.IsEmpty())
		{
			return false;
		}
	}
	else if (StrCmpI(lpName, _T("driver")) == 0)
	{
		pDev->sDriver = CString(lpValue);
	}
	else if (StrCmpI(lpName, _T("device")) == 0)
	{
		pDev->sDevice = CString(lpValue);
	}
	else if (StrCmpI(lpName, _T("channel")) == 0)
	{
		if (StrCmpI(pDev->sDriver, _T("WDM")) == 0)
		{
			pDev->nChannel = StrToInt(lpValue) + 1;
		}
		else
		{
			pDev->nChannel = StrToInt(lpValue);
		}
	}
	else if (StrCmpI(lpName, _T("vp")) == 0)
	{
		pDev->nVp = _ttof(lpValue);
	}
	else if (StrCmpI(lpName, _T("sample")) == 0)
	{
		pDev->nSamplingRate = _ttof(lpValue);
	}
	else if (StrCmpI(lpName, _T("bit")) == 0)
	{
		pDev->nBitDepth = StrToInt(lpValue);
	}
	else if (StrCmpI(lpName, _T("sensitivity")) == 0)
	{
		pDev->nSensitivity = _ttof(lpValue);
	}
	else if (StrCmpI(lpName, _T("gain")) == 0)
	{
		pDev->nGain = _ttof(lpValue);
	}
	else if (StrCmpI(lpName, _T("index")) == 0)
	{
		pDev->nDevIndex = StrToInt(lpValue);
	}

	CString sData(_T(""));
	if (bInput)
	{
		sData.Format(_T("%s,%s,%s,%d,%.3f,%.3f,%d,%.3f,%d"), pDev->sName, pDev->sDriver,
			pDev->sDevice, pDev->nChannel, pDev->nVp, pDev->nSamplingRate,
			pDev->nBitDepth, pDev->nSensitivity, pDev->nDevIndex);
		ScpWriteStr(_T("InputChannels"), IntToStr(nIndex), sData);
	}
	else
	{
		sData.Format(_T("%s,%s,%s,%d,%.3f,%.3f,%d,%.3f,%d"), pDev->sName, pDev->sDriver,
			pDev->sDevice, pDev->nChannel, pDev->nVp, pDev->nSamplingRate,
			pDev->nBitDepth, pDev->nGain, pDev->nDevIndex);
		ScpWriteStr(_T("OutputChannels"), IntToStr(nIndex), sData);
	}

	return true;
}

void CTestData::LoadStimulusParam()
{
	CString sData;
	vector<CString> vList;
	stimParam stStim;

	m_vStimParam.clear();
	int nItems = ScpReadInt(_T("Stimulus"), _T("Items"), 0);
	for (int i = 0; i < nItems; i++)
	{
		sData = ScpReadStr(_T("Stimulus"), IntToStr(i), _T(""));
		vList.clear();
		Split(sData.GetBuffer(0), _T(","), vList);
		if (vList.size() < 11)
		{
			continue;
		}

		stStim.sName = vList[0];
		stStim.nVoltage = _ttof(vList[1].GetBuffer(0));
		stStim.nFrequencyStart = _ttof(vList[2].GetBuffer(0));
		stStim.nFrequencyStop = _ttof(vList[3].GetBuffer(0));
		stStim.nSamplingRate = _ttof(vList[4].GetBuffer(0));
		stStim.nOctave = StrToInt(vList[5].GetBuffer(0));
		stStim.nType = StrToInt(vList[6].GetBuffer(0));
		stStim.nCycle = StrToInt(vList[7].GetBuffer(0));
		stStim.nDuration = StrToInt(vList[8].GetBuffer(0));
		stStim.bEQEnable = StrToInt(vList[9].GetBuffer(0)) != 0;
		stStim.sEQPath = vList[10];
		m_vStimParam.push_back(stStim);
	}
}

int CTestData::GetStimulusIndex(LPCTSTR lpName)
{
	int nRet = -1;

	for (int i = 0; i < m_vStimParam.size(); i++)
	{
		if (StrCmp(m_vStimParam[i].sName, lpName) == 0)
		{
			nRet = i;
			break;
		}
	}

	return nRet;
}

bool CTestData::AddStimulusParam(LPCTSTR lpName)
{
	stimParam stStim;
	stStim.sName = CString(lpName);

	CString sData;
	sData.Format(_T("%s,%.3f,%.3f,%.3f,%.3f,%d,%d,%d,%d,%d,%s"), stStim.sName, stStim.nVoltage,
		stStim.nFrequencyStart, stStim.nFrequencyStop, stStim.nSamplingRate, stStim.nOctave,
		stStim.nType, stStim.nCycle, stStim.nDuration, stStim.bEQEnable ? 1 : 0, stStim.sEQPath);

	m_vStimParam.push_back(stStim);

	int nItems = ScpReadInt(_T("Stimulus"), _T("Items"), 0);
	ScpWriteStr(_T("Stimulus"), IntToStr(nItems), sData);
	ScpWriteStr(_T("Stimulus"), _T("Items"), IntToStr(nItems + 1));
	return true;
}

bool CTestData::EditStimulusParam(LPCTSTR lpItem, LPCTSTR lpValue, int nIndex)
{
	stimParam* pStim = &m_vStimParam[nIndex];

	if (StrCmpI(lpItem, _T("name")) == 0)
	{
		pStim->sName = CString(lpValue);
		if (pStim->sName.IsEmpty())
		{
			return false;
		}
	}
	else if (StrCmpI(lpItem, _T("voltage")) == 0)
	{
		pStim->nVoltage = _ttof(lpValue);
	}
	else if (StrCmpI(lpItem, _T("frstart")) == 0)
	{
		pStim->nFrequencyStart = _ttof(lpValue);
	}
	else if (StrCmpI(lpItem, _T("frstop")) == 0)
	{
		pStim->nFrequencyStop = StrToInt(lpValue);
	}
	else if (StrCmpI(lpItem, _T("sample")) == 0)
	{
		pStim->nSamplingRate = _ttof(lpValue);
	}
	else if (StrCmpI(lpItem, _T("octave")) == 0)
	{
		pStim->nOctave = StrToInt(lpValue);
	}
	else if (StrCmpI(lpItem, _T("type")) == 0)
	{
		pStim->nType = StrToInt(lpValue);
	}
	else if (StrCmpI(lpItem, _T("cycle")) == 0)
	{
		pStim->nCycle = StrToInt(lpValue);
	}
	else if (StrCmpI(lpItem, _T("duration")) == 0)
	{
		pStim->nDuration = StrToInt(lpValue);
	}
	else if (StrCmpI(lpItem, _T("eqenable")) == 0)
	{
		pStim->bEQEnable = StrToInt(lpValue);
	}
	else if (StrCmpI(lpItem, _T("eqpath")) == 0)
	{
		pStim->sEQPath = CString(lpValue);
	}

	CString sData(_T(""));
	sData.Format(_T("%s,%.3f,%.3f,%.3f,%.3f,%d,%d,%d,%d,%d,%s"), pStim->sName, pStim->nVoltage,
		pStim->nFrequencyStart, pStim->nFrequencyStop, pStim->nSamplingRate, pStim->nOctave,
		pStim->nType, pStim->nCycle, pStim->nDuration, pStim->bEQEnable ? 1 : 0, pStim->sEQPath);
	ScpWriteStr(_T("Stimulus"), IntToStr(nIndex), sData);

	return true;
}

bool CTestData::DeleteStimulusParam(int nIndex)
{
	CString sData1, sData2;
	int nItems = ScpReadInt(_T("Stimulus"), _T("Items"), 0);
	if (nItems <= nIndex)
	{
		return false;
	}

	for (int i = nIndex; i < nItems - 1; i++)
	{
		sData1 = ScpReadStr(_T("Stimulus"), IntToStr(i), _T(""));
		sData2 = ScpReadStr(_T("Stimulus"), IntToStr(i + 1), _T(""));
		ScpWriteStr(_T("Stimulus"), IntToStr(i), sData2);
	}

	
	return true;
}

void CTestData::LoadPlayAndRecord()
{
	CString sData, sSection;
	vector<CString> vList;
	prParam stPlayRecord;

	m_vSpkPlayRecord.clear();
	m_vMicPlayRecord.clear();

	for (int n = 0; n < 2; n++)
	{
		sSection = _T("SPKPlayAndRecord");
		if (n == 1)
		{
			sSection = _T("MICPlayAndRecord");
		}

		int nItems = ScpReadInt(sSection, _T("Items"), 0);
		for (int i = 0; i < nItems; i++)
		{
			sData = ScpReadStr(sSection, IntToStr(i), _T(""));
			vList.clear();
			Split(sData.GetBuffer(0), _T(","), vList);
			if (vList.size() < 3)
			{
				continue;
			}

			stPlayRecord.sPlaysDev = vList[0];
			stPlayRecord.sStimName = vList[1];
			stPlayRecord.sRecordDev = vList[2];
			stPlayRecord.nPlayIndex = GetDeviceIndex(vList[0], false);
			stPlayRecord.nRecordIndex = GetDeviceIndex(vList[2], true);
			stPlayRecord.nStimIndex = GetStimulusIndex(vList[1]);
			stPlayRecord.nMode = n;
			stPlayRecord.bEnable = ScpReadBool(sSection, _T("Enable"), true);

			if (n == 0)
			{
				m_vSpkPlayRecord.push_back(stPlayRecord);
			}
			else
			{
				m_vMicPlayRecord.push_back(stPlayRecord);
			}
		}
	}
}

bool CTestData::AddPlayRecordItem(const prParam* pPR, bool bSpkMode)
{
	CString sData;
	sData.Format(_T("%s,%s,%s"), pPR->sPlaysDev, pPR->sStimName, pPR->sRecordDev);
	
	prParam stPR;
	stPR.sPlaysDev = pPR->sPlaysDev;
	stPR.sStimName = pPR->sStimName;
	stPR.sRecordDev = pPR->sRecordDev;
	stPR.nPlayIndex = pPR->nPlayIndex;
	stPR.nStimIndex = pPR->nStimIndex;
	stPR.nRecordIndex = pPR->nRecordIndex;

	CString sSection = _T("MICPlayAndRecord");
	if (bSpkMode)
	{
		stPR.nMode = 0;
		m_vSpkPlayRecord.push_back(stPR);
		sSection = _T("SPKPlayAndRecord");
	}
	else
	{
		stPR.nMode = 1;
		m_vMicPlayRecord.push_back(stPR);
	}
	
	int nItems = ScpReadInt(sSection, _T("Items"), 0);
	ScpWriteStr(sSection, IntToStr(nItems), sData);
	ScpWriteStr(sSection, _T("Items"), IntToStr(nItems + 1));
	return true;
}

bool CTestData::EditPlayRecordItem(LPCTSTR lpName, LPCTSTR lpValue, int nIndex, bool bSpkMode)
{
	prParam* pPR = NULL;
	if (bSpkMode)
	{
		pPR = &m_vSpkPlayRecord[nIndex];
		pPR->nMode = 0;
	}
	else
	{
		pPR = &m_vMicPlayRecord[nIndex];
		pPR->nMode = 1;
	}

	if (StrCmpI(lpName, _T("play")) == 0)
	{
		pPR->sPlaysDev = CString(lpValue);
		pPR->nPlayIndex = GetDeviceIndex(lpValue, false);
	}
	else if (StrCmpI(lpName, _T("stimulus")) == 0)
	{
		pPR->sStimName = CString(lpValue);
		pPR->nStimIndex = GetStimulusIndex(lpValue);
	}
	else if (StrCmpI(lpName, _T("record")) == 0)
	{
		pPR->sRecordDev = CString(lpValue);
		pPR->nRecordIndex = GetDeviceIndex(lpValue, true);
	}

	CString sData(_T(""));
	sData.Format(_T("%s,%s,%s"), pPR->sPlaysDev, pPR->sStimName, pPR->sRecordDev);

	if (bSpkMode)
	{
		ScpWriteStr(_T("MICPlayAndRecord"), IntToStr(nIndex), sData);
	}
	else
	{
		ScpWriteStr(_T("OutputChannels"), IntToStr(nIndex), sData);
	}

	return true;
}

void CTestData::GetItemLimit(LPCTSTR lpSection, testItem* pItem)
{
	CString sData;
	vector<CString> vList;
	itemLimit stLimit;
	int nItems = ScpReadInt(lpSection, _T("Items"), 0);
	for (int i = 0; i < nItems; i++)
	{
		sData = ScpReadStr(lpSection, IntToStr(i), _T(""));
		vList.clear();
		Split(sData.GetBuffer(0), _T(","), vList);
		if (vList.size() < 3)
		{
			continue;
		}

		stLimit.nFrequency = _ttof(vList[0]);
		stLimit.nLower = _ttof(vList[1]);
		stLimit.nUpper = _ttof(vList[2]);

		if (vList.size() > 3 && vList.size() < 8)
		{
			stLimit.bPoint = _ttoi(vList[3]) == 0 ? false : true;
			stLimit.bLower = _ttoi(vList[4]) == 0 ? false : true;
			stLimit.bUpper = _ttoi(vList[5]) == 0 ? false : true;
			stLimit.bData = _ttoi(vList[6]) == 0 ? false : true;
		}

		pItem->vLimit.push_back(stLimit);
	}
}

void CTestData::GetTestItems(slotParam* pSlot, bool bSpk)
{
	testItem stItem;
	vector<CString> vList;
	CString sSection = _T("SPKTestItems");
	if (!bSpk)
	{
		stItem.nMode = 1;
		sSection = _T("MICTestItems");
	}

	CString sItem, sData;
	int nItems = ScpReadInt(sSection, _T("Items"), 0);
	for (int i = 0; i < nItems; i++)
	{
		sItem = ScpReadStr(sSection, IntToStr(i), _T(""));
		stItem.sAliasName = ScpReadStr(sItem, _T("AliasName"), _T(""));
		stItem.sItemName = ScpReadStr(sItem, _T("ItemName"), _T(""));
		stItem.bEnable = ScpReadBool(sItem, _T("Enable"), false);
		stItem.nMethod = ScpReadInt(sItem, _T("Method"), 0);
		sData = ScpReadStr(sItem, _T("Harmonic"), _T(""));
		vList.clear();
		Split(sData.GetBuffer(0), _T(","), vList);
		stItem.vHarmonic.clear();
		stItem.vLimit.clear();
		GetItemLimit(sItem, &stItem);

		for (int j = 0; j < vList.size(); j++)
		{
			stItem.vHarmonic.push_back(StrToInt(vList[j]));
		}

		if (bSpk)
		{
			pSlot->vSpkItems.push_back(stItem);
		}
		else
		{
			pSlot->vMicItems.push_back(stItem);
		}
	}
}

void CTestData::LoadTestSlotParam()
{
	CString sSection;
	slotParam stSlot;
	m_vSlotParam.clear();

	int nItems = ScpReadInt(_T("Slot"), _T("Items"), 0);
	for (int i = 0; i < nItems; i++)
	{
		stSlot.nSlotIndex = i + 1;
		sSection = ScpReadStr(_T("Slot"), IntToStr(i), _T(""));
		stSlot.nSpkPlayRecord = ScpReadInt(sSection, _T("SpkPlayRecord"), 0);
		stSlot.nMicPlayRecord = ScpReadInt(sSection, _T("MicPlayRecord"), 0);
		stSlot.pSerial = new CSerialPortDriver();
		stSlot.pSerial->Port = ScpReadInt(sSection, _T("Port"), 3);
		stSlot.pSerial->Baud = ScpReadInt(sSection, _T("Baud"), 921600);
		stSlot.bEnableBT = ScpReadBool(sSection, _T("EnableBT"), true);
		stSlot.vMicItems.clear();
		stSlot.vSpkItems.clear();

		GetTestItems(&stSlot, true);
		GetTestItems(&stSlot, false);
		m_vSlotParam.push_back(stSlot);
	}
}

bool CTestData::SetTestSlotItem(LPCTSTR lpItem, LPCTSTR lpValue, int nIndex)
{
	slotParam* pTest = &m_vSlotParam[nIndex];

	if (StrCmpI(lpItem, _T("barcode")) == 0)
	{
		pTest->sBarcode = CString(lpValue);
	}
	else if (StrCmpI(lpItem, _T("failmsg")) == 0)
	{
		pTest->sFailMsg = CString(lpValue);
	}
	else if (StrCmpI(lpItem, _T("result")) == 0)
	{
		pTest->nTestResult = StrToInt(lpValue);
	}
	else if (StrCmpI(lpItem,_T("buleID"))==0)
	{
		pTest->sBuleId = CString(lpValue);
	}

	return true;
}

void CTestData::LoadAllFrequencyPoints()
{
	ifstream in;
	ifstream out;
	char szFrFileName[256] = "\0";
	CString sFrPointFile = GetFileFromConfig(_T("FilePath"), _T("FrequencyPointsFile"), _T("f.ini"));
	WCharToChar(sFrPointFile, szFrFileName, sizeof(szFrFileName));

	m_vAllPoints.clear();
	in.open(szFrFileName, ios::in);
	char buf[4096]{0};
	char tempstr[100]{0};
	if (in.is_open())
	{
		string str;
		streamsize s = 4096;
		in.getline(buf, s);

		int i = 0;
		int j = 0;
		while (i < 40967 && buf[i] != '\n')
		{
			if (buf[i] == '\t' || i>2246)
			{
				m_vAllPoints.push_back(atof(tempstr));

				memset(tempstr, 0, 100);
				i++;
				j = 0;

				if (i>2246)
				{
					break;
				}
			}
			else
			{
				if (j < 100)
				{
					tempstr[j++] = buf[i++];
				}
			}
		}
	}
}

double CTestData::FindFrPointPos(double nVal, int& nPos)
{
	double nRet = 0;
	double nMin = 0;
	nMin = abs(nVal - m_vAllPoints.size());
	nRet = m_vAllPoints[0];
	nPos = 0;

	for (size_t i = 1; i < m_vAllPoints.size(); i++)
	{
		if (abs(nVal - m_vAllPoints[i])< nMin)
		{
			nRet = m_vAllPoints[i];
			nMin = abs(nVal - m_vAllPoints[i]);
			nPos = i;

		}
	}

	return nRet;
}

void CTestData::GetFrPoints(prParam* pPlayRecord)
{
	int nStep = 0;
	stimParam* pStim = &m_vStimParam[pPlayRecord->nStimIndex];
	switch (pStim->nOctave)
	{
	case 0:
		//1/3
		nStep = 24 / 3;
		break;
	case 1:
		//1/6
		nStep = 24 / 6;
		break;
	case 2:
		//1/12
		nStep = 24 / 12;
		break;
	case 3:
		//1/24
		nStep = 24 / 24;
		break;
	default:
		break;
	}

	//1.确定开始频点
	int nPos1 = 0;
	int nPos2 = 0;
	double nVal = FindFrPointPos(pStim->nFrequencyStart, nPos1);
	nVal = FindFrPointPos(pStim->nFrequencyStop, nPos2);

	//2.根据步长确定取频点
	for (size_t i = nPos1; i < m_vAllPoints.size(); i += nStep)
	{
		if (nPos2 <= i)
		{
			break;
		}

		pPlayRecord->vPoints.push_back(m_vAllPoints[i]);
	}

	pPlayRecord->vPoints.push_back(nVal);
}

void CTestData::GetEQValues(const prParam* pPlayRecord)
{
	double nVol, nAlarmVol;
	CString sData;
	vector<CString> vList;
	stimParam* pStim = GetStimParam(pPlayRecord->nStimIndex);
	nAlarmVol = ReadDouble(_T("EQVoltage"), _T("AlarmVoltage"), 8, GetFilePath(pStim->sEQPath));
	int nItems = ReadInt(_T("EQVoltage"), _T("Items"), 0, GetFilePath(pStim->sEQPath));
	for (int i = 0; i < nItems; i++)
	{
		sData = ReadStr(_T("EQVoltage"), IntToStr(i), _T(""), GetFilePath(pStim->sEQPath));
		vList.clear();
		Split(sData.GetBuffer(0), _T(","), vList);
		nVol = _ttof(vList[1]);
		if (nVol > nAlarmVol)
			nVol = nAlarmVol;

		m_mEQValue[_ttof(vList[0])] = nVol;
	}
}

void CTestData::CalcStimulusData(int nPRIndex, bool bSpk)
{
	//1.生成激励信号
	//1.1 根据倍频程确定频点
	prParam* pPlayRecord = &m_vMicPlayRecord[nPRIndex];
	if (bSpk)
	{
		pPlayRecord = &m_vSpkPlayRecord[nPRIndex];
	}

	int nStim = pPlayRecord->nStimIndex;
	stimParam* pStim = &m_vStimParam[nStim];
	devParam* pOutputDev = &m_vOutputDevs[pPlayRecord->nPlayIndex];

	pPlayRecord->vDatas.clear();
	pPlayRecord->vPoints.clear();
	pPlayRecord->vPointPos.clear();

	GetFrPoints(pPlayRecord);

	if (pStim->bEQEnable)
	{
		GetEQValues(pPlayRecord);
	}


	//1.2 根据频点集合创建激励信号
	int count = 0;
	double v = pStim->nVoltage * pow(2, 31) * sqrt(2) / pOutputDev->nVp;//pow(2, 31)

	if (pOutputDev->sDriver.IsEmpty())
	{
		AfxMessageBox(_T("设备管理，输出通道未设置！"));
		return;
	}

	int f1 = pStim->nCycle / (pStim->nDuration * 0.001);
	double f;


	//信号前补0
	double offsetTime = 0.05;
	int buling = pStim->nSamplingRate * offsetTime;
	for (size_t i = 0; i < buling; i++)
	{
		pPlayRecord->vDatas.push_back(0);
	}

	for (size_t i = 0; i < pPlayRecord->vPoints.size(); i++)
	{
		f = pPlayRecord->vPoints[i];
		int cur_samples = 0;

		if (f <= f1)
		{
			//cycle/f cycle个周期 乘以1/f 一个周期的时间 等于 cycle个周期的时间 每秒的点数fs  总点数 fs*总时间
			cur_samples = pStim->nCycle * pStim->nSamplingRate / f;
		}
		else
		{
			cur_samples = round(pStim->nDuration * 0.001 * f) * pStim->nSamplingRate / f;
		}


		if (pStim->bEQEnable && m_mEQValue.size() >= pPlayRecord->vPoints.size())
		{
			v = m_mEQValue[f] * pow(2, 31) * sqrt(2) / pOutputDev->nVp;
		}

		pPlayRecord->vPointPos.push_back(cur_samples);

		for (size_t j = 0; j <cur_samples; j++)
		{
			pPlayRecord->vDatas.push_back(1.0 * v * sin(2.0 * 3.1415926 * f * j / pStim->nSamplingRate));
		}
	}

	buling = pStim->nSamplingRate * 0.05;
	for (size_t i = 0; i < buling; i++)
	{
		pPlayRecord->vDatas.push_back(0);
	}

	double nTime = 1.0 * pPlayRecord->vDatas.size() / pStim->nSamplingRate;
	pPlayRecord->nTime = nTime;
}

testItem* CTestData::GetTestItemParam(LPCTSTR lpItem, bool bSpkMode, int nIndex)
{
	slotParam* pTest = &m_vSlotParam[nIndex];

	if (bSpkMode)
	{
		for (int i = 0; i < pTest->vSpkItems.size(); i++)
		{
			if (StrCmpI(lpItem, pTest->vSpkItems[i].sItemName) == 0)
			{
				return &pTest->vSpkItems[i];
			}
		}
	}
	else
	{
		for (int i = 0; i < pTest->vMicItems.size(); i++)
		{
			if (StrCmpI(lpItem, pTest->vMicItems[i].sItemName) == 0)
			{
				return &pTest->vMicItems[i];
			}
		}
	}

	return nullptr;
}

bool CTestData::AddTestItemData(bool bResult, vector<double>& vItemData, LPCTSTR lpItemName, bool bSpkMode, int nIndex)
{
	slotParam* pTest = &m_vSlotParam[nIndex];
	testItem* pItem = GetTestItemParam(lpItemName, bSpkMode, nIndex);
	if (pItem == nullptr)
	{
		return false;
	}

	if (bResult)
	{
		pItem->nTestResult = 0;
	}
	else
	{
		pItem->nTestResult = 1;
		pTest->nTestResult = 1;
	}

	for (int i = 0; i < vItemData.size(); i++)
	{
		pItem->vDatas.push_back(vItemData[i]);
	}

	return true;
}

bool CTestData::ClearAllTestItemData()
{
	for (int i = 0; i < m_vSlotParam.size(); i++)
	{
		slotParam* pSlot = &m_vSlotParam[i];
		for (int j = 0; j < pSlot->vMicItems.size(); j++)
		{
			pSlot->vMicItems[j].nTestResult = 2;
			pSlot->vMicItems[j].vDatas.clear();
		}

		for (int j = 0; j < pSlot->vSpkItems.size(); j++)
		{
			pSlot->vSpkItems[j].nTestResult = 2;
			pSlot->vSpkItems[j].vDatas.clear();
		}

		if (pSlot->bEnable)
		{
			pSlot->nTestResult = 0;
		}
		else
		{
			pSlot->nTestResult = -1;
		}
	}

	return true;
}

itemLimit* CTestData::GetPointLimitParam(LPCTSTR lpItem, bool bSpkMode, double nPoint, int nIndex)
{
	testItem* pItem = GetTestItemParam(lpItem, bSpkMode, nIndex);

	for (int i = 0; i < pItem->vLimit.size(); i++)
	{
		if (nPoint == pItem->vLimit[i].nFrequency)
		{
			return &pItem->vLimit[i];
		}
	}

	return nullptr;
}
