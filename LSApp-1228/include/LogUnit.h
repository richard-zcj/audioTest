/**********************************************************
 *
 * FileName:    LogUnit.h
 * Function:    ��־
 * Description: ͨ��"debug.ini"���Ƿ��������Debugģʽ
 *              ȱʡ��־�ļ���Ϊ����YYYY-MM-DD.log��
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
// ������: ClearOverdueLog(LPCTSTR p_pLogPath, int p_nDays, LPCTSTR p_pType)
// ����:   ���������־
// ���:   p_pLogPath	��־�ļ��洢·��
//         p_nKeepDays	��־�ļ���������
//         p_pType		��־�ļ����ͣ�Ĭ���������ͣ�*.*��
// ����:   ��
// ����:   ��
// ����:   ������ͬ������ϵͳʱ������
//******************************************************************************
extern void ClearOverdueLog(LPCTSTR p_pLogPath, int p_nKeepDays,
	LPCTSTR p_pType = _T("*.*"));

//******************************************************************************
// ������: InitLogService(int p_nKeepDays = DEFAULT_KEEPDAYS)
//         InitLogService(LPCTSTR p_pLogPath, int p_nKeepDays = DEFAULT_KEEPDAYS)
// ����:   ��ʼ��������ʵ����
// ���:   p_pLogPath     ��־�ļ��洢·�������Ϊ�գ�Ĭ��Ϊ��D:\\log\\��
//         p_pFilePrefix  ��־�ļ�ǰ����ƣ����Ϊ�գ��Ե�ǰ����������
//						  ����Ϊ��FileName_yyyy-mm-dd.log��
//         p_nKeepDays    ���������־������Ĭ��0�������
// ����:   ��
// ����:   ��־�����
// ����:   ��
//******************************************************************************
extern CLogService* InitLogService(int p_nKeepDays = 0);
extern CLogService* InitLogService(LPCTSTR p_pLogPath, int p_nKeepDays = 0);

//******************************************************************************
// ������: DestroyLogService()
// ����:   �ͷź������ͷ������
// ���:   ��
// ����:   ��
// ����:   ��
// ����:   ��
//******************************************************************************
extern void DestroyLogService();

//******************************************************************************
// ������: SetLogFileAttr(LPCTSTR pName, LPCTSTR pValue)
// ����:   ����log�ļ����ԣ�����struct ST_LOG_FILE_ATTR���ݣ��ýӿڿɵ��ö��
// ���:   pName	������, ���Ϊ�գ���ָ�Ĭ��ֵ
//			"ExtName": log��չ�����磺.log, .txt
//			"Prefix": log�ļ���ǰ׺
//			"DateFormat": log�ļ����е����ڸ�ʽ���磺YYYY-MM-DD
//			"TimeFormat": log�����е�ʱ�����ʽ���磺HH:NN:SS.ZZZ��ZZZ�Ǻ��룩
//		   pValue	����ֵ
// ����:   ��
// ����:   ��
// ����:   ��
//******************************************************************************
extern void SetLogFileAttr(LPCTSTR pName, LPCTSTR pValue);

//******************************************************************************
// ������: WriteInfoLog(LPCTSTR p_pFormat, ...);
//         WriteInfoLogEx(LPCTSTR p_pModule, LPCTSTR p_pFormat, ...);
// ����:   д������Ϣ��־
// ���:   p_pModule   ��־ģ�飬�����Ϊ�գ��Ѹ���־�����ģ������Ӧ�ļ�����
//         p_pFormat   ��ʽ�����ַ���  
//         ...         ���������б�
// ����:   ��
// ����:   ��
// ����:   ��
//******************************************************************************
extern void WriteInfoLog(LPCTSTR p_pFormat, ...);
extern void WriteInfoLogEx(LPCTSTR p_pModule, LPCTSTR p_pFormat, ...);

//******************************************************************************
// ������: WriteDebugLog(LPCTSTR p_pFormat, ...);   
//         WriteDebugLogPos(LPCTSTR p_pFile, int p_nLine, LPCTSTR p_pFormat, ...);
//         WriteDebugLogEx(LPCTSTR p_pModule, LPCTSTR p_pFormat, ...);
//         WriteDebugLogPosEx(LPCTSTR p_pModule, LPCTSTR p_pFile, int p_nLine,
//				LPCTSTR p_pFormat, ...);
// ����:   д������Ϣ��־������ͨ����־·����"debug.ini"���Ƿ������ȷ���Ƿ��ӡ������Ϣ
// ���:   p_pModule   ��־ģ�飬�����Ϊ�գ��Ѹ���־�����ģ���ļ�����
//         p_pFile     ��ӡ����־��Դ�����ļ���
//         p_nLine     ��ӡ����־��Դ��������
//         p_pFormat   ��ʽ�����ַ���
//         ...         ���������б�
// ����:   ��
// ����:   ��
// ����:   ��
//******************************************************************************
extern void WriteDebugLog(LPCTSTR p_pFormat, ...);
extern void WriteDebugLogPos(LPCTSTR p_pFile, int p_nLine, LPCTSTR p_pFormat, ...);
extern void WriteDebugLogEx(LPCTSTR p_pModule, LPCTSTR p_pFormat, ...);
extern void WriteDebugLogPosEx(LPCTSTR p_pModule, LPCTSTR p_pFile, int p_nLine,
	LPCTSTR p_pFormat, ...);

//******************************************************************************
// ������: WriteErrorLog(LPCTSTR p_pFormat, ...);
//         WriteErrorLogPos(LPCTSTR p_pFile, int p_nLine, LPCTSTR p_pFormat, ...);
//         WriteErrorLogEx(LPCTSTR p_pModule, LPCTSTR p_pFormat, ...);
//         WriteErrorLogPosEx(LPCTSTR p_pModule, LPCTSTR p_pFile, int p_nLine,
//				LPCTSTR p_pFormat, ...);
// ����:   д������Ϣ��־
// ���:   p_pModule   ��־ģ�飬�����Ϊ�գ��Ѹ���־�����ģ���ļ�����
//         p_pFile     ��ӡ����־��Դ�����ļ���
//         p_nLine     ��ӡ����־��Դ��������
//         p_pFormat   ��ʽ�����ַ���
//         ...         ���������б�
// ����:   ��
// ����:   ��
// ����:   ��
//******************************************************************************
extern void WriteErrorLog(LPCTSTR p_pFormat, ...);
extern void WriteErrorLogPos(LPCTSTR p_pFile, int p_nLine, LPCTSTR p_pFormat, ...);
extern void WriteErrorLogEx(LPCTSTR p_pModule, LPCTSTR p_pFormat, ...);
extern void WriteErrorLogPosEx(LPCTSTR p_pModule, LPCTSTR p_pFile, int p_nLine,
	LPCTSTR p_pFormat, ...);

#endif  // _LOG_UNIT_H_

