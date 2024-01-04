#pragma once
#ifndef DRAW_WAVEFORM_H
#define DRAW_WAVEFORM_H
#include "afxcmn.h"
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <map>
using namespace std;


#define NORMAL_GRID	 0
#define AUDIO_GRID	 1

struct CoordinateParam
{


	/*画布宽 单位像素*/
	int BmpWidth;
	/*画布高 单位像素*/
	int BmpHeight;
	/*网格类型*/
	int GridType;
	/*音频网格线起始频率 需>=20*/
	double AudioFreqStart;
	/*音频网格线终止频率*/
	double AudioFreqStop;
	/*音频网格线起始dbv值*/
	double AudioDBVStart;
	/*音频网格线dbv步进值*/
	double AudioDBVStep;
	/*Y轴最小单元格标注宽度*/
	//double YCoordinateCellSize;
	/*X轴实线数量，最小为3*/
	int X_AxisSolidLineCount;
	/*Y轴实线数量，最小为3*/
	int Y_AxisSolidLineCount;
	/*X轴两根实线间虚线格数量,最小为1*/
	int X_AxisBrokenLineCount;
	/*Y轴两根实线间虚线格数量,最小为1*/
	int Y_AxisBrokenLineCount;
	/*坐标系与图片边缘的距离（为了显示标准）默认55像素*/
	int BorderWidth;
	/*坐标系中显示字体的宽度*/
	int FontWidth;
	/*坐标系中显示字体的高度*/
	int FontHeight;
	/*字体颜色*/
	COLORREF FontColor;
	/*坐标系中X坐标的单位*/
	string X_AxisUnit;
	/*坐标系中Y坐标的单位*/
	string Y_AxisUnit;
	/*Line线宽*/
	int WaveformLineWidth;
	CoordinateParam()
	{
		BmpWidth = 0;
		BmpHeight = 0;
		GridType = NORMAL_GRID;
		AudioFreqStart = 20;
		AudioFreqStop = 10000;
		AudioDBVStart = -90;
		AudioDBVStep = 10;
		//YCoordinateCellSize = 0.1;
		//XCoordinateScale = 1;
		//YCoordinateScale = 1;
		X_AxisSolidLineCount = 5;
		Y_AxisSolidLineCount = 5;
		//XCoordinateSolidCellCount = 10;
		//YCoordinateSolidCellCount = 10;
		BorderWidth = 55;
		FontWidth = 5;
		FontHeight = 16;
		FontColor = RGB(0xCC, 99, 33);
		X_AxisUnit = "[X unit]";
		Y_AxisUnit = "[Y unit]";
		WaveformLineWidth = 1;
	}

	

};
class CDrawWaveformBitmap
{
public:
	/** 初始化画布内存空间
	*  @param:  HWND hwnd	控件所在窗体句柄
	*  @param:  CWnd *pWin	待显示控件句柄，通过了MFC下的Frame模式的PictureControl的测试，其他控件未知
	*  @param:  u_int wmsg   画图时定时更新界面的主窗体消息
	*  @param:  CoordinateParam param   画布和坐标系参数
	*/
	CDrawWaveformBitmap(HWND hwnd, CWnd *pWin, u_int wmsg, CoordinateParam param);
	~CDrawWaveformBitmap();
	/** 从内存画布复制到当前显示空间
	*/
	void GetCurrentBmp();

	///** 加载波形点参数，会覆盖同名点数据
	//*  @param: string line_name 波形名称
	//*  @param: double *y_point	波形点数组Y
	//*  @param: int length	点数据长度
	//*  @param: COLORREF color   波形颜色
	//*/
	bool LoadPoint(string line_name, double *y_point, long length, COLORREF color = RGB(0, 0, 255));
	///** 加载波形点参数，会覆盖同名点数据
	//*  @param: string line_name 波形名称
	//*  @param: double *x_point	波形点数组X
	//*  @param: double *y_point	波形点数组Y
	//*  @param: int length	点数据长度
	//*  @param: COLORREF color   波形颜色
	//*/
	bool LoadPoint(string line_name, double *x_point, double *y_point, long length, COLORREF color = RGB(0, 0, 255));
	///*CDC初始化是否成功*/
	bool IsInitializeOK ;//= false;
	///**显示所有加载的点位*/
	void Show();
	///**显示指定的点位集
	//* string line_name  点位集标注的名称
	//*/
	void Show(string line_name);
	///**清除画布*/
	void Hide();
	///**清除指定的点集画线
	//* string line_name  点位集标注的名称
	//*/
	void Hide(string line_name);
	///**清除所有加载的点位*/
	void Clear();
	///**清除指定的点数据
	//* string line_name  点位集标注的名称
	//*/
	//void Delete(string line_name);

