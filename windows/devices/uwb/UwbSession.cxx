
#include <UwbCxLrpDeviceGlue.h>
#include <wil/result.h>

#include <algorithm>
#include <magic_enum.hpp>
#include <memory>
#include <numeric>

#include <windows/devices/uwb/UwbSession.hxx>

using namespace windows::devices::uwb;

UwbSession::UwbSession(std::weak_ptr<::uwb::UwbSessionEventCallbacks> callbacks, wil::unique_hfile handleDriver) :
    ::uwb::UwbSession(std::move(callbacks)),
    m_handleDriver(std::move(handleDriver))
{}

template <class T>
std::unique_ptr<UWB_APP_CONFIG_PARAM>
GetUWB_APP_CONFIG_PARAM(const T &value)
{
    auto pdata = std::make_unique<uint8_t>(FIELD_OFFSET(UWB_APP_CONFIG_PARAM, paramValue[sizeof T]));
}

template <class T>
UWB_SET_APP_CONFIG_PARAMS
GetUWB_SET_APP_CONFIG_PARAMS()
{
}

/**
 * @brief This function only works for integer types, aka uint8_t, uint16_t, uint32_t, uint64_t
 *
 * @param value
 * @param size
 * @param dst
 * @return uint8_t*
 */
uint8_t *
WriteBytes(uint64_t value, std::size_t size, uint8_t *dst)
{
    switch (size) {
    case 1: {
        dst[0] = (uint8_t)value;
        return &dst[1];
    }
    case 2: {
        auto asdf = (uint16_t *)dst;
        asdf[0] = (uint16_t)value;
        return (uint8_t *)(&asdf[1]);
    }
    case 4: {
        auto asdf = (uint32_t *)dst;
        asdf[0] = (uint32_t)value;
        return (uint8_t *)(&asdf[1]);
    }
    case 8: {
        auto asdf = (uint64_t *)dst;
        asdf[0] = (uint64_t)value;
        return (uint8_t *)(&asdf[1]);
    }
    default:
        throw std::runtime_error("wrong number of bytes requested");
    };
}

template <int N>
uint8_t *
WriteBytes(std::array<uint8_t, N> value, unsigned int size, uint8_t *dst)
{
    memcpy(dst, value.data(), size);
    return dst + size;
}

/**
 * @brief Maps the param to the length of the value. If the value is 0, means a variable length and special code to handle.
 * Taken from the FiRa UCI spec Table 29:APP Configuration IDs.
 * TODO put this somewhere more appropriate.
 */
std::unordered_map<UWB_APP_CONFIG_PARAM_TYPE, unsigned int> appConfigParamLengths{
    { UWB_APP_CONFIG_PARAM_TYPE_DEVICE_TYPE, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_USAGE, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_STS_CONFIG, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_MULTI_NODE_MODE, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_CHANNEL_NUMBER, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_CONTROLEES, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_DEVICE_MAC_ADDRESS, 0 },
    { UWB_APP_CONFIG_PARAM_TYPE_DST_MAC_ADDRESS, 0 },
    { UWB_APP_CONFIG_PARAM_TYPE_SLOT_DURATION, 2 },
    { UWB_APP_CONFIG_PARAM_TYPE_RANGING_INTERVAL, 4 },
    { UWB_APP_CONFIG_PARAM_TYPE_STS_INDEX, 4 },
    { UWB_APP_CONFIG_PARAM_TYPE_MAC_FCS_TYPE, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_RANGING_ROUND_CONTROL, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_AOA_RESULT_REQ, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_CONFIG, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_PROXIMITY_NEAR, 2 },
    { UWB_APP_CONFIG_PARAM_TYPE_RANGE_DATA_NTF_PROXIMITY_FAR, 2 },
    { UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_RFRAME_CONFIG, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_CODE_INDEX, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_SFD_ID, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_PSDU_DATA_RATE, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_PREAMBLE_DURATION, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_RANGING_TIME_STRUCT, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_SLOTS_PER_RR, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_TX_ADAPTIVE_PAYLOAD_POWER, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_RESPONDER_SLOT_INDEX, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_PRF_MODE, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_SCHEDULED_MODE, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_KEY_ROTATION_RATE, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_SESSION_PRIORITY, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_MAC_ADDRESS_MODE, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_VENDOR_ID, 2 },
    { UWB_APP_CONFIG_PARAM_TYPE_STATIC_STS_IV, 6 },
    { UWB_APP_CONFIG_PARAM_TYPE_NUMBER_OF_STS_SEGMENTS, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_MAX_RR_RETRY, 2 },
    { UWB_APP_CONFIG_PARAM_TYPE_UWB_INITIATION_TIME, 4 },
    { UWB_APP_CONFIG_PARAM_TYPE_HOPPING_MODE, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_BLOCK_STRIDE_LENGTH, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_RESULT_REPORT_CONFIG, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_IN_BAND_TERMINATION_ATTEMPT_COUNT, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_SUB_SESSION_ID, 4 },
    { UWB_APP_CONFIG_PARAM_TYPE_BPRF_PHR_DATA_RATE, 1 },
    { UWB_APP_CONFIG_PARAM_TYPE_MAX_NUMBER_OF_MEASUREMENTS, 2 },
    { UWB_APP_CONFIG_PARAM_TYPE_STS_LENGTH, 1 }
};

