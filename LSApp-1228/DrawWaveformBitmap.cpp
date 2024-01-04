
#include "stdafx.h"
#include "DrawWaveformBitmap.h"
#include <time.h>


CDrawWaveformBitmap::CDrawWaveformBitmap(HWND hwnd,CWnd *pWin, u_int wmsg, CoordinateParam param):IsInitializeOK(false),
IsShowCoordinateY(false), X_AxisFixed(false), Y_AxisFixed(false), X_Axis_Start(0), Y_Axis_Start(0), IsShowCoordinateX(false),
	X_Axis_Stop(0), Y_Axis_Stop(0), X_Axis_Step(0), Y_Axis_Step(0), X_Pixel_Scale(1), Y_Pixel_Scale(1), bExit(false)
{
	try
	{
		
		mHwnd = hwnd;
		pgdiCDC = pWin->GetDC();
		mWmsg = wmsg;
		if (param.BmpWidth == 0 || param.BmpHeight == 0)
		{
			CRect rect;
			pWin->GetClientRect(rect);//把控件的长宽、坐标等信息保存在rect里

			param.BmpWidth = rect.Width();//可以获取宽和高
			param.BmpHeight = rect.Height();
		}
		SetWaveformParam(param);
		
		//建立与控件显示兼容的内存显示设备
		MemCDC.CreateCompatibleDC(pgdiCDC);
		//下面建立一个与屏幕显示兼容的位图，至于位图的大小嘛，可以用窗口的大小，也可以自己定义（如：有滚动条时就要大于当前窗口的大小，在BitBlt时决定拷贝内存的哪部分到屏幕上）
		MemBitmap.CreateCompatibleBitmap(pgdiCDC, mWaveformParam.BmpWidth, mWaveformParam.BmpHeight);
		MemCDC.SelectObject(&MemBitmap);

		MemCDCWaveform.CreateCompatibleDC(pgdiCDC);
		MemBitmapWaveform.CreateCompatibleBitmap(pgdiCDC, mWaveformParam.BmpWidth, mWaveformParam.BmpHeight);
		MemCDCWaveform.SelectObject(&MemBitmapWaveform);
		DrawGrid();
		if (OpenDrawThread())
			IsInitializeOK = true;
	}
	catch (...)
	{

	}
}
CDrawWaveformBitmap::~CDrawWaveformBitmap(){
	Clear();
	CloseDrawTread();
	MemBitmap.DeleteObject();
	MemCDC.DeleteDC();
}



long CDrawWaveformBitmap::PointSize(string line_name){
	return m_YPoints.at(line_name).size();
}
void CDrawWaveformBitmap::GetWaveformParam(CoordinateParam *wParam){
	*wParam = mWaveformParam;
}


void CDrawWaveformBitmap::SetWaveformParam(CoordinateParam wParam){
	mWaveformParam = wParam;
	LeftSorderSize = wParam.BorderWidth * 3 / 2;
	RightSorderSize = (int)(wParam.BorderWidth / 2);
	TopSorderSize = wParam.BorderWidth / 2;
	BottomSorderSize = wParam.BorderWidth;
	CoordinateWidth = wParam.BmpWidth - LeftSorderSize - RightSorderSize;
	CoordinateHeight = wParam.BmpHeight - TopSorderSize - BottomSorderSize;
	if (CoordinateWidth < 10 || CoordinateHeight < 10) return;
	CoordinateOrg.x = LeftSorderSize;
	CoordinateOrg.y = wParam.BmpHeight - BottomSorderSize;
	if (wParam.AudioFreqStart < 20) wParam.AudioFreqStart = 20;
	if (wParam.AudioFreqStop < 100) wParam.AudioFreqStop = 100;
	if (wParam.X_AxisSolidLineCount < 3) wParam.X_AxisSolidLineCount = 3;
	if (wParam.Y_AxisSolidLineCount < 3) wParam.Y_AxisSolidLineCount = 3;
	if (wParam.X_AxisBrokenLineCount < 1) wParam.X_AxisBrokenLineCount = 1;
	if (wParam.Y_AxisBrokenLineCount < 1) wParam.Y_AxisBrokenLineCount = 1;
	XWaveformCellSize = CoordinateWidth / (wParam.X_AxisSolidLineCount*wParam.X_AxisBrokenLineCount);
	YWaveformCellSize = CoordinateHeight / (wParam.Y_AxisSolidLineCount*wParam.Y_AxisBrokenLineCount);
	if (IsInitializeOK){
		DrawGrid();
		DrawLineName();
		LoadGridLine();
	}
}


void CDrawWaveformBitmap::DrawGrid()
{
	if (mWaveformParam.GridType == AUDIO_GRID)
	{
		X_AxisFixed = true;
		Y_AxisFixed = true;
		DrawAudioGridLine();
	}
	else if (mWaveformParam.GridType == NORMAL_GRID)
	{
		DrawGridLine();
	}
}

void CDrawWaveformBitmap::GetCurrentBmp(){
	if (!IsInitializeOK) return;
	pgdiCDC->BitBlt(0, 0, mWaveformParam.BmpWidth, mWaveformParam.BmpHeight, &MemCDCWaveform, 0, 0, SRCCOPY);
}

