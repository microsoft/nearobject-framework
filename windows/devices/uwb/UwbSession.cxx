
#include <UwbCxLrpDeviceGlue.h>
#include <wil/result.h>

#include <algorithm>
#include <memory>
#include <numeric>

#include <windows/devices/uwb/UwbCxAdapter.hxx>
#include <windows/devices/uwb/UwbSession.hxx>

#include <plog/Log.h>

using namespace windows::devices::uwb;

UwbSession::UwbSession(std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks, wil::unique_hfile handleDriver) :
    ::uwb::UwbSession(std::move(callbacks)),
    m_handleDriver(std::move(handleDriver))
{}

void
UwbSession::ConfigureImpl(const ::uwb::protocol::fira::UwbSessionData &uwbSessionData)
{
    PLOG_VERBOSE << "ConfigureImpl";

    // Populate the session initialization command argument.
    UWB_SESSION_INIT sessionInit;
    sessionInit.sessionId = uwbSessionData.sessionId;
    sessionInit.sessionType = UWB_SESSION_TYPE_RANGING_SESSION;

    // Request a new session from the driver.
    HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_SESSION_INIT, &sessionInit, sizeof sessionInit, nullptr, 0, nullptr, nullptr);
    if (FAILED(hr)) {
        // TODO: handle this
        PLOG_ERROR << "could not request a new session from the driver, hr=" << std::hex << hr;
    }

    m_sessionId = uwbSessionData.sessionId;

    // Populate the PUWB_SET_APP_CONFIG_PARAMS
    auto setParamsAdaptor = GenerateUwbSetAppConfigParameterDdi(uwbSessionData);
    auto &setParamsBuffer = setParamsAdaptor.DdiBuffer();
    auto &setParams = setParamsAdaptor.DdiParameters();

    // Allocate memory for the PUWB_SET_APP_CONFIG_PARAMS_STATUS
    auto statusSize = offsetof(UWB_SET_APP_CONFIG_PARAMS_STATUS, appConfigParamsStatus[setParams.appConfigParamsCount]);
    auto statusBuffer = std::make_unique<uint8_t[]>(statusSize);
    auto &statusHolder = *reinterpret_cast<UWB_SET_APP_CONFIG_PARAMS_STATUS *>(statusBuffer.get());
    statusHolder.size = statusSize;
    statusHolder.appConfigParamsCount = setParams.appConfigParamsCount;

    hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_SET_APP_CONFIG_PARAMS, std::data(setParamsBuffer), std::size(setParamsBuffer), statusBuffer.get(), statusSize, nullptr, nullptr);
    if (FAILED(hr)) {
        // TODO: handle this
        PLOG_ERROR << "could not send params to driver";
    }
}

void
UwbSession::StartRangingImpl()
{
    UWB_START_RANGING_SESSION startRangingSession;
    startRangingSession.sessionId = GetId();
    HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_START_RANGING_SESSION, &startRangingSession, sizeof startRangingSession, nullptr, 0, nullptr, nullptr);
    if (FAILED(hr)) {
        // TODO
        PLOG_ERROR << "could not send start ranging command to driver";
    }
}

void
UwbSession::StopRangingImpl()
{
    UWB_STOP_RANGING_SESSION stopRangingSession;
    stopRangingSession.sessionId = GetId();
    HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_STOP_RANGING_SESSION, &stopRangingSession, sizeof stopRangingSession, nullptr, 0, nullptr, nullptr);
    if (FAILED(hr)) {
        // TODO
        PLOG_ERROR << "could not send stop ranging command to driver";
    }
}

void
    UwbSession::AddPeerImpl(::uwb::UwbMacAddress /* peerMacAddress */)
{
    PLOG_VERBOSE << "AddPeerImpl";
    // TODO: two main options for updating the UWB-CLX peer list:
    //  1) Every time a peer is added (on-demand)
    //  2) Only when StartRanging() is called.
    // Below sample code exemplifies 1) for simplicity but this is not necessarily the way to go.
    //
    // TODO: request UWB-CLX to update controlee list per below pseudo-code,
    // which is *very* rough and some parts probably plain wrong:
    //
    const auto macAddressLength = m_uwbMacAddressSelf.GetLength();
    const auto macAddressessLength = macAddressLength * m_peers.size();

    std::size_t appConfigParamsSize = 0;
    appConfigParamsSize += macAddressessLength;
    // TODO: all other memory required for this structure must be accounted for, the above calculation was left incomplete.
    // Also, proper memory alignment of trailing structures in the allocated buffer has not been taken into account.
    auto appConfigParamsBuffer = std::make_unique<uint8_t[]>(appConfigParamsSize);
    auto *appConfigParams = reinterpret_cast<UWB_SET_APP_CONFIG_PARAMS *>(appConfigParamsBuffer.get());
    appConfigParams->sessionId = GetId();
    appConfigParams->appConfigParamsCount = 2;
    UWB_APP_CONFIG_PARAM *appConfigParamList = reinterpret_cast<UWB_APP_CONFIG_PARAM *>(appConfigParams + 1);
    UWB_APP_CONFIG_PARAM *appConfigParamNumberOfControlees = appConfigParamList + 0;
    UWB_APP_CONFIG_PARAM *appConfigParamDstMacAddress = appConfigParamList + 1;

    // Populate NUMBER_OF_CONTROLEES app configuration parameter.
    auto &numberOfControleesPayload = *reinterpret_cast<uint8_t *>(appConfigParamNumberOfControlees + 1);
    appConfigParamNumberOfControlees->paramType = UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_CONTROLEES;
    appConfigParamNumberOfControlees->paramLength = 1;
    numberOfControleesPayload = static_cast<uint8_t>(m_peers.size());

    // Populate DST_MAC_ADDRESS app configuration parameter.
    auto dstMacAddressPayload = reinterpret_cast<uint8_t *>(appConfigParamDstMacAddress + 1);
    appConfigParamDstMacAddress->paramType = UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS;
    appConfigParamDstMacAddress->paramLength = static_cast<uint32_t>(macAddressessLength);
    auto dstMacAddress = dstMacAddressPayload;
    for (const auto &peer : m_peers) {
        const auto value = peer.GetValue();
        std::copy(std::cbegin(value), std::cend(value), dstMacAddress);
        std::advance(dstMacAddress, std::size(value));
    }

    // Attempt to set all new parameters.
    DWORD bytesReturned = 0;
    UWB_SET_APP_CONFIG_PARAMS_STATUS appConfigParamsStatus; // TODO: this needs to be dynamically allocated to fit returned content
    HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_SET_APP_CONFIG_PARAMS, &appConfigParams, static_cast<DWORD>(appConfigParamsSize), &appConfigParamsStatus, sizeof appConfigParamsStatus, &bytesReturned, nullptr);
    if (FAILED(hr)) {
        // TODO
        PLOG_ERROR << "could not send params to driver";
    }
}