	void GetWaveformParam(CoordinateParam *wParam);
	void SetWaveformParam(CoordinateParam wParam);
	long PointSize(string line_name);

	
private:
	void DrawGrid();
	///**初始化画布和画网格*/
	void DrawGridLine();
	///**初始化画布和画音频网格*/
	void DrawAudioGridLine();
	///** 开启画图线程*/
	bool OpenDrawThread();
	///** 关闭画图线程*/
	bool CloseDrawTread();
	///**线程函数*/
	void DrawThread();
	///*坐标归一化*/
	POINT NormalizationPoint(double x,double y);
	///*用坐标图擦除画布*/
	void LoadGridLine();
	///*标注线名*/
	void DrawLineName();
	///**重新设定坐标系像素比例尺
	//  *Y轴坐标
	//  *数据长度
	//*/
	void ResetScale(double* point, int len, vector<double> &xVec);
	///**重新设定坐标系像素比例尺
	//  *X轴坐标
	//  *Y轴坐标
	//  *数据长度
	//*/
	void ResetScale(double* x_point, double* y_point, int len);
	///** 存储点位队列后增添点位
	//*  @param: string line_name 波形名称
	//*  @param: POINT *point	波形点数组
	//*  @param: int length	点数据长度
	//*/
	bool AddPoint(string line_name, double *point, int length);
private:
	/*显示窗体句柄*/
	HWND mHwnd;
	/*界面的主窗体消息*/
	u_int mWmsg;
	/*源控件的CDC 显示对象*/
	CDC* pgdiCDC;
	/*格线画布*/
	CDC MemCDC; 
	/*绑定在内存中的格线位图*/
	CBitmap MemBitmap;
	/*从源控件CDC复制的显示对象*/
	CDC MemCDCWaveform;
	/*绑定在内存中的位图*/
	CBitmap MemBitmapWaveform;
	/*坐标系像素原点*/
	POINT CoordinateOrg;
	/*波形图中X轴单元格像素尺寸，与坐标系单元格对应*/
	int XWaveformCellSize;
	/*波形图中Y轴单元格像素尺寸，与坐标系单元格对应*/
	int YWaveformCellSize;
	/*坐标系像素宽度 */
	int CoordinateWidth;
	/*坐标系像素高度 */
	int CoordinateHeight;
	/*坐标系及画布参数 */
	CoordinateParam mWaveformParam;
	CoordinateParam mCurrentWaveformParam;

	/* 线程退出标志变量 */
	 bool bExit ;//= false;
	/*点位容器*/
	 //map<int, double> mAudioGridParam;
	 map<string, vector<double>> m_XPoints;
	 map<string, vector<double>> m_YPoints;
	 map<string, int>mLineShowed;
	 map<string, CPen> mPointsColor;
	 map<string, long> mPointsIndex;
	 vector<string> showLine;
	 vector<string> totalLine;
	 /*容器互斥锁*/
	 mutex mtx;
	 int LeftSorderSize;
	 int RightSorderSize;
	 int TopSorderSize;
	 int BottomSorderSize;
	 /*是否对坐标参数进行重绘*/
	 bool IsShowCoordinateX ;//= false;
	 bool IsShowCoordinateY ;//= false;
	 //坐标轴是否被固定，不再为动态坐标
	 bool X_AxisFixed ;//= false;
	 bool Y_AxisFixed ;//= false;
	 //坐标系参数起始点和实线步进点
	 double X_Axis_Start ;//= 0;
	 double Y_Axis_Start ;//= 0;
	 double X_Axis_Stop ;//= 0;
	 double Y_Axis_Stop ;//= 0;
	 double X_Axis_Step ;//= 0;
	 double Y_Axis_Step ;//= 0;
	 double X_Pixel_Scale ;//= 1;
	 double Y_Pixel_Scale ;//= 1;
};
#endif