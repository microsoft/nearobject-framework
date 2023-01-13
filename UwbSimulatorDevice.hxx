
#ifndef UWB_SIMULATOR_DEVICE_HXX
#define UWB_SIMULATOR_DEVICE_HXX

#include <windows.h>

#include <wdf.h>
#include <wdfrequest.h>

#include "UwbSimulatorIoQueue.hxx"

/**
 * @brief
 */
class UwbSimulatorDevice
{
public:
    /**
     * @brief Construct a new UwbSimulatorDevice object.
     *
     * @param wdfDevice
     */
    explicit UwbSimulatorDevice(WDFDEVICE wdfDevice);

    /**
     * @brief
     *
     * @return NTSTATUS
     */
    NTSTATUS
    Initialize();

    /**
     * @brief
     *
     * @return NTSTATUS
     */
    NTSTATUS
    Uninitialize();

public:
    static EVT_WDF_DRIVER_DEVICE_ADD OnWdfDeviceAdd;
    static EVT_WDF_OBJECT_CONTEXT_CLEANUP OnWdfObjectContextCleanup;
    static EVT_WDF_DEVICE_D0_ENTRY OnWdfD0Entry;
    static EVT_WDF_DEVICE_D0_EXIT OnWdfD0Exit;
    static EVT_WDF_DEVICE_FILE_CREATE OnWdfFileCreate;
    static EVT_WDF_FILE_CLOSE OnWdfFileClose;

private:
    /**
     * @brief
     *
     * @param device
     * @param request
     * @param file
     */
    void
    OnFileCreate(WDFDEVICE device, WDFREQUEST request, WDFFILEOBJECT file);

    /**
     * @brief
     *
     * @param file
     */
    void
    OnFileClose(WDFFILEOBJECT file);

    /**
     * @brief
     *
     * @param previousState
     * @return NTSTATUS
     */
    NTSTATUS
    OnD0Entry(WDF_POWER_DEVICE_STATE previousState);

    /**
     * @brief
     *
     * @param targetState
     * @return NTSTATUS
     */
    NTSTATUS
    OnD0Exit(WDF_POWER_DEVICE_STATE targetState);

private:
    WDFDEVICE m_wdfDevice;
    UwbSimulatorIoQueue *m_ioQueue{ nullptr };
};

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(UwbSimulatorDevice, GetUwbSimulatorDevice)

#endif // UWB_SIMULATOR_DEVICE_HXX