void CDrawWaveformBitmap::DrawAudioGridLine(){
	vector<double> steps;
	vector<int> freq_steps;
	double freq = mWaveformParam.AudioFreqStart;
	double f1 = log10(freq);
	while (( mWaveformParam.AudioFreqStop-freq)>-0.0001)
	{
		steps.push_back(f1);
		freq_steps.push_back((int)(freq + .5));
		int df1 = (int)f1;
		freq = ((int)(freq / (pow(10, df1))) + 1)*pow(10, df1);
		f1 = log10(freq);
	}
	X_Axis_Start = steps[0];
	X_Pixel_Scale = (double)CoordinateWidth / (steps[steps.size() - 1] - steps[0]);
	Y_Axis_Start = mWaveformParam.AudioDBVStart;
	Y_Pixel_Scale = (double)YWaveformCellSize / mWaveformParam.AudioDBVStep;

	//位图的宽度
	long width = mWaveformParam.BmpWidth;
	//位图的高度
	long height = mWaveformParam.BmpHeight;
	//有了画布，下面可以自由的画图了，想怎么画就怎么画，
	CRect rect(0, 0, width, height);
	//MemCDC.Rectangle(rect);
	CBrush bkBrush, drawBrush;
	bkBrush.CreateSolidBrush(RGB(0, 0, 60));
	MemCDC.FillRect(rect, &bkBrush);

	//坐标系画布
	rect.SetRect(LeftSorderSize, TopSorderSize, width - RightSorderSize, CoordinateOrg.y);
	//MemCDC.Rectangle(rect);
	drawBrush.CreateSolidBrush(RGB(0, 0, 30));
	MemCDC.FillRect(rect, &drawBrush);
	//waveform_step_width = (int)(waveform_rect_width / 50);
	//waveform_step_height = (int)(waveform_rect_height / 50);


	/*********************************************************************************************************************************************************************/


	/*************************************************************打印Y轴坐标单位*********************************************************************************************/
	CFont font;
	MemCDC.SetTextColor(mWaveformParam.FontColor);
	MemCDC.SetBkColor(RGB(0, 0, 60));
	font.CreateFontW(
		mWaveformParam.FontHeight,							//字体高度
		mWaveformParam.FontWidth,							//字体宽度
		900,							//字体角度 单位0.1度
		0,							//字符基线和x轴之间的夹角（用0.1度单位）？
		600,						//字体磅数
		FALSE,						//字体是否为斜体
		FALSE,						//字体是否带下划线
		FALSE,						//字体是否带删除线
		0,							//字体的字符集
		OUT_DEFAULT_PRECIS,			//指定所需的输出精度
		CLIP_DEFAULT_PRECIS,		//指定所需的剪贴精度
		DEFAULT_QUALITY,			//指示字体的输出质量
		DEFAULT_PITCH | FF_ROMAN,	//字体间距和家族
		_T("Arial")					//字体类型
		);

	MemCDC.SelectObject(&font);
	CString str_unit_y = CA2T(mWaveformParam.Y_AxisUnit.c_str());
	MemCDC.TextOutW(0, (int)(CoordinateHeight / 2) + (int)(str_unit_y.GetLength()*mWaveformParam.FontWidth / 2), str_unit_y);
	font.DeleteObject();
	/*********************************************************************************************************************************************************************/


	/*************************************************************打印X轴坐标单位*********************************************************************************************/
	font.CreateFontW(mWaveformParam.FontHeight, mWaveformParam.FontWidth, 0, 0, 600,
		FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Arial"));
	MemCDC.SelectObject(&font);
	CString str_unit_x = CA2T(mWaveformParam.X_AxisUnit.c_str());
	MemCDC.TextOutW(LeftSorderSize + (int)(CoordinateWidth / 2) - (int)(str_unit_x.GetLength()*mWaveformParam.FontWidth / 2),
		height - mWaveformParam.FontHeight, str_unit_x);
	/*********************************************************************************************************************************************************************/


	/*************************************************************打印XY轴线 及 标注计量数字*********************************************************************************************/
	CPen SolidPen, DashPen;

	SolidPen.CreatePen(PS_SOLID, 0, RGB(180, 180, 180));
	DashPen.CreatePen(PS_SOLID, 0, RGB(60, 60, 60));
	int j = 0;
	char cStr[100] = { '\0' };
	while (j < steps.size() || YWaveformCellSize*j < CoordinateHeight)
	{
		//x轴划线
		if (j < freq_steps.size())
		{
			if (j == 0 || j == freq_steps.size() - 1 || ((int)steps[j] - (int)steps[j - 1] == 1))
			{
				if (freq_steps[j]>=1000)
				{
					sprintf_s(cStr, "%dk", (int)(freq_steps[j]/1000));
				}
				else
				{
					sprintf_s(cStr, "%d", freq_steps[j] );
				}
				CString str = CA2T(cStr);
				MemCDC.TextOutW((int)((steps[j] - steps[0])*X_Pixel_Scale) + LeftSorderSize - (int)(str.GetLength()*mWaveformParam.FontWidth / 2), CoordinateOrg.y + 10, str);
				MemCDC.SelectStockObject(BLACK_PEN);
				MemCDC.MoveTo((int)((steps[j] - steps[0])*X_Pixel_Scale) + LeftSorderSize, CoordinateOrg.y + 2);
				MemCDC.LineTo((int)((steps[j] - steps[0])*X_Pixel_Scale) + LeftSorderSize, CoordinateOrg.y + 8);
				MemCDC.SelectObject(&SolidPen);
			}
			else
			{
				MemCDC.SelectObject(&DashPen);
			}
			MemCDC.MoveTo((int)((steps[j] - steps[0])*X_Pixel_Scale) + LeftSorderSize, TopSorderSize);
			MemCDC.LineTo((int)((steps[j] - steps[0])*X_Pixel_Scale) + LeftSorderSize, CoordinateOrg.y);
		}
		//打印Y坐标系标注
		if (j % mWaveformParam.Y_AxisBrokenLineCount == 0 && YWaveformCellSize*j < CoordinateHeight)
		{
			if (mWaveformParam.AudioDBVStep > 1)
				sprintf_s(cStr, "%d", (int)(mWaveformParam.AudioDBVStart + mWaveformParam.AudioDBVStep*j));
			else
				sprintf_s(cStr, "%.2f", (mWaveformParam.AudioDBVStart + mWaveformParam.AudioDBVStep*j));
			CString str = CA2T(cStr);
			MemCDC.TextOutW(LeftSorderSize - 12 - (int)(str.GetLength()*mWaveformParam.FontWidth),
			CoordinateOrg.y - YWaveformCellSize*j - (int)(mWaveformParam.FontHeight / 2), str);

			MemCDC.SelectStockObject(BLACK_PEN);
			MemCDC.MoveTo(LeftSorderSize - 8, CoordinateOrg.y - YWaveformCellSize*j);
			MemCDC.LineTo(LeftSorderSize - 2, CoordinateOrg.y - YWaveformCellSize*j);
			MemCDC.SelectObject(&SolidPen);
		}
		else
		{
			MemCDC.SelectObject(&DashPen);
		}


		if (YWaveformCellSize*j < CoordinateHeight)
		{
			MemCDC.MoveTo(LeftSorderSize, CoordinateOrg.y - YWaveformCellSize*j);
			MemCDC.LineTo(width - RightSorderSize, CoordinateOrg.y - YWaveformCellSize*j);
		}
		j++;
	}

	SolidPen.DeleteObject();
	DashPen.DeleteObject();
	font.DeleteObject();
}


void CDrawWaveformBitmap::DrawGridLine(){
	//位图的宽度
	long width = mWaveformParam.BmpWidth;
	//位图的高度
	long height = mWaveformParam.BmpHeight;         
	//有了画布，下面可以自由的画图了，想怎么画就怎么画，
	CRect rect(0, 0, width, height);
	//MemCDC.Rectangle(rect);
	CBrush bkBrush, drawBrush;
	bkBrush.CreateSolidBrush(RGB(0, 0, 60));
	MemCDC.FillRect(rect, &bkBrush);

	//坐标系画布
	rect.SetRect(LeftSorderSize, TopSorderSize, width - RightSorderSize, CoordinateOrg.y);
	//MemCDC.Rectangle(rect);
	drawBrush.CreateSolidBrush(RGB(0, 0, 30));
	MemCDC.FillRect(rect, &drawBrush);
	//waveform_step_width = (int)(waveform_rect_width / 50);
	//waveform_step_height = (int)(waveform_rect_height / 50);


	/*********************************************************************************************************************************************************************/


	/*************************************************************打印Y轴坐标单位*********************************************************************************************/
	CFont font;
	MemCDC.SetTextColor(mWaveformParam.FontColor);
	MemCDC.SetBkColor(RGB(0, 0, 60));
	font.CreateFontW(
		mWaveformParam.FontHeight,							//字体高度
		mWaveformParam.FontWidth,							//字体宽度
		900,							//字体角度 单位0.1度
		0,							//字符基线和x轴之间的夹角（用0.1度单位）？
		600,						//字体磅数
		FALSE,						//字体是否为斜体
		FALSE,						//字体是否带下划线
		FALSE,						//字体是否带删除线
		0,							//字体的字符集
		OUT_DEFAULT_PRECIS,			//指定所需的输出精度
		CLIP_DEFAULT_PRECIS,		//指定所需的剪贴精度
		DEFAULT_QUALITY,			//指示字体的输出质量
		DEFAULT_PITCH | FF_ROMAN,	//字体间距和家族
		_T("Arial")					//字体类型
		);

	MemCDC.SelectObject(&font);
	CString str_unit_y = CA2T(mWaveformParam.Y_AxisUnit.c_str());
	MemCDC.TextOutW(0, (int)(CoordinateHeight / 2) + (int)(str_unit_y.GetLength()*mWaveformParam.FontWidth / 2), str_unit_y);
	font.DeleteObject();
	/*********************************************************************************************************************************************************************/


	/*************************************************************打印X轴坐标单位*********************************************************************************************/
	font.CreateFontW(mWaveformParam.FontHeight,	mWaveformParam.FontWidth, 0, 0, 600,
		FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Arial"));
	MemCDC.SelectObject(&font);
	CString str_unit_x = CA2T(mWaveformParam.X_AxisUnit.c_str());
	MemCDC.TextOutW(LeftSorderSize + (int)(CoordinateWidth / 2) - (int)(str_unit_x.GetLength()*mWaveformParam.FontWidth / 2),
		height - mWaveformParam.FontHeight, str_unit_x);
	/*********************************************************************************************************************************************************************/


	/*************************************************************打印XY轴线 及 标注计量数字*********************************************************************************************/
	CPen SolidPen, DashPen ;

	SolidPen.CreatePen(PS_SOLID, 0, RGB(180, 180, 180));
	DashPen.CreatePen(PS_SOLID, 0, RGB(60, 60, 60));
	int j = 0;
	char cStr[100] = { 0 };
	while (XWaveformCellSize*j  < CoordinateWidth || YWaveformCellSize*j < CoordinateHeight)
	{
		//打印X坐标系标注
		if (j % mWaveformParam.X_AxisBrokenLineCount == 0 && XWaveformCellSize*j < CoordinateWidth)
		{
			/*if (mWaveformParam.XCoordinateCellSize >= 10)
			{
				sprintf_s(cStr, "%d", (int)(mWaveformParam.XCoordinateStart + mWaveformParam.XCoordinateCellSize*j));
			}
			else if (mWaveformParam.XCoordinateCellSize >= 0.1)
			{
				sprintf_s(cStr, "%.1f", mWaveformParam.XCoordinateStart + mWaveformParam.XCoordinateCellSize*j);
			}
			else if (mWaveformParam.XCoordinateCellSize >= 0.01)
			{
				sprintf_s(cStr, "%.2f", mWaveformParam.XCoordinateStart + mWaveformParam.XCoordinateCellSize*j);
			}
			else
			{
				sprintf_s(cStr, "%.3f", mWaveformParam.XCoordinateStart + mWaveformParam.XCoordinateCellSize*j);
			}*/
			/*CString str = CA2T(cStr);
			MemCDC.TextOutW(XWaveformCellSize*j + LeftSorderSize - (int)(str.GetLength()*mWaveformParam.FontWidth / 2), CoordinateOrg.y + 10, str);*/
			MemCDC.SelectStockObject(BLACK_PEN);
			MemCDC.MoveTo(XWaveformCellSize*j + LeftSorderSize, CoordinateOrg.y + 2);
			MemCDC.LineTo(XWaveformCellSize*j + LeftSorderSize, CoordinateOrg.y + 8);
			MemCDC.SelectObject(&SolidPen);
		}
		else
		{
			MemCDC.SelectObject(&DashPen);
		}
		if (XWaveformCellSize*j < CoordinateWidth)
		{
			MemCDC.MoveTo(XWaveformCellSize*j + LeftSorderSize,TopSorderSize);
			MemCDC.LineTo(XWaveformCellSize*j + LeftSorderSize, CoordinateOrg.y);
		}
		//打印Y坐标系标注
		if (j % mWaveformParam.Y_AxisBrokenLineCount == 0 && YWaveformCellSize*j < CoordinateHeight)
		{
			/*if (mWaveformParam.YCoordinateCellSize >= 10)
			{
				sprintf_s(cStr, "%d", (int)(mWaveformParam.YCoordinateStart + mWaveformParam.YCoordinateCellSize*j));
			}
			else if (mWaveformParam.YCoordinateCellSize >= 0.1)
			{
				sprintf_s(cStr, "%.1f", mWaveformParam.YCoordinateStart + mWaveformParam.YCoordinateCellSize*j);
			}
			else if (mWaveformParam.YCoordinateCellSize >= 0.01)
			{
				sprintf_s(cStr, "%.2f", mWaveformParam.YCoordinateStart + mWaveformParam.YCoordinateCellSize*j);
			}
			else
			{
				sprintf_s(cStr, "%.3f", mWaveformParam.YCoordinateStart + mWaveformParam.YCoordinateCellSize*j);
			}*/
			/*CString str = CA2T(cStr);
			MemCDC.TextOutW(LeftSorderSize - 12 - (int)(str.GetLength()*mWaveformParam.FontWidth),
				CoordinateOrg.y - YWaveformCellSize*j - (int)(mWaveformParam.FontHeight / 2), str);*/
			MemCDC.SelectStockObject(BLACK_PEN);
			MemCDC.MoveTo(LeftSorderSize - 8, CoordinateOrg.y - YWaveformCellSize*j);
			MemCDC.LineTo(LeftSorderSize - 2, CoordinateOrg.y - YWaveformCellSize*j);
			MemCDC.SelectObject(&SolidPen);
		}
		else
		{
			MemCDC.SelectObject(&DashPen);
		}
		
		
		if (YWaveformCellSize*j < CoordinateHeight)
		{
			MemCDC.MoveTo(LeftSorderSize, CoordinateOrg.y - YWaveformCellSize*j);
			MemCDC.LineTo(width - RightSorderSize, CoordinateOrg.y - YWaveformCellSize*j);
		}
		j++;
	}
	SolidPen.DeleteObject();
	DashPen.DeleteObject();
	font.DeleteObject();
	/*********************************************************************************************************************************************************************/

}


void CDrawWaveformBitmap::Show(){
	bool isUpdateTotalLine = false;
	mtx.lock();
	map<string, vector<double>>::iterator it;
	it = m_YPoints.begin();
	totalLine.clear();
	while (it != m_YPoints.end())
	{
		if (find(totalLine.begin(), totalLine.end(), it->first) == totalLine.end())
		{
			totalLine.push_back(it->first);
			showLine.push_back(it->first);
			mPointsIndex[it->first] = 0;
			isUpdateTotalLine = true;
		}
		it++;
	}
	if (isUpdateTotalLine)
	{
		DrawGrid();
		DrawLineName();
	}
	LoadGridLine();
	mtx.unlock();
}

void CDrawWaveformBitmap::Show(string line_name){
	mtx.lock();
	if (m_YPoints.find(line_name) != m_YPoints.end())
	{
		if (find(totalLine.begin(), totalLine.end(), line_name) == totalLine.end())
		{
			totalLine.push_back(line_name);
			showLine.push_back(line_name);
			mPointsIndex[line_name] = 0;
			DrawGrid();
			DrawLineName();
			LoadGridLine();
		}
	}
	mtx.unlock();
}

void CDrawWaveformBitmap::Hide(){
	mtx.lock();
	showLine.clear();
	vector<string>(showLine).swap(showLine);
	totalLine.clear();
	vector<string>(totalLine).swap(totalLine);
	LoadGridLine();
	PostMessage(mHwnd, mWmsg, NULL, NULL);
	mtx.unlock();
}
void CDrawWaveformBitmap::Hide(string line_name){
	mtx.lock();
	vector<string>::iterator ite = find(totalLine.begin(), totalLine.end(), line_name);
	if (ite != totalLine.end())
	{
		totalLine.erase(ite); 
		showLine.clear();
		vector<string>(showLine).swap(showLine);
		showLine.insert(showLine.begin(), totalLine.begin(), totalLine.end());
		for (int i = 0; i < showLine.size(); i++)
		{
			mPointsIndex[showLine[i]] = 0;
		}
		LoadGridLine();
		PostMessage(mHwnd, mWmsg, NULL, NULL);
	}
	mtx.unlock();
}

void CDrawWaveformBitmap::Clear(){
	mtx.lock();
	showLine.clear();
	vector<string>(showLine).swap(showLine);
	totalLine.clear();
	vector<string>(totalLine).swap(totalLine);
	map<string, vector<double>>::iterator it;
	it = m_YPoints.begin();

	while (it != m_YPoints.end())
	{
		it->second.clear();
		vector<double>(it->second).swap(it->second);
		m_YPoints.erase(it);
		it = m_YPoints.begin();
	}
	it = m_XPoints.begin();
	while (it != m_XPoints.end())
	{
		it->second.clear();
		vector<double>(it->second).swap(it->second);
		m_XPoints.erase(it);
		it = m_XPoints.begin();
	}

	mPointsColor.clear();
	mPointsIndex.clear(); 
	 X_Axis_Start = 0;
	 Y_Axis_Start = 0;
	 X_Axis_Stop = 0;
	 Y_Axis_Stop = 0;
	LoadGridLine();
	PostMessage(mHwnd, mWmsg, NULL, NULL);
	mtx.unlock();
}


void CDrawWaveformBitmap::ResetScale(double* point, int len, vector<double> &xVec){
	xVec.push_back(0);
	if (len < 2) return;
	if (!X_AxisFixed)
	{
		for (int i = 1; i < len; i++)
		{
			xVec.push_back(i);
		}
		//每个实线格内点数
		int n_count = int((double)len / (mWaveformParam.X_AxisSolidLineCount*mWaveformParam.X_AxisBrokenLineCount) + 1);
		if (m_XPoints.size() <= 1 || (m_XPoints.size() > 1 && X_Axis_Step < n_count))
		{
			X_Axis_Step = 0;
			X_Axis_Step = n_count;
			X_Pixel_Scale = (double)XWaveformCellSize / X_Axis_Step;
		}
	}
	if (!Y_AxisFixed)
	{
		double y_max = point[0], y_min  = point[0];
		for (int i = 1; i < len; i++)
		{
			if (y_max < point[i]) y_max = point[i];
			else if (y_min > point[i]) y_min = point[i];
		}
		
		if (m_XPoints.size() <= 1 || (m_XPoints.size()>1 && (Y_Axis_Start > y_min || Y_Axis_Stop < y_max)))
		{
			if (Y_Axis_Start > y_min) Y_Axis_Start = y_min;
			if (Y_Axis_Stop < y_max) Y_Axis_Stop = y_max;
			double n_count = (Y_Axis_Stop - Y_Axis_Start) / (mWaveformParam.Y_AxisSolidLineCount*mWaveformParam.Y_AxisBrokenLineCount);
			Y_Axis_Step = n_count;
			Y_Pixel_Scale = (double)YWaveformCellSize / Y_Axis_Step;
		}
	}
}



void CDrawWaveformBitmap::ResetScale(double* x_point, double* y_point, int len){
	if (len < 2) return;
	if (!X_AxisFixed)
	{
		double x_max = x_point[0], x_min = x_point[0];
		for (int i = 1; i < len; i++)
		{
			if (x_max < x_point[i]) x_max = x_point[i];
			else if (x_min > x_point[i]) x_min = x_point[i];
		}
		if (m_XPoints.size() <= 1 || (m_XPoints.size()>1 && (X_Axis_Start > x_min || X_Axis_Stop < x_max)))
		{
			if (X_Axis_Start > x_min) X_Axis_Start = x_min;
			if (X_Axis_Stop < x_max) X_Axis_Stop = x_max;
			double n_count = (X_Axis_Stop - X_Axis_Start) / (mWaveformParam.X_AxisSolidLineCount*mWaveformParam.X_AxisBrokenLineCount);
			X_Axis_Step = n_count;
			X_Pixel_Scale = (double)XWaveformCellSize / X_Axis_Step;
		}

	}
	if (!Y_AxisFixed)
	{
		double y_max = y_point[0], y_min = y_point[0];
		for (int i = 1; i < len; i++)
		{
			if (y_max < y_point[i]) y_max = y_point[i];
			else if (y_min > y_point[i]) y_min = y_point[i];
		}
		if (m_XPoints.size() <= 1 || (m_XPoints.size()>1 && (Y_Axis_Start > y_min || Y_Axis_Stop < y_max)))
		{
			if (Y_Axis_Start > y_min) Y_Axis_Start = y_min;
			if (Y_Axis_Stop < y_max) Y_Axis_Stop = y_max;
			double n_count = (Y_Axis_Stop - Y_Axis_Start) / (mWaveformParam.Y_AxisSolidLineCount*mWaveformParam.Y_AxisBrokenLineCount);
			Y_Axis_Step = n_count;
			Y_Pixel_Scale = (double)YWaveformCellSize / Y_Axis_Step;
		}
	}
}

bool CDrawWaveformBitmap::LoadPoint(string line_name, double *point, long length, COLORREF color){
	if (line_name.empty()) return false;
	//if (showLine.size() > 0) return false;
	mtx.lock();
	if (m_YPoints.find(line_name) != m_YPoints.end())
	{
		m_YPoints[line_name].clear();
		vector<double>(m_YPoints[line_name]).swap(m_YPoints[line_name]);
		m_XPoints[line_name].clear();
		vector<double>(m_XPoints[line_name]).swap(m_XPoints[line_name]);
		mPointsColor.at(line_name).DeleteObject();
	}
	m_YPoints[line_name].insert(m_YPoints[line_name].begin(), point + 0, point + length);
	ResetScale(point, length, m_XPoints[line_name]);
	mPointsColor.at(line_name).CreatePen(PS_SOLID, mWaveformParam.WaveformLineWidth, color);
	mPointsIndex[line_name] = 0;
	if (find(totalLine.begin(), totalLine.end(), line_name) != totalLine.end())
	{
		LoadGridLine();
		showLine.clear();
		vector<string>(showLine).swap(showLine);
		showLine.insert(showLine.begin(), totalLine.begin(), totalLine.end());
		for (int i = 0; i < showLine.size(); i++)
		{
			mPointsIndex[showLine[i]] = 0;
		}
	}
	mtx.unlock();
	return true;
}


bool CDrawWaveformBitmap::LoadPoint(string line_name, double *x_point, double *y_point, long length, COLORREF color){
	if (line_name.empty()) return false;
	//if (showLine.size() > 0) return false;
	mtx.lock();
	if (m_YPoints.find(line_name) != m_YPoints.end())
	{
		m_YPoints[line_name].clear();
		vector<double>(m_YPoints[line_name]).swap(m_YPoints[line_name]);
		m_XPoints[line_name].clear();
		vector<double>(m_XPoints[line_name]).swap(m_XPoints[line_name]);
		mPointsColor.at(line_name).DeleteObject();
	}
	m_XPoints[line_name].insert(m_XPoints[line_name].begin(), x_point + 0, x_point + length);
	m_YPoints[line_name].insert(m_YPoints[line_name].begin(), y_point + 0, y_point + length);
	ResetScale(x_point, y_point, length);
	
	mPointsColor[line_name].CreatePen(PS_SOLID, mWaveformParam.WaveformLineWidth, color);
	mPointsIndex[line_name] = 0;
	if (find(totalLine.begin(), totalLine.end(), line_name) != totalLine.end())
	{
		LoadGridLine();
		showLine.clear();
		vector<string>(showLine).swap(showLine);
		showLine.insert(showLine.begin(), totalLine.begin(), totalLine.end());
		for (int i = 0; i < showLine.size(); i++)
		{
			mPointsIndex[showLine[i]] = 0;
		}
	}
	mtx.unlock();
	return true;
}


bool CDrawWaveformBitmap::AddPoint(string line_name, double *point, int length){
	if (line_name.empty()) return false;
	mtx.lock();
	if (m_YPoints.find(line_name) != m_YPoints.end())
	{
		m_YPoints[line_name].insert(m_YPoints[line_name].end(), point + 0, point + length);
		if (find(totalLine.begin(), totalLine.end(), line_name) != totalLine.end())
		{
			if (find(showLine.begin(), showLine.end(), line_name) == showLine.end())
			{
				showLine.push_back(line_name);
			}
		}
	}
	else
	{
		mtx.unlock();
		return false;
	}
	mtx.unlock();
	return true;
}

bool CDrawWaveformBitmap::OpenDrawThread()
{
	bExit = false;
	thread thread_draw_bitmap(&CDrawWaveformBitmap::DrawThread,this);
	thread_draw_bitmap.detach();
	return true;
}

bool CDrawWaveformBitmap::CloseDrawTread()
{
		/** 通知线程退出 */
		bExit = true;
		/** 等待线程退出 */
		Sleep(10);
		return true;
}
void CDrawWaveformBitmap::DrawThread()
{
	clock_t clock_start;
	bool b_last_refresh = true;
	bool b_show_end = false;
	clock_start = clock();
	while (!bExit)
	{
		//Sleep(1);
		//100ms刷新一次界面
		if (b_last_refresh && clock() - clock_start > 100)
		{
			b_last_refresh = false;
			clock_start = clock();
			PostMessage(mHwnd, mWmsg, NULL, NULL);
		}
		if (showLine.size() == 0)
		{
			Sleep(10);
			continue;
		}
		b_last_refresh = true;
		mtx.lock(); // 与数据更新处互斥，防止异常
#if TRUE
		while (showLine.size() > 0)
		{
			try
			{
				string line_name = showLine.front();
				if (mPointsIndex.at(line_name) >= m_YPoints.at(line_name).size() - 1)
				{
					Sleep(5);
					showLine.erase(showLine.begin());
					PostMessage(mHwnd, mWmsg, NULL, NULL);
					break;
				}
				MemCDCWaveform.SelectObject(&mPointsColor.at(line_name));
				//double PointY1 = m_YPoints[line_name][mPointsIndex[line_name]];
				double PointY1 = m_YPoints[line_name][mPointsIndex[line_name]];
				//double PointY2 = m_YPoints[line_name][mPointsIndex[line_name] + 1];
				double PointY2 = m_YPoints.at(line_name).at(mPointsIndex.at(line_name) + 1);
				//double PointX1 = m_XPoints[line_name][mPointsIndex[line_name]];
				double PointX1 = m_XPoints.at(line_name).at(mPointsIndex.at(line_name));
				//double PointX2 = m_XPoints[line_name][mPointsIndex[line_name] + 1];
				double PointX2 = m_XPoints.at(line_name).at(mPointsIndex.at(line_name) + 1);
				POINT Point1 = NormalizationPoint(PointX1, PointY1);
				POINT Point2 = NormalizationPoint(PointX2, PointY2);
				if (Point1.x == Point2.x && Point1.y == Point2.y)
				{
					mPointsIndex.at(line_name) ++;
					break;
				}
				else
				{
					//线程中划线有失败几率，一定要进行判定
					if (MoveToEx(MemCDCWaveform, Point1.x, Point1.y, NULL) && LineTo(MemCDCWaveform, Point2.x, Point2.y))
					{
						/*MemCDCWaveform.MoveTo(Point1);
						MemCDCWaveform.LineTo(Point2);*/
						mPointsIndex.at(line_name) ++;
						break;
					}
				}
			}
			catch(...)
			{
				PostMessage(mHwnd, mWmsg, NULL, NULL);
			}
		}
#else
		POINT Point1 = NormalizationPoint(mPoints[vector_index]);
		if (SetPixelV(MemCDC, Point1.x, Point1.y, RGB(255, 0, 0)))
			vector_index++;
#endif
		mtx.unlock();

	}
}


POINT CDrawWaveformBitmap::NormalizationPoint(double x, double y)
{
	POINT point;
	if (mWaveformParam.GridType == AUDIO_GRID)
	{
		point.x = (int)((log10(x) - X_Axis_Start) *X_Pixel_Scale) + CoordinateOrg.x;
	}
	else
	{
		point.x = (int)((x - X_Axis_Start) *X_Pixel_Scale) + CoordinateOrg.x;
	}
	point.y = -(int)((y - Y_Axis_Start) * Y_Pixel_Scale) + CoordinateOrg.y;
	return point;
}

void CDrawWaveformBitmap::LoadGridLine(){

	
	MemCDCWaveform.BitBlt(0, 0, mWaveformParam.BmpWidth, mWaveformParam.BmpHeight, &MemCDC, 0, 0, SRCCOPY);

	MemCDCWaveform.SetTextColor(mWaveformParam.FontColor);
	MemCDCWaveform.SetBkColor(RGB(0, 0, 60));
	CFont font;
	font.CreateFontW(mWaveformParam.FontHeight, mWaveformParam.FontWidth, 0, 0, 600,
		FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Arial"));
	MemCDCWaveform.SelectObject(&font);
	int j = 0;
	if (mWaveformParam.GridType != AUDIO_GRID)
	{
		while (XWaveformCellSize*j < CoordinateWidth)
		{
			//打印X坐标系标注
			if (j % mWaveformParam.X_AxisBrokenLineCount == 0 && XWaveformCellSize*j < CoordinateWidth)
			{
				char cStr[100] = { '\0' };
				if (X_Axis_Step >= 10)
				{
					sprintf_s(cStr, "%d", (int)(X_Axis_Start + X_Axis_Step*j));
				}
				else if (X_Axis_Step >= 0.1)
				{
					sprintf_s(cStr, "%.1f", X_Axis_Start + X_Axis_Step*j);
				}
				else if (X_Axis_Step >= 0.01)
				{
					sprintf_s(cStr, "%.2f", X_Axis_Start + X_Axis_Step*j);
				}
				else if (X_Axis_Step >= 0.001)
				{
					sprintf_s(cStr, "%.3f", X_Axis_Start + X_Axis_Step*j);
				}
				else
				{
					sprintf_s(cStr, "%.4f", X_Axis_Start + X_Axis_Step*j);
				}
				CString str(cStr);
				MemCDCWaveform.TextOutW(XWaveformCellSize*j + LeftSorderSize - (int)(str.GetLength()*mWaveformParam.FontWidth / 2), CoordinateOrg.y + 10, str);
			}
			j++;
		}

		j = 0;
		while (YWaveformCellSize*j < CoordinateHeight)
		{
			if (j % mWaveformParam.Y_AxisBrokenLineCount == 0 && YWaveformCellSize*j < CoordinateHeight)
			{
				char cStr[100] = { '\0' };
				if (Y_Axis_Step >= 10)
				{
					sprintf_s(cStr, "%d", (int)(Y_Axis_Start + Y_Axis_Step*j));
				}
				else if (Y_Axis_Step >= 0.1)
				{
					sprintf_s(cStr, "%.1f", Y_Axis_Start + Y_Axis_Step*j);
				}
				else if (Y_Axis_Step >= 0.01)
				{
					sprintf_s(cStr, "%.2f", Y_Axis_Start + Y_Axis_Step*j);
				}
				else if (Y_Axis_Step >= 0.001)
				{
					sprintf_s(cStr, "%.3f", Y_Axis_Start + Y_Axis_Step*j);
				}
				else
				{
					sprintf_s(cStr, "%.4f", Y_Axis_Start + Y_Axis_Step*j);
				}
				CString str(cStr);
				MemCDCWaveform.TextOutW(LeftSorderSize - 12 - (int)(str.GetLength()*mWaveformParam.FontWidth),
					CoordinateOrg.y - YWaveformCellSize*j - (int)(mWaveformParam.FontHeight / 2), str);
			}
			j++;
		}
	}
}


void CDrawWaveformBitmap::DrawLineName()
{

	int n_mult = 5;
	int LineWidth = 2;
	CRect rect(mWaveformParam.BmpWidth - RightSorderSize*n_mult - 10,
		TopSorderSize ,
		mWaveformParam.BmpWidth - RightSorderSize,
		TopSorderSize + (mWaveformParam.FontHeight + 2 + 4)*totalLine.size());
	CBrush drawBrush;
	drawBrush.CreateSolidBrush(RGB(0x60, 0x60, 0x60));
	MemCDC.FillRect(rect, &drawBrush);

	MemCDC.SetBkColor(RGB(0x60, 0x60, 0x60));
	CFont font;
	font.CreateFontW(mWaveformParam.FontHeight, mWaveformParam.FontWidth, 0, 0, 600,
		FALSE, FALSE, FALSE, 0, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_ROMAN, _T("Arial"));
	MemCDC.SelectObject(&font);
	
	for (int i = 0; i < totalLine.size(); i++)
	{
		
		LOGPEN lPen;
		//mPointsColor[totalLine[i]].GetLogPen(&lPen);

		/*if (mPointsColor.size() == 0)
		{
			string str = "jiliset";
			pair<string, CPen> p1= { str, CPen() };
			mPointsColor.insert(p1);
		}*/
		mPointsColor.at(totalLine.at(i)).GetLogPen(&lPen);
		CPen cPen;
		cPen.CreatePen(lPen.lopnStyle, LineWidth, lPen.lopnColor);
		MemCDC.SelectObject(&cPen);

		MoveToEx(MemCDC, mWaveformParam.BmpWidth - RightSorderSize * n_mult - 10 + 2,
			TopSorderSize + (mWaveformParam.FontHeight + 2 + 4)*i + (mWaveformParam.FontHeight + 2) / 2 , NULL);
		LineTo(MemCDC, mWaveformParam.BmpWidth - RightSorderSize * n_mult - 10 + 2 + mWaveformParam.BorderWidth / 2,
			TopSorderSize + (mWaveformParam.FontHeight + 2 + 4)*i + (mWaveformParam.FontHeight + 2) / 2 );
		CString cStr = CA2T(totalLine[i].c_str());
		MemCDC.SetTextColor(RGB(255,255,255));
		MemCDC.TextOutW(mWaveformParam.BmpWidth - RightSorderSize * n_mult - 10 + 2 + mWaveformParam.BorderWidth / 2 + 4,
			TopSorderSize + (mWaveformParam.FontHeight + 2 + 4)*i + 1, cStr);
		cPen.DeleteObject();
		drawBrush.DeleteObject();
	 
		Sleep(2);
	}

	font.DeleteObject();
}
