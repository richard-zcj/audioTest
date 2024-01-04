#include "stdafx.h"
#include "SerialPort.h"


CSerialPortDriver::CSerialPortDriver()
{
    Port = -1;
    Baud = 9600;
    DataBits = 8;
    StopBits = 1;
    Parity = 'n';
	IsComPort = false;
	IsUsbToCom = false;
	m_hHandle = INVALID_HANDLE_VALUE;
}

CSerialPortDriver::~CSerialPortDriver()
{
	Close();
}

void CSerialPortDriver::Open()
{
	char szPort[32] = "\0";
	FWriteEvent = NULL;
	FReadEvent = NULL;

	if (Port <= 100 && Port > 0)
	{
		IsComPort = true;
	}

	//char port_config[100]="\0";

	if (IsComPort)
	{
		sprintf_s(szPort, "\\\\.\\COM%d", Port);
		//sprintf_s(port_config,"baud=%d parity=%c data=%d stop=%d", Baud, Parity, DataBits, StopBits);
	}
	else
	{
		sprintf_s(szPort, "\\\\.\\LPT%d", Port - 100);
	}

	m_hHandle = CreateFileA(szPort, GENERIC_READ | GENERIC_WRITE , 0,
		NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, NULL);
	if (m_hHandle == INVALID_HANDLE_VALUE)
	{
		// 假如初始化失败
		if (GetLastError() == 5)
		{
			throw EDeviceError(DEV_E_PORT_REFUSE_VISIT, _T("The port refuses to visit."));
		}
		else
		{
			throw EDeviceError(DEV_E_PORT_NOT_EXISTS, _T("The port not exists."));
		}
	}

	if (IsComPort)
	{
		DCB dcb;
		memset(&dcb, 0, sizeof(DCB));
		dcb.DCBlength = sizeof(DCB);

		if (!GetCommState(m_hHandle, &dcb))
		{
			Close();
			throw EDeviceError(DEV_E_PORT_OPERATION, _T("Config the port parameter defeat."));
		}

		// 波特率设置
		if (Baud == 300)
		{
			dcb.BaudRate = CBR_300;
		}
		else if (Baud == 600)
		{
			dcb.BaudRate = CBR_600;
		}
		else if (Baud == 1200)
		{
			dcb.BaudRate = CBR_1200;
		}
		else if (Baud == 2400)
		{
			dcb.BaudRate = CBR_2400;
		}
		else if (Baud == 4800)
		{
			dcb.BaudRate = CBR_4800;
		}
		else if (Baud == 19200)
		{
			dcb.BaudRate = CBR_19200;
		}
		else if (Baud == 38400)
		{
			dcb.BaudRate = CBR_38400;
		}
		else if (Baud == 56000)
		{
			dcb.BaudRate = CBR_56000;
		}
		else if (Baud == 57600)
		{
			dcb.BaudRate = CBR_57600;
		}
		else if (Baud == 115200)
		{
			dcb.BaudRate = CBR_115200;
		}
		else
		{
			dcb.BaudRate = Baud;
		}

		//dcb.BaudRate = Baud; //CBR_38400
		// 数据位设置
		dcb.ByteSize = DataBits;

		// 停止位设置
		if (StopBits == 2)
		{
			dcb.StopBits = TWOSTOPBITS;
		}
		else
		{
			dcb.StopBits = ONESTOPBIT;
		}

		//dcb.StopBits = StopBits; //TWOSTOPBITS;

		// 奇偶校验设置
		if (Parity == 'e' || Parity == 'E')
		{
			dcb.Parity = EVENPARITY;
		}
		else if (Parity == 'o' || Parity == 'O')
		{
			dcb.Parity = ODDPARITY;
		}
		else
		{
			dcb.Parity = NOPARITY;
		}

		dcb.fRtsControl = RTS_CONTROL_HANDSHAKE; //@ RTS_CONTROL_ENABLE; //控制接收，/RTS有效

		if (!SetCommState(m_hHandle, &dcb))
		{
			Close();
			throw EDeviceError(DEV_E_SET_PORT_PARAM, _T("Config the port parameter defeat."));
		}

		COMMTIMEOUTS  CommTimeOuts;
		if (!GetCommTimeouts(m_hHandle, &CommTimeOuts))
		{
			Close(); 
			throw EDeviceError(DEV_E_PORT_OPERATION, _T("Config the port parameter defeat."));
		}

		CommTimeOuts.ReadIntervalTimeout = MAXDWORD;// 0xFFFFFFFF;
		CommTimeOuts.ReadTotalTimeoutMultiplier = 0; // 500;
		CommTimeOuts.ReadTotalTimeoutConstant = 0; // 200; // 不自动处理超时
		CommTimeOuts.WriteTotalTimeoutMultiplier = 0; // 20000;
		CommTimeOuts.WriteTotalTimeoutConstant = 5000; // 200; // 不自动处理超时
		SetupComm(m_hHandle, 0x800, 0x800);

		if (!SetCommTimeouts(m_hHandle, &CommTimeOuts))
		{
			Close();
			throw EDeviceError(DEV_E_SET_PORT_PARAM, _T("Config the port parameter defeat."));
		}
	}

	FWriteEvent = CreateEvent( NULL, TRUE, FALSE, NULL);
	FReadEvent = CreateEvent( NULL, TRUE, FALSE, NULL);
}