std::unordered_map<
    UWB_APP_CONFIG_PARAM_TYPE,
    std::tuple<uwb::protocol::fira::UwbConfiguration::ParameterTag,
        std::function<uint8_t *(std::any, std::size_t, uint8_t *)>>>
    ddi_service_map{
        { UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE,
            { uwb::protocol::fira::UwbConfiguration::ParameterTag::DeviceRole,
                [](std::any arg, std::size_t size, uint8_t *dst) {
                    auto role = std::any_cast<uwb::protocol::fira::DeviceRole>(arg);
                    auto index = magic_enum::enum_index<uwb::protocol::fira::DeviceRole>(role).value();
                    return WriteBytes(index, appConfigParamLengths[UWB_APP_CONFIG_PARAM_TYPE_DEVICE_ROLE], dst);
                } } }
    };

std::shared_ptr<uint8_t>
WriteUWB_APP_CONFIG_PARAM(UWB_APP_CONFIG_PARAM_TYPE dditype, std::any arg)
{
    auto paramLength = appConfigParamLengths[dditype];
    auto size = FIELD_OFFSET(UWB_APP_CONFIG_PARAM, paramValue[paramLength]);
    auto result = std::make_shared<uint8_t>(size);
    auto param = std::reinterpret_pointer_cast<UWB_APP_CONFIG_PARAM, uint8_t>(result);
    param->size = size;
    param->paramType = dditype;
    param->paramLength = paramLength;
    auto [_, fn] = ddi_service_map[dditype];
    fn(arg, paramLength, param->paramValue);
    return result;
}

void
UwbSession::ConfigureImpl(const ::uwb::protocol::fira::UwbSessionData &uwbSessionData)
{
    // TODO: collect options set from uwbConfiguration, translate them to
    // UWB_APP_CONFIG_PARAM, and send them to the driver.

    // Populate the session initialization command argument.
    UWB_SESSION_INIT sessionInit;
    sessionInit.sessionId = uwbSessionData.sessionId;
    sessionInit.sessionType = UWB_SESSION_TYPE_RANGING_SESSION;

    // Request a new session from the driver.
    HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_SESSION_INIT, &sessionInit, sizeof sessionInit, nullptr, 0, nullptr, nullptr);
    if (FAILED(hr)) {
        // TODO: handle this
    }

    m_sessionId = uwbSessionData.sessionId;

    // Populate the PUWB_SET_APP_CONFIG_PARAMS
    auto sessionUwbMap = uwbSessionData.uwbConfiguration.GetValueMap();

    std::vector<std::shared_ptr<uint8_t>> datavector(ddi_service_map.size());

    std::transform(std::cbegin(ddi_service_map),
        std::cend(ddi_service_map),
        std::cbegin(datavector),
        [&sessionUwbMap](const auto &it) {
            const auto &[dditype, t] = it;
            const auto &[ptag, fn] = t;
            auto data = WriteUWB_APP_CONFIG_PARAM(dditype, sessionUwbMap[ptag]);
            return data;
        });

    auto overallSize = std::transform_reduce(
        std::cbegin(datavector),
        std::cend(datavector),
        FIELD_OFFSET(UWB_SET_APP_CONFIG_PARAMS, appConfigParams),
        [](const int a, const int b) {
            return a + b;
        },
        [](std::shared_ptr<uint8_t> data) {
            auto paramStruct = std::reinterpret_pointer_cast<UWB_APP_CONFIG_PARAM, uint8_t>(data);
            return paramStruct->size;
        });

    auto asdf = std::make_shared<uint8_t>(overallSize);
    auto paramHolder = std::reinterpret_pointer_cast<UWB_SET_APP_CONFIG_PARAMS, uint8_t>(asdf);
    paramHolder->size = overallSize;
    paramHolder->sessionId = uwbSessionData.sessionId;
    paramHolder->appConfigParamsCount = datavector.size();
    uint8_t *dst = (uint8_t *)(paramHolder->appConfigParams);
    for (std::shared_ptr<uint8_t> p : datavector) {
        auto data = std::reinterpret_pointer_cast<UWB_APP_CONFIG_PARAM, uint8_t>(p);
        memcpy(dst, p.get(), data->size);
        dst += data->size;
    }

    // Allocate memory for the PUWB_SET_APP_CONFIG_PARAMS_STATUS
    auto statusSize = FIELD_OFFSET(UWB_SET_APP_CONFIG_PARAMS_STATUS, appConfigParamsStatus[datavector.size()]);
    auto tmpHolder = std::make_shared<uint8_t>(statusSize);
    auto statusHolder = std::reinterpret_pointer_cast<UWB_SET_APP_CONFIG_PARAMS_STATUS, uint8_t>(tmpHolder);
    statusHolder->size = statusSize;
    statusHolder->appConfigParamsCount = datavector.size();

    hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_SET_APP_CONFIG_PARAMS, paramHolder.get(), overallSize, statusHolder.get(), statusSize, nullptr, nullptr);
    if (FAILED(hr)) {
        // TODO: handle this
    }
}

void
UwbSession::StartRangingImpl()
{
    UWB_START_RANGING_SESSION startRangingSession;
    startRangingSession.sessionId = GetId();
    HRESULT hr = DeviceIoControl(m_handleDriver.get(), IOCTL_UWB_STOP_RANGING_SESSION, &startRangingSession, sizeof startRangingSession, nullptr, 0, nullptr, nullptr);
    if (FAILED(hr)) {
        // TODO
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
    }
}

void
    UwbSession::AddPeerImpl(::uwb::UwbMacAddress /* peerMacAddress */)
{
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
    }
}
