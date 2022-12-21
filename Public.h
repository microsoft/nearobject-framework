/*++

Module Name:

    public.h

Abstract:

    This module contains the common declarations shared by driver
    and user applications.

Environment:

    driver and application

--*/

//
// Define an Interface Guid so that apps can find the device and talk to it.
//


DEFINE_GUID (GUID_DEVINTERFACE_UwbSimulator,
    0xe906bc6b,0x0d79,0x4282,0x91,0x65,0x1b,0xd6,0x06,0x4e,0x28,0x79);
// {e906bc6b-0d79-4282-9165-1bd6064e2879}

// {A7424370-45E5-49C3-AE35-42A3C41B34AD}
DEFINE_GUID(GUID_UWB_DEVICE_INTERFACE, 
    0xa7424370, 0x45e5, 0x49c3, 0xae, 0x35, 0x42, 0xa3, 0xc4, 0x1b, 0x34, 0xad);
