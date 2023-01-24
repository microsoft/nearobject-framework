
#ifndef UWB_SIMULATOR_DEVICE_FILE_OBJECT
#define UWB_SIMULATOR_DEVICE_FILE_OBJECT

#include <windows.h>

#include <wdf.h>

class UwbSimulatorDeviceFile
{
public:
    explicit UwbSimulatorDeviceFile(WDFFILEOBJECT wdfFile);

public:
    static EVT_WDF_OBJECT_CONTEXT_DESTROY OnWdfDestroy;
    static EVT_WDF_REQUEST_CANCEL OnWdfRequestCancel;

private:
    /**
     * @brief
     */
    void
    OnDestroy();

    /**
     * @brief
     *
     * @param request
     */
    void
    OnRequestCancel(WDFREQUEST request);

private:
    WDFFILEOBJECT m_wdfFile;
};

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(UwbSimulatorDeviceFile, GetUwbSimulatorFile);

#endif // UWB_SIMULATOR_DEVICE_FILE_OBJECT