void CSerialPortDriver::Close()
{
	DiscardIO(true, true);

	::CloseHandle(m_hHandle);
	m_hHandle = INVALID_HANDLE_VALUE;
	if(FWriteEvent)
		::CloseHandle(FWriteEvent);
	if(FReadEvent)
		::CloseHandle(FReadEvent);
	FReadEvent = NULL;
	FWriteEvent = NULL;
}

bool CSerialPortDriver::IsOpen()
{
	if (m_hHandle == INVALID_HANDLE_VALUE)
	{
		return false;
	}

	return true;
}

void CSerialPortDriver::DiscardIO(bool p_bInput, bool p_bOutput)
{
	if (!IsComPort)
	{
		return;
	}

	if (!p_bInput && !p_bOutput)
	{
		return;
	}

	DWORD dwFlags = 0;
	if (p_bInput)
	{
		dwFlags |= PURGE_RXABORT | PURGE_RXCLEAR;
	}

	if(p_bOutput)
	{
		dwFlags |= PURGE_TXABORT | PURGE_TXCLEAR;
	}

	PurgeComm(m_hHandle, dwFlags);
}

int CSerialPortDriver::ReadReady(int p_nTimeout)
{
    if (IsComPort)
	{
        DWORD dwPrev = GetTickCount();
        while ((GetTickCount() - dwPrev) < (unsigned)p_nTimeout)
		{
            COMSTAT ComStat;
            DWORD   dwErrorFlags;
            ::ClearCommError(m_hHandle, &dwErrorFlags, &ComStat);
            if (ComStat.cbInQue > 0)
			{
                return 0;
            }

            ::Sleep(10);
        }
    }
	else
	{
        return 0;
	}

	return 1;
}

int CSerialPortDriver::WriteReady(int p_nTimeout)
{
	return 0;
}

int CSerialPortDriver::Read(void* p_pBuffer, unsigned int p_nMaxCount, unsigned int p_nTimeout)
{
	unsigned long nRet = p_nMaxCount;

	switch(ReadReady(p_nTimeout))
	{
	case 1:
		return 0;
	case -1:
		return -1;
	}

	OVERLAPPED stOver;
	stOver.hEvent = FReadEvent;
	stOver.Offset = 0;
    stOver.OffsetHigh = 0;
	ResetEvent(FReadEvent);
    
	if (IsComPort)
	{
        COMSTAT  ComStat;
        DWORD    dwErrorFlags;
        ::ClearCommError(m_hHandle, &dwErrorFlags, &ComStat);
        if (nRet > ComStat.cbInQue)
		{
			nRet = ComStat.cbInQue;
		}
    }

	if (!ReadFile(m_hHandle, p_pBuffer, nRet, &nRet, &stOver))
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
        	int nTimeout = p_nTimeout*1000;
			if (nTimeout <= 0)
			{
				nTimeout = 1000;
			}

			int nWait = WaitForSingleObject(FReadEvent, nTimeout);
			if (nWait != WAIT_OBJECT_0)
			{
				return 0;
			}

			if (!GetOverlappedResult(m_hHandle, &stOver, &nRet, false))
			{
				return 0;
			}
		}
		else
		{
			throw EDeviceError(DEV_E_READ_DATA, _T("Read error."));
		}
	}

	return nRet;
}

