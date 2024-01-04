//////////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2003-2021, Michael Tippach
//
// Use of this work is subject to the ASIO4ALL Private API SDK license
// agreement.
//
//                            !!! PRELIMINARY !!!
//
//////////////////////////////////////////////////////////////////////////////

#ifndef IAA4APRIV_INCLUDED
#define IAA4APRIV_INCLUDED

//
// Error codes. So far only a few codes are used.
//

typedef enum A4AError {
	A4A_PRIVATE_NOERROR = 0x00000000,
	A4A_PRIVATE_ERR_ENUM_REENTRY = 0xA4AE0001,
	A4A_PRIVATE_ERR_DRIVER_NOT_IDLE = 0xA4AE0002,
	A4A_PRIVATE_ERR_NO_SUCH_OBJECT = 0xA4AE1001,
	A4A_PRIVATE_ERR_ILLEGAL_ARGUMENT = 0xA4AE2001
};

// A4A_PRIVATE_NOERROR
// _________________________________
//
// The operation completed successfully.
//
//
// A4A_PRIVATE_ERR_ENUM_REENTRY
// _________________________________
//
// An attempt was made to re-enter device enumeration while device enumeration
// is already in progress. The most likely reason is a callback installed using
// iA4APrivateEnumSetCallback() attempting to invoke iA4APrivateEnumerate()
//
// iA4APrivateEnumerate() must never be used inside the callback!
//
//
// A4A_PRIVATE_ERR_DRIVER_NOT_IDLE
// _________________________________
//
// An attempt was made to access enumeration data while the ASIO driver is not
// in the idle state.
//
// Stop all audio processing and only then call iA4APrivateEnumerate()!
//
//
// A4A_PRIVATE_ERR_NO_SUCH_OBJECT
// _________________________________
//
// The object (device, interface, pin) with the index provided does not exist.
// You may legally encounter this error when enumerating objects. If this code
// is returned inside an enumeration loop, this would be an indicator that the
// end of the device/interface/pin list has been reached.
//
//
// A4A_PRIVATE_ERR_ILLEGAL_ARGUMENT
// _________________________________
//
// An argument has been passed to the interface outside of what it can handle.
// Possible causes include out of range property identifiers, NULL pointer to
// property data or a data size argument too small for the property requested.
//

//
// Bit definitions for the
//  kA4A_Device_dwFlags,
//  kA4A_Interface_dwFlags,
//  kA4A_Pin_dwFlags
// properties
//
#define A4A_FLAG_STATEMASK			0x00000060	// GUI housekeeping, DO NOT TOUCH!
#define A4A_FLAG_WATCHDOG_ENABLE	0x00000001	// Application level setting, DO NOT TOUCH!
#define A4A_FLAG_SAFEMODE_ENABLE	0x00000002	// Application level setting, DO NOT TOUCH!
#define A4A_FLAG_NOINPUT			0x00002000	// Used in OEM builds, reserved otherwise, DO NOT TOUCH!
#define A4A_FLAG_NOSHUTDOWN			0x00004000	// Application level status, DO NOT TOUCH!
#define A4A_FLAG_ADVANCED			0x00008000	// GUI housekeeping, DO NOT TOUCH!
#define A4A_FLAG_HWBUFFER			0x00010000	// h/w buffer enable.
#define A4A_FLAG_FORCESRC			0x00020000	// Force 44.1KHz resampling. Applies to Device.
#define A4A_FLAG_FORCE16			0x00040000	// Force WDM driver to 16Bit. Applies to Device.
#define A4A_FLAG_RTAUDIO			0x00080000	// WaveRT interface indicator, DO NOT TOUCH!
#define A4A_FLAG_MEMBARRIER			0x00100000	// WaveRT interface indicator, DO NOT TOUCH!
#define A4A_FLAG_PULLMODE			0x00200000	// Allow WaveRT "Pull" mode. Applies to Device
#define A4A_FLAG_OFFSET_VALID		0x01000000	// Internal housekeeping, DO NOT TOUCH!
#define A4A_FLAG_RELAX_NUMPLAYING	0x02000000	// Internal housekeeping, DO NOT TOUCH!
#define A4A_FLAG_RUNNING			0x10000000	// Indicates object is in the "running" state.
#define A4A_FLAG_ERROR				0x20000000	// Unspecified error indicator.
#define A4A_FLAG_AVAILABLE			0x40000000	// Possible instance available Applies mainly to Pin.
#define A4A_FLAG_ENABLED			0x80000000	// Object enable/disable switch.

//
// Bit field definitions for kA4A_Interface_dwProperties property
//
#define A4A_IFPROPERTY_ADC			0x00000001	// Analog Input
#define A4A_IFPROPERTY_DAC			0x00000002	// Analog Output
#define A4A_IFPROPERTY_SPDIF		0x00000004	// S/PDIF I/O

