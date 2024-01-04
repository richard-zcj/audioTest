//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2003-2008, Michael Tippach
//
// Use of this work is subject to the ASIO4ALL Private API SDK license
// agreement.
//
//                            !!! PRELIMINARY !!!
//
//////////////////////////////////////////////////////////////////////////////

#pragma once
#include "iaa4aprv.h"

class a4aInterface
{
public:
	a4aInterface(void);
	~a4aInterface(void);

	DWORD GetVersion(void);
	void Enumerate(void);
	void SetCallback(BOOL Callback(void*), void* pCbData);
	A4AError GetDeviceProperty(A4ADeviceProperty DeviceProperty, long DeviceIndex, void* PropertyData);
	A4AError SetDeviceProperty(A4ADeviceProperty DeviceProperty, long DeviceIndex, void* PropertyData);
	A4AError GetInterfaceProperty(A4AInterfaceProperty InterfaceProperty, long DeviceIndex, long InterfaceIndex, void* PropertyData);
	A4AError SetInterfaceProperty(A4AInterfaceProperty InterfaceProperty, long DeviceIndex, long InterfaceIndex, void* PropertyData);
	A4AError GetPinProperty(A4APinProperty PinProperty, long DeviceIndex, long InterfaceIndex, long PinIndex, void* PropertyData);
	A4AError SetPinProperty(A4APinProperty PinProperty, long DeviceIndex, long InterfaceIndex, long PinIndex, void* PropertyData);

private:
	IA4APRIVATE* ifcInstance;
};
