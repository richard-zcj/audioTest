#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

// 基本错误信息
#define DEV_SUCCESS					 0		// 正常
#define DEV_E_PORT_NOT_EXISTS		-1001	// 端口不存在
#define DEV_E_PORT_REFUSE_VISIT		-1002	// 端口拒绝访问
#define DEV_E_SET_PORT_PARAM		-1003	// 设置串口参数失败
#define DEV_E_READ_TIMEOUT			-1004	// 读数据超时
#define DEV_E_WRITE_TIMEOUT			-1005	// 写数据超时
#define DEV_E_READ_DATA				-1006	// 读数据错误
#define DEV_E_WRITE_DATA			-1007	// 写数据错误
#define DEV_E_PORT_OPERATION		-1008	// 串口操作错误
#define DEV_E_UNKNOWN				-1099	// 未知错误

// 异常类型
typedef CString Exception;

// 设备异常类
class EDeviceError
{
public:
	/**
	 * 设备异常构造函数
	 * @param ErrorCode 错误码，具体错误码由驱动定义，定义的规则如下:
	 *                  =0 - 正常;
	 *                  >0 - 警告;
	 *                  <0 - 错误;
	 * @param Message 错误信息
	 */
	EDeviceError(int p_nErrorCode, CString p_sMessage)
	{
		ErrorCode = p_nErrorCode;
		Message = p_sMessage;
	};

	/**
	 * 错误码，具体错误码由驱动定义，定义的规则如下:
	 *                  =0 - 正常;
	 *                  >0 - 警告;
	 *                  <0 - 错误;
	 */
	int ErrorCode;
	CString Message;
};

/**
 * 基于串口的驱动
 */
class CSerialPortDriver
{
protected:
	HANDLE m_hHandle;

	void *FReadEvent,*FWriteEvent;
	int ReadReady(int p_nTimeout);
	int WriteReady(int p_nTimeout);

public:
	CSerialPortDriver();
	~CSerialPortDriver();

	void Open();
	void Close();
	bool IsOpen();

	int Read(void* p_pBuffer, unsigned int p_nMaxCount, unsigned int p_nTimeout);
	int Write(void* p_pBuffer, unsigned int p_nMaxCount, unsigned int p_nTimeout);
    void ReadBuffer(void* p_pBuffer, unsigned int p_nCount, unsigned int p_nTimeout);
    void WriteBuffer(void* p_pBuffer, unsigned int p_nCount, unsigned int p_nTimeout);
	void DiscardIO(bool p_bInput, bool p_bOutput);
	bool GetRTS();
	bool GetCTS();
	bool GetDTR();
	bool GetDSR();

	bool SetRTS(bool p_bHaveSingal);
	bool SetCTS(bool p_bHaveSingal);
	bool SetDTR(bool p_bHaveSingal);
	bool SetDSR(bool p_bHaveSingal);

public:
    int Port;
    int Baud;
    int DataBits;
    int StopBits;
	char Parity;
	bool IsComPort;
	bool IsUsbToCom;
};
#endif  // _SERIAL_PORT_H_
