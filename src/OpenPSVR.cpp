#include "OpenPSVR.h"

#include <openvr_driver.h>
#include "driverlog.h"

#include "WatchdogDriver_OpenPSVR.h"
#include "OpenPSVRDeviceDriver.h"
#include "ServerDriver_OpenPSVR.h"

using namespace vr;

//monitor info
/*
DISPLAY\SNYB403\5&14CF1EEA&1&UID4353
HardwareID: MONITOR\SNYB403
*/

//----------------------------------------------------------------
//Watchdog Driver
//----------------------------------------------------------------
CWatchdogDriver_OpenPSVR g_watchdogDriverPSVR;
//----------------------------------------------------------------

//----------------------------------------------------------------
//Server Driver
//----------------------------------------------------------------
CServerDriver_OpenPSVR g_serverDriverPSVR;
//----------------------------------------------------------------

HMD_DLL_EXPORT void *HmdDriverFactory(const char *pInterfaceName, int *pReturnCode) {
	if (pReturnCode)
		*pReturnCode = VRInitError_None;

	if (0 == strcmp(IServerTrackedDeviceProvider_Version, pInterfaceName)) {
		return &g_serverDriverPSVR;
	}
	if (0 == strcmp(IVRWatchdogProvider_Version, pInterfaceName)) {
		return &g_watchdogDriverPSVR;
	}

	if (pReturnCode)
		*pReturnCode = VRInitError_Init_InterfaceNotFound;

	return NULL;
}

/*
Events:
-psvr processor starts
-watchdog notices usb attachment
--WatchdogWakeUp
-vr server starts up
-server driver connects to psvr
--psvr_open, get device id
-server driver creates device driver for hmd
--set vr mode
--set settings
--psvr_sensor_frame
--psvr_control_frame

-psvr processor shutdown
-usb disconnects
--psvr_close / invalidate context
*/