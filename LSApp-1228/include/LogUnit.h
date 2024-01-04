/**********************************************************
 *
 * FileName:    LogUnit.h
 * Function:    日志
 * Description: 通过"debug.ini"的是否存在来打开Debug模式
 *              缺省日志文件名为：“YYYY-MM-DD.log”
 * CreateTime:  2012.09.28
 * Author:      liguangjun
 * History:
 *
**********************************************************/
#ifndef _LOG_UNIT_H_
#define _LOG_UNIT_H_

#define _FL_  CString(__FILE__), __LINE__
#define _DT_  __DATE__, __TIME__

class CLogService;

//******************************************************************************
// 函数名: ClearOverdueLog(LPCTSTR p_pLogPath, int p_nDays, LPCTSTR p_pType)
// 功能:   清除过期日志
// 入参:   p_pLogPath	日志文件存储路径
//         p_nKeepDays	日志文件保留天数
//         p_pType		日志文件类型，默认所有类型（*.*）
// 出参:   无
// 返回:   空
// 其它:   建议在同步操作系统时间后调用
//******************************************************************************
extern void ClearOverdueLog(LPCTSTR p_pLogPath, int p_nKeepDays,
	LPCTSTR p_pType = _T("*.*"));

//******************************************************************************
// 函数名: InitLogService(int p_nKeepDays = DEFAULT_KEEPDAYS)
//         InitLogService(LPCTSTR p_pLogPath, int p_nKeepDays = DEFAULT_KEEPDAYS)
// 功能:   初始化函数，实例化
// 入参:   p_pLogPath     日志文件存储路径，如果为空，默认为“D:\\log\\”
//         p_pFilePrefix  日志文件前辍名称，如果为空，以当前日期命名，
//						  否则为“FileName_yyyy-mm-dd.log”
//         p_nKeepDays    保存最近日志天数，默认0，不清除
// 出参:   无
// 返回:   日志类对象
// 其它:   无
//******************************************************************************
extern CLogService* InitLogService(int p_nKeepDays = 0);
extern CLogService* InitLogService(LPCTSTR p_pLogPath, int p_nKeepDays = 0);

//******************************************************************************
// 函数名: DestroyLogService()
// 功能:   释放函数，释放类对象
// 入参:   无
// 出参:   无
// 返回:   空
// 其它:   无
//******************************************************************************
extern void DestroyLogService();

//******************************************************************************
// 函数名: SetLogFileAttr(LPCTSTR pName, LPCTSTR pValue)
// 功能:   设置log文件属性，设置struct ST_LOG_FILE_ATTR数据，该接口可调用多次
// 入参:   pName	属性名, 如果为空，则恢复默认值
//			"ExtName": log扩展名，如：.log, .txt
//			"Prefix": log文件名前缀
//			"DateFormat": log文件名中的日期格式，如：YYYY-MM-DD
//			"TimeFormat": log内容中的时间戳格式，如：HH:NN:SS.ZZZ（ZZZ是毫秒）
//		   pValue	属性值
// 出参:   无
// 返回:   空
// 其它:   无
//******************************************************************************
extern void SetLogFileAttr(LPCTSTR pName, LPCTSTR pValue);

//******************************************************************************
// 函数名: WriteInfoLog(LPCTSTR p_pFormat, ...);
//         WriteInfoLogEx(LPCTSTR p_pModule, LPCTSTR p_pFormat, ...);
// 功能:   写正常信息日志
// 入参:   p_pModule   日志模块，如果不为空，把该日志存放在模块名对应文件夹中
//         p_pFormat   格式控制字符串  
//         ...         不定变量列表
// 出参:   无
// 返回:   空
// 其它:   无
//******************************************************************************
extern void WriteInfoLog(LPCTSTR p_pFormat, ...);
extern void WriteInfoLogEx(LPCTSTR p_pModule, LPCTSTR p_pFormat, ...);

//******************************************************************************
// 函数名: WriteDebugLog(LPCTSTR p_pFormat, ...);   
//         WriteDebugLogPos(LPCTSTR p_pFile, int p_nLine, LPCTSTR p_pFormat, ...);
//         WriteDebugLogEx(LPCTSTR p_pModule, LPCTSTR p_pFormat, ...);
//         WriteDebugLogPosEx(LPCTSTR p_pModule, LPCTSTR p_pFile, int p_nLine,
//				LPCTSTR p_pFormat, ...);
// 功能:   写调试信息日志，可以通过日志路径下"debug.ini"的是否存在来确认是否打印调试信息
// 入参:   p_pModule   日志模块，如果不为空，把该日志存放在模块文件夹中
//         p_pFile     打印该日志的源代码文件名
//         p_nLine     打印该日志的源代码行数
//         p_pFormat   格式控制字符串
//         ...         不定变量列表
// 出参:   无
// 返回:   空
// 其它:   无
//******************************************************************************
extern void WriteDebugLog(LPCTSTR p_pFormat, ...);
extern void WriteDebugLogPos(LPCTSTR p_pFile, int p_nLine, LPCTSTR p_pFormat, ...);
extern void WriteDebugLogEx(LPCTSTR p_pModule, LPCTSTR p_pFormat, ...);
extern void WriteDebugLogPosEx(LPCTSTR p_pModule, LPCTSTR p_pFile, int p_nLine,
	LPCTSTR p_pFormat, ...);

//******************************************************************************
// 函数名: WriteErrorLog(LPCTSTR p_pFormat, ...);
//         WriteErrorLogPos(LPCTSTR p_pFile, int p_nLine, LPCTSTR p_pFormat, ...);
//         WriteErrorLogEx(LPCTSTR p_pModule, LPCTSTR p_pFormat, ...);
//         WriteErrorLogPosEx(LPCTSTR p_pModule, LPCTSTR p_pFile, int p_nLine,
//				LPCTSTR p_pFormat, ...);
// 功能:   写错误信息日志
// 入参:   p_pModule   日志模块，如果不为空，把该日志存放在模块文件夹中
//         p_pFile     打印该日志的源代码文件名
//         p_nLine     打印该日志的源代码行数
//         p_pFormat   格式控制字符串
//         ...         不定变量列表
// 出参:   无
// 返回:   空
// 其它:   无
//******************************************************************************
extern void WriteErrorLog(LPCTSTR p_pFormat, ...);
extern void WriteErrorLogPos(LPCTSTR p_pFile, int p_nLine, LPCTSTR p_pFormat, ...);
extern void WriteErrorLogEx(LPCTSTR p_pModule, LPCTSTR p_pFormat, ...);
extern void WriteErrorLogPosEx(LPCTSTR p_pModule, LPCTSTR p_pFile, int p_nLine,
	LPCTSTR p_pFormat, ...);

#endif  // _LOG_UNIT_H_

