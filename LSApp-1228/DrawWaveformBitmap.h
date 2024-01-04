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


	/*������ ��λ����*/
	int BmpWidth;
	/*������ ��λ����*/
	int BmpHeight;
	/*��������*/
	int GridType;
	/*��Ƶ��������ʼƵ�� ��>=20*/
	double AudioFreqStart;
	/*��Ƶ��������ֹƵ��*/
	double AudioFreqStop;
	/*��Ƶ��������ʼdbvֵ*/
	double AudioDBVStart;
	/*��Ƶ������dbv����ֵ*/
	double AudioDBVStep;
	/*Y����С��Ԫ���ע���*/
	//double YCoordinateCellSize;
	/*X��ʵ����������СΪ3*/
	int X_AxisSolidLineCount;
	/*Y��ʵ����������СΪ3*/
	int Y_AxisSolidLineCount;
	/*X������ʵ�߼����߸�����,��СΪ1*/
	int X_AxisBrokenLineCount;
	/*Y������ʵ�߼����߸�����,��СΪ1*/
	int Y_AxisBrokenLineCount;
	/*����ϵ��ͼƬ��Ե�ľ��루Ϊ����ʾ��׼��Ĭ��55����*/
	int BorderWidth;
	/*����ϵ����ʾ����Ŀ��*/
	int FontWidth;
	/*����ϵ����ʾ����ĸ߶�*/
	int FontHeight;
	/*������ɫ*/
	COLORREF FontColor;
	/*����ϵ��X����ĵ�λ*/
	string X_AxisUnit;
	/*����ϵ��Y����ĵ�λ*/
	string Y_AxisUnit;
	/*Line�߿�*/
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
	/** ��ʼ�������ڴ�ռ�
	*  @param:  HWND hwnd	�ؼ����ڴ�����
	*  @param:  CWnd *pWin	����ʾ�ؼ������ͨ����MFC�µ�Frameģʽ��PictureControl�Ĳ��ԣ������ؼ�δ֪
	*  @param:  u_int wmsg   ��ͼʱ��ʱ���½������������Ϣ
	*  @param:  CoordinateParam param   ����������ϵ����
	*/
	CDrawWaveformBitmap(HWND hwnd, CWnd *pWin, u_int wmsg, CoordinateParam param);
	~CDrawWaveformBitmap();
	/** ���ڴ滭�����Ƶ���ǰ��ʾ�ռ�
	*/
	void GetCurrentBmp();

	///** ���ز��ε�������Ḳ��ͬ��������
	//*  @param: string line_name ��������
	//*  @param: double *y_point	���ε�����Y
	//*  @param: int length	�����ݳ���
	//*  @param: COLORREF color   ������ɫ
	//*/
	bool LoadPoint(string line_name, double *y_point, long length, COLORREF color = RGB(0, 0, 255));
	///** ���ز��ε�������Ḳ��ͬ��������
	//*  @param: string line_name ��������
	//*  @param: double *x_point	���ε�����X
	//*  @param: double *y_point	���ε�����Y
	//*  @param: int length	�����ݳ���
	//*  @param: COLORREF color   ������ɫ
	//*/
	bool LoadPoint(string line_name, double *x_point, double *y_point, long length, COLORREF color = RGB(0, 0, 255));
	///*CDC��ʼ���Ƿ�ɹ�*/
	bool IsInitializeOK ;//= false;
	///**��ʾ���м��صĵ�λ*/
	void Show();
	///**��ʾָ���ĵ�λ��
	//* string line_name  ��λ����ע������
	//*/
	void Show(string line_name);
	///**�������*/
	void Hide();
	///**���ָ���ĵ㼯����
	//* string line_name  ��λ����ע������
	//*/
	void Hide(string line_name);
	///**������м��صĵ�λ*/
	void Clear();
	///**���ָ���ĵ�����
	//* string line_name  ��λ����ע������
	//*/
	//void Delete(string line_name);

	void GetWaveformParam(CoordinateParam *wParam);
	void SetWaveformParam(CoordinateParam wParam);
	long PointSize(string line_name);

	
private:
	void DrawGrid();
	///**��ʼ�������ͻ�����*/
	void DrawGridLine();
	///**��ʼ�������ͻ���Ƶ����*/
	void DrawAudioGridLine();
	///** ������ͼ�߳�*/
	bool OpenDrawThread();
	///** �رջ�ͼ�߳�*/
	bool CloseDrawTread();
	///**�̺߳���*/
	void DrawThread();
	///*�����һ��*/
	POINT NormalizationPoint(double x,double y);
	///*������ͼ��������*/
	void LoadGridLine();
	///*��ע����*/
	void DrawLineName();
	///**�����趨����ϵ���ر�����
	//  *Y������
	//  *���ݳ���
	//*/
	void ResetScale(double* point, int len, vector<double> &xVec);
	///**�����趨����ϵ���ر�����
	//  *X������
	//  *Y������
	//  *���ݳ���
	//*/
	void ResetScale(double* x_point, double* y_point, int len);
	///** �洢��λ���к������λ
	//*  @param: string line_name ��������
	//*  @param: POINT *point	���ε�����
	//*  @param: int length	�����ݳ���
	//*/
	bool AddPoint(string line_name, double *point, int length);
private:
	/*��ʾ������*/
	HWND mHwnd;
	/*�������������Ϣ*/
	u_int mWmsg;
	/*Դ�ؼ���CDC ��ʾ����*/
	CDC* pgdiCDC;
	/*���߻���*/
	CDC MemCDC; 
	/*�����ڴ��еĸ���λͼ*/
	CBitmap MemBitmap;
	/*��Դ�ؼ�CDC���Ƶ���ʾ����*/
	CDC MemCDCWaveform;
	/*�����ڴ��е�λͼ*/
	CBitmap MemBitmapWaveform;
	/*����ϵ����ԭ��*/
	POINT CoordinateOrg;
	/*����ͼ��X�ᵥԪ�����سߴ磬������ϵ��Ԫ���Ӧ*/
	int XWaveformCellSize;
	/*����ͼ��Y�ᵥԪ�����سߴ磬������ϵ��Ԫ���Ӧ*/
	int YWaveformCellSize;
	/*����ϵ���ؿ�� */
	int CoordinateWidth;
	/*����ϵ���ظ߶� */
	int CoordinateHeight;
	/*����ϵ���������� */
	CoordinateParam mWaveformParam;
	CoordinateParam mCurrentWaveformParam;

	/* �߳��˳���־���� */
	 bool bExit ;//= false;
	/*��λ����*/
	 //map<int, double> mAudioGridParam;
	 map<string, vector<double>> m_XPoints;
	 map<string, vector<double>> m_YPoints;
	 map<string, int>mLineShowed;
	 map<string, CPen> mPointsColor;
	 map<string, long> mPointsIndex;
	 vector<string> showLine;
	 vector<string> totalLine;
	 /*����������*/
	 mutex mtx;
	 int LeftSorderSize;
	 int RightSorderSize;
	 int TopSorderSize;
	 int BottomSorderSize;
	 /*�Ƿ��������������ػ�*/
	 bool IsShowCoordinateX ;//= false;
	 bool IsShowCoordinateY ;//= false;
	 //�������Ƿ񱻹̶�������Ϊ��̬����
	 bool X_AxisFixed ;//= false;
	 bool Y_AxisFixed ;//= false;
	 //����ϵ������ʼ���ʵ�߲�����
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