int CSerialPortDriver::Write(void* p_pBuffer, unsigned int p_nMaxCount, unsigned int p_nTimeout)
{             
	unsigned long nRet = p_nMaxCount;

	switch(WriteReady(p_nTimeout))
	{
	case 1:
		return 0;
	case -1:
		return -1;
	}         

	OVERLAPPED stOver;
	stOver.hEvent = FWriteEvent;
	stOver.Offset = 0;
    stOver.OffsetHigh = 0;
	ResetEvent(FWriteEvent);

	if (!WriteFile(m_hHandle, p_pBuffer, nRet, &nRet,&stOver))
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
        	int nTimeout = p_nTimeout*1000;
			if (nTimeout <= 0)
			{
				nTimeout = 1000;
			}

			int nWait = WaitForSingleObject(FWriteEvent, nTimeout);
			if (nWait != WAIT_OBJECT_0)
			{
				return 0;
			}

			if (!GetOverlappedResult(m_hHandle, &stOver, &nRet, false))
			{
				return 0;
			}
		}
		else
		{
			throw EDeviceError(DEV_E_WRITE_DATA, _T("Write error."));
		}
	}

	FlushFileBuffers(m_hHandle);
	return nRet;
}

void CSerialPortDriver::ReadBuffer(void* p_pBuffer, unsigned int p_nCount, unsigned int p_nTimeout)
{
	char* pBuffer = (char*)p_pBuffer;
	int nLeftLen = p_nCount;
	unsigned long ulCurrent = GetTickCount();

	while (nLeftLen > 0 && (unsigned int)(GetTickCount() - ulCurrent) < p_nTimeout)
	{
		int nLen = Read(pBuffer, nLeftLen, p_nTimeout);
		//printf("Read len: %d, data: %s\n", nLen, pBuffer);
		if (nLen > 0)
		{
			ulCurrent = GetTickCount();
			nLeftLen -= nLen;
			pBuffer += nLen;
			
			if (nLeftLen <= 0)
			{
				break;
			}
		}
	}

	if (nLeftLen > 0)
	{
		throw EDeviceError(DEV_E_READ_TIMEOUT, _T("Read timeout"));
	}
}

void CSerialPortDriver::WriteBuffer(void* p_pBuffer, unsigned int p_nCount, unsigned int p_nTimeout)
{
	char* pBuffer = (char*)p_pBuffer;
	int nLeftLen = p_nCount;
	unsigned long ulCurrent = GetTickCount();

	while (nLeftLen > 0 && (unsigned int)(GetTickCount() - ulCurrent) < p_nTimeout)
	{
		int nLen = Write(pBuffer, nLeftLen, p_nTimeout);
		if (nLen > 0)
		{
			nLeftLen -= nLen;
			pBuffer += nLen;
			if (nLeftLen <= 0)
			{
				break;
			}
		}
	}

	if (nLeftLen > 0)
	{
		throw EDeviceError(DEV_E_WRITE_TIMEOUT, _T("Write timeout"));
	}
}

bool CSerialPortDriver::GetRTS()
{
	bool bRet = false;
	return bRet;
}

bool CSerialPortDriver::GetCTS()
{
	bool bRet = false;
	DWORD dwStat = 0;
	GetCommModemStatus(m_hHandle, &dwStat);
	bRet = (dwStat & MS_CTS_ON) == MS_CTS_ON;
	return bRet;
}

bool CSerialPortDriver::GetDTR()
{
	bool bRet = false;
	return bRet;
}

bool CSerialPortDriver::GetDSR()
{
	bool bRet = false;
	DWORD dwStat = 0;
	GetCommModemStatus(m_hHandle, &dwStat);
	bRet = (dwStat & MS_DSR_ON) == MS_DSR_ON;
	return bRet;
}

bool CSerialPortDriver::SetRTS(bool p_bHaveSingal)
{
    if (!IsComPort)
	{
		return true;
	}

	bool bRet = false;

	if (p_bHaveSingal)
	{
		bRet = EscapeCommFunction(m_hHandle, SETRTS) == TRUE;
	}
	else
	{
		bRet = EscapeCommFunction(m_hHandle, CLRRTS) == TRUE;
	}

	return bRet;
}

bool CSerialPortDriver::SetCTS(bool p_bHaveSingal)
{
    if (!IsComPort)
	{
		return true;
	}

	bool bRet = false;
	return bRet;
}

bool CSerialPortDriver::SetDTR(bool p_bHaveSingal)
{
    if (!IsComPort)
	{
		return true;
	}

	bool bRet = false;

	if (p_bHaveSingal)
	{
		bRet = EscapeCommFunction(m_hHandle, SETDTR) == TRUE;
	}
	else
	{
		bRet = EscapeCommFunction(m_hHandle, CLRDTR) == TRUE;
	}

	return bRet;
}

bool CSerialPortDriver::SetDSR(bool p_bHaveSingal)
{
    if (!IsComPort)
	{
		return true;
	}

	bool bRet = false;
	return bRet;
}

