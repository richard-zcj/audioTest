/*
该类为算法实现类

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

