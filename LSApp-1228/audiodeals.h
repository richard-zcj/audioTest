/*
����Ϊ�㷨ʵ����

Created by zcj
*/

#pragma once
class audiodeals
{
public:
	audiodeals();
	~audiodeals();

	void readWav(char* path);

	void myDFT(double* indata,double* outdata,double fs,int Len);
};

