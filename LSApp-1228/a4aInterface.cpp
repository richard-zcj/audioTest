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

#include "StdAfx.h"
#include "a4aInterface.h"


a4aInterface::a4aInterface(void)
{
	ifcInstance = NULL;
	//
	// For backwards compatibility with the 1.0 API, we also check for a valid CLSID.
	// Note that creating another CLSID in the 1.0 API was a pretty stupid design decision!
	// In ref 2.0 of the API, you will be able to discover an IA4APRIVATE on any ASIO4ALL based driver IASIO interface.
	//
	CoCreateInstance(IID_IA4APRIVATE, 0, 1, IID_IA4APRIVATE, (LPVOID*)&ifcInstance);
	if (ifcInstance == NULL)
	{
		CoCreateInstance(IID_IA4A, 0, 1, IID_IA4APRIVATE, (LPVOID*)&ifcInstance);
	}
}

a4aInterface::~a4aInterface(void)
{
	if (ifcInstance) ifcInstance->Release();
}

DWORD a4aInterface::GetVersion(void)
{
	if (ifcInstance == NULL) return 0;
	return (ifcInstance->iA4APrivateGetVersion());
}

void a4aInterface::Enumerate(void)
{
	if (ifcInstance == NULL) return;
	ifcInstance->iA4APrivateEnumerate();
}

void a4aInterface::SetCallback(BOOL Callback(void*), void* pCbData)
{
	if (ifcInstance) ifcInstance->iA4APrivateEnumSetCallback(Callback, pCbData);
}

A4AError a4aInterface::GetDeviceProperty(A4ADeviceProperty DeviceProperty, long DeviceIndex, void* PropertyData)
{
	if (ifcInstance == NULL) return A4A_PRIVATE_ERR_NO_SUCH_OBJECT;
	return ifcInstance->iA4APrivateGetDeviceProperty(DeviceProperty, DeviceIndex, PropertyData, sizeof(void*));
}

A4AError a4aInterface::SetDeviceProperty(A4ADeviceProperty DeviceProperty, long DeviceIndex, void* PropertyData)
{
	if (ifcInstance == NULL) return A4A_PRIVATE_ERR_NO_SUCH_OBJECT;
	return ifcInstance->iA4APrivateSetDeviceProperty(DeviceProperty, DeviceIndex, PropertyData, sizeof(void*));
}
  
A4AError a4aInterface::GetInterfaceProperty(A4AInterfaceProperty InterfaceProperty, long DeviceIndex, long InterfaceIndex, void* PropertyData)
{
	if (ifcInstance == NULL) return A4A_PRIVATE_ERR_NO_SUCH_OBJECT;
	return ifcInstance->iA4APrivateGetInterfaceProperty(InterfaceProperty, DeviceIndex, InterfaceIndex, PropertyData, sizeof(void*));
}

A4AError a4aInterface::SetInterfaceProperty(A4AInterfaceProperty InterfaceProperty, long DeviceIndex, long InterfaceIndex, void* PropertyData)
{
	if (ifcInstance == NULL) return A4A_PRIVATE_ERR_NO_SUCH_OBJECT;
	return ifcInstance->iA4APrivateSetInterfaceProperty(InterfaceProperty, DeviceIndex, InterfaceIndex, PropertyData, sizeof(void*));
}

A4AError a4aInterface::GetPinProperty(A4APinProperty PinProperty, long DeviceIndex, long InterfaceIndex, long PinIndex, void* PropertyData)
{
	if (ifcInstance == NULL) return A4A_PRIVATE_ERR_NO_SUCH_OBJECT;
	return ifcInstance->iA4APrivateGetPinProperty(PinProperty, DeviceIndex, InterfaceIndex, PinIndex, PropertyData, sizeof(void*));
}

A4AError a4aInterface::SetPinProperty(A4APinProperty PinProperty, long DeviceIndex, long InterfaceIndex, long PinIndex, void* PropertyData)
{
	if (ifcInstance == NULL) return A4A_PRIVATE_ERR_NO_SUCH_OBJECT;
	return ifcInstance->iA4APrivateSetPinProperty(PinProperty, DeviceIndex, InterfaceIndex, PinIndex, PropertyData, sizeof(void*));
}