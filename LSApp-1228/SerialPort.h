#ifndef _SERIAL_PORT_H_
#define _SERIAL_PORT_H_

// ����������Ϣ
#define DEV_SUCCESS					 0		// ����
#define DEV_E_PORT_NOT_EXISTS		-1001	// �˿ڲ�����
#define DEV_E_PORT_REFUSE_VISIT		-1002	// �˿ھܾ�����
#define DEV_E_SET_PORT_PARAM		-1003	// ���ô��ڲ���ʧ��
#define DEV_E_READ_TIMEOUT			-1004	// �����ݳ�ʱ
#define DEV_E_WRITE_TIMEOUT			-1005	// д���ݳ�ʱ
#define DEV_E_READ_DATA				-1006	// �����ݴ���
#define DEV_E_WRITE_DATA			-1007	// д���ݴ���
#define DEV_E_PORT_OPERATION		-1008	// ���ڲ�������
#define DEV_E_UNKNOWN				-1099	// δ֪����

// �쳣����
typedef CString Exception;

// �豸�쳣��
class EDeviceError
{
public:
	/**
	 * �豸�쳣���캯��
	 * @param ErrorCode �����룬������������������壬����Ĺ�������:
	 *                  =0 - ����;
	 *                  >0 - ����;
	 *                  <0 - ����;
	 * @param Message ������Ϣ
	 */
	EDeviceError(int p_nErrorCode, CString p_sMessage)
	{
		ErrorCode = p_nErrorCode;
		Message = p_sMessage;
	};

	/**
	 * �����룬������������������壬����Ĺ�������:
	 *                  =0 - ����;
	 *                  >0 - ����;
	 *                  <0 - ����;
	 */
	int ErrorCode;
	CString Message;
};

/**
 * ���ڴ��ڵ�����
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
