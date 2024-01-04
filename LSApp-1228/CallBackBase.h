#pragma once
#include "StdAfx.h"   
#include<time.h>
#include<string>
#include<vector>
using namespace std;
/*
*回调基类，使用时需要继承实例化
*/
class CCallBackBase
{
public:
	CCallBackBase(){};
	~CCallBackBase(){};
	virtual void ErrorMessage(const char* pStr){};
	virtual void RecieveMessage(const char* pStr){};
	virtual void GetDev(const char* pStr){};
	virtual void RunLog(const char* pStr){};
	virtual void RecieveUSB4431Data(double * readArrary, UINT32 dataLength){};
	virtual void ShowData(const char* wave_name, double * x_point, double * y_point, UINT32 dataLength){};
	virtual void TimerEvent(int index){};
	string getTime()
	{
		char tmp[64];
		time_t timep;
		time(&timep);
		struct tm lt;
		localtime_s(&lt, &timep);
		sprintf_s(tmp, "%02d:%02d:%02d", lt.tm_hour, lt.tm_min, lt.tm_sec);
		return tmp;
	};
	void SplitString(const string& s, vector<string>& v, const string& c)
	{
		string::size_type pos1, pos2;
		pos2 = s.find(c);
		pos1 = 0;
		while (string::npos != pos2)
		{
			v.push_back(s.substr(pos1, pos2 - pos1));

			pos1 = pos2 + c.size();
			pos2 = s.find(c, pos1);
		}
		if (pos1 != s.length())
			v.push_back(s.substr(pos1));
	};
	long getTimeInterval(long time_start)
	{
		clock_t time_end = clock();
		return 1000 * (time_end - time_start) / CLOCKS_PER_SEC;
	};
};