//
// Device properties
//
typedef enum A4ADeviceProperty {
	kA4A_Device_dwFlags = 0,		// [DataSize=4] See bit definitions above!
	kA4A_Device_lpszName,			// [DataSize=4] Device name in registry
	kA4A_Device_lpszVendorName,		// [DataSize=4] Vendor name in registry
	kA4A_Device_lpszServiceName,	// [DataSize=4] Service name in registry
	kA4A_Device_lpszLocationInfo,	// [DataSize=4] Location information in registry
	kA4A_Device_lpszDeviceID,		// [DataSize=4] PnP ID in registry
	kA4A_Device_dwBusNumber,		// [DataSize=4] Bus number. Actually a 3 char string with trailing 0
	kA4A_Device_dwKsBuffers,		// [DataSize=4] Number of Kernel buffers to use
	kA4A_Device_dwIoDelay,			// [DataSize=4] Delay offset in h/w buffer (ms)
	kA4A_Device_dwInputComp,		// [DataSize=4] Input latency compensation (samples)
	kA4A_Device_dwOutputComp,		// [DataSize=4] Output latency compensation (samples)
	kA4A_Device_dwBufferSize,		// [DataSize=4] ASIO buffer size for this device
	kA4A_Device_dwInstanceHash		// [DataSize=4] Hash code as used in registry. Useful for what?
};

//
// Interface properties
//
typedef enum A4AInterfaceProperty {
	kA4A_Interface_dwFlags = 0,		// [DataSize=4] See bit definitions above!
	kA4A_Interface_lpszName,		// [DataSize=4] Interface name in registry
	kA4A_Interface_pSPDIDD,			// [DataSize=4] PSP_DEVICE_INTERFACE_DETAIL_DATA, see Windows DDK!
	kA4A_Interface_dwProperties		// [DataSize=4] See bit definitions above!
};

//
// Pin properties
//
typedef enum A4APinProperty {
	kA4A_Pin_dwFlags = 0,			// [DataSize=4] See bit definitions above!
	kA4A_Pin_dwDataFlow,			// [DataSize=4] Either of KSPIN_DATAFLOW_IN of KSPIN_DATAFLOW_OUT, see Windows DDK!
	kA4A_Pin_dwMaxChannels,			// [DataSize=4] Maximum PCM channels
	kA4A_Pin_dwMaxBits,				// [DataSize=4] Maximum audio bit depth
	kA4A_Pin_dwMinSR,				// [DataSize=4] Minimum sample frequency
	kA4A_Pin_dwMaxSR				// [DataSize=4] Maximum sample frequency
};

//
// COM interface definition.
//
interface IA4APRIVATE : public IUnknown
{
	virtual	DWORD  iA4APrivateGetVersion() = 0;
	virtual	void iA4APrivateEnumerate() = 0;
	virtual	void iA4APrivateEnumSetCallback(BOOL(void*), void* pCbData) = 0;
	//----------------------------------------------------------------------------
	// The following can only be safely called from inside a callback installed
	// using method above!
	//
	virtual	A4AError iA4APrivateGetDeviceProperty(A4ADeviceProperty DeviceProperty, long DeviceIndex, void* PropertyData, long DataSize) = 0;
	virtual	A4AError iA4APrivateSetDeviceProperty(A4ADeviceProperty DeviceProperty, long DeviceIndex, void* PropertyData, long DataSize) = 0;
	virtual	A4AError iA4APrivateGetInterfaceProperty(A4AInterfaceProperty InterfaceProperty, long DeviceIndex, long InterfaceIndex, void* PropertyData, long DataSize) = 0;
	virtual	A4AError iA4APrivateSetInterfaceProperty(A4AInterfaceProperty InterfaceProperty, long DeviceIndex, long InterfaceIndex, void* PropertyData, long DataSize) = 0;
	virtual	A4AError iA4APrivateGetPinProperty(A4APinProperty PinProperty, long DeviceIndex, long InterfaceIndex, long PinIndex, void* PropertyData, long DataSize) = 0;
	virtual	A4AError iA4APrivateSetPinProperty(A4APinProperty PinProperty, long DeviceIndex, long InterfaceIndex, long PinIndex, void* PropertyData, long DataSize) = 0;
	//
	//----------------------------------------------------------------------------
	// Revision 1.0 interface ends here.
	//----------------------------------------------------------------------------
};

//
// The interface and class GUID. This one is dependent on you specific OEM version.
//
extern GUID IID_IA4APRIVATE;
extern GUID IID_IA4A;

#endif			// IAA4APRIV_INCLUDED
