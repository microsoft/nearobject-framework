
#ifndef WINDOWS_DEVICE_RESOURCE_H
#define WINDOWS_DEVICE_RESOURCE_H

#include <wil/resource.h>

#ifdef _CFGMGR32_H_
using unique_hcmnotification = wil::unique_any<HCMNOTIFICATION, decltype(&::CM_Unregister_Notification), ::CM_Unregister_Notification>;
#endif // _CFGMGR32_H_

#endif // WINDOWS_DEVICE_RESOURCE_H