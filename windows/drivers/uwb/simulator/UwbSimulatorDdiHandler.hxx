
#ifndef UWB_SIMULATOR_DDI_HANDLER
#define UWB_SIMULATOR_DDI_HANDLER

#include <initializer_list>
#include <memory>
#include <optional>

#include <windows.h>

#include <wdf.h>

#include "IUwbSimulatorDdiHandler.hxx"
#include "UwbSimulatorDdiCallbacks.hxx"
#include "UwbSimulatorDevice.hxx"
#include "UwbSimulatorDeviceFile.hxx"
#include "UwbSimulatorDispatchEntry.hxx"

namespace windows::devices::uwb::simulator
{
class UwbSimulatorDdiHandler :
    public IUwbSimulatorDdiHandler
{
public:
    // TODO: docs
    explicit UwbSimulatorDdiHandler(UwbSimulatorDevice *device);

    /**
     * @brief Indicates whether the specified i/o control code is handled by
     * this handler.
     *
     * @param ioControlCode The i/o control code to check.
     * @return true
     * @return false
     */
    bool
    HandlesIoControlCode(ULONG ioControlCode) override;

    /**
     * @brief
     *
     * @param request
     * @param ioControlCode
     * @param inputBufferLength
     * @param outputBufferLength
     * @return NTSTATUS
     */
    NTSTATUS
    ValidateRequest(WDFREQUEST request, ULONG ioControlCode, std::size_t inputBufferLength, std::size_t outputBufferLength) override;

    /**
     * @brief
     *
     * @param request
     * @param ioControlCode
     * @param inputBuffer
     * @param outputBuffer
     * @return NTSTATUS
     */
    NTSTATUS
    HandleRequest(WDFREQUEST request, ULONG ioControlCode, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer) override;

private:
    // GUID_UWB_DEVICE_INTERFACE Handlers
    NTSTATUS
    OnUwbDeviceReset(WDFREQUEST, std::span<uint8_t>, std::span<uint8_t>);

    NTSTATUS
    OnUwbGetDeviceInformation(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbGetDeviceCapabilities(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbGetDeviceConfigurationParameters(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSetDeviceConfigurationParameters(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbGetApplicationConfigurationParameters(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSetApplicationConfigurationParameters(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbGetSessionCount(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSessionInitialize(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSessionDeinitialize(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbGetSessionState(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSessionUpdateControllerMulticastList(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSessionStartRanging(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSessionStopRanging(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSessionGetRangingCount(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbNotification(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    // GUID_DEVINTERFACE_UWB_SIMULATOR Handlers

    NTSTATUS
    OnUwbSimulatorCapabilities(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

    NTSTATUS
    OnUwbSimulatorTriggerSessionEvent(WDFREQUEST request, std::span<uint8_t> inputBuffer, std::span<uint8_t> outputBuffer);

private:
    std::optional<windows::devices::uwb::simulator::UwbSimulatorDispatchEntry<UwbSimulatorDdiHandler>>
    TryGetDispatchEntry(ULONG ioControlCode);

private:
    static const std::initializer_list<windows::devices::uwb::simulator::UwbSimulatorDispatchEntry<UwbSimulatorDdiHandler>> Dispatch;

private:
    UwbSimulatorDevice *m_device;
    std::unique_ptr<windows::devices::uwb::simulator::UwbSimulatorDdiCallbacks> m_callbacks;
};
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DDI_HANDLER
