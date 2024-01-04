#include "stdafx.h"
#include "audiodeals.h"
#include "wavHandler.h"
#include <fstream>

using namespace std;


audiodeals::audiodeals()
{

	//readWav("D:\\TestRecord\\wdmRecord1.wav");


	/*double Pi = 3.1415926;
	int l = 260;
	double* data = new double[l];

	for (size_t i = 0; i < l; i++)
	{
		if (i < 20)
		{
			data[i] = 12;
		}
		else if (i>240)
		{
			data[i] = 129;
		}
		else
		{
			data[i] = 1 * sin(2 * Pi * 20 * i / l + Pi / 3) + cos(2 * Pi * 50 * i / l);
		}

		
	}

	myDFT(data, NULL, l, l);*/

}


audiodeals::~audiodeals()
{
}

void audiodeals::readWav(char* path)
{

	/*wave_t* wav = (wave_t*)malloc(sizeof(wave_t));
	wavread(path, wav);
	fclose(wav->fp);*/

	/*ofstream outf1;
	outf1.open("D:\\TestRecord\\audiodeals.txt", ios::trunc |ios::out);

	if (!outf1.is_open())
		AfxMessageBox(_T("audiodeals err"));


	for (size_t i = 0; i < wav->length; i++)
	{



	}

	outf1.close();*/



}


//需要引入噪声处理，滤波器或小波变换

void audiodeals::myDFT(double* indata, double* outdata, double fs, int Len)
{
	/*double* X = new double[Len]{0};
	double* Y = new double[Len]{0};
	double* F = new double[Len]{0};*/





	double X[260]{0};
	double Y[260]{0};
	double F[260]{0};

	double Pi = 3.1415926;
	//与cos做相关性运算

	/*double* x = new double[Len];
	for (size_t i = 0; i < Len; i++)
	{
		x[i]=
	}*/

	for (size_t k = 0; k < Len; k++)
	{
		double x = 0;
		for (size_t i = 0; i < Len; i++)
		{
			 x+= indata[i] * cos(2 * Pi*k*i / Len);
		}
		X[k] = x;
	}



	//与sin做相关行运算
	for (size_t k = 0; k < Len; k++)
	{
		double y = 0;
		for (size_t i = 0; i < Len; i++)
		{
			y += indata[i] * sin(2 * Pi*k*i / Len);
		}
		Y[k] = y;
	}


	//寻找最大值
	double valX = 0;
	valX = X[0];
	int maxX=0, maxY=0;
	for (size_t i = 0; i < Len; i++)
	{
		if (valX<X[i])
		{
			maxX=i;
			valX = X[i];
		}
	}

	double valY =0 ;
	valY = Y[0];
	for (size_t i = 0; i < Len; i++)
	{
		if (valY<Y[i])
		{
			maxY=i;
			valY = Y[i];
		}
	}

	//根据采样率计算出频率
	//f=fs*k/N


	double ap = 2*sqrt(pow(valX, 2) + pow(valY,2))/Len;

	for (size_t i = 0; i < Len/2; i++)
	{
		F[i] = 2*fs*i / Len;
	}

}
