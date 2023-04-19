
#ifndef DEVICE_HANDLE_HXX
#define DEVICE_HANDLE_HXX

#include <concepts>
#include <memory>
#include <string>

#include <windows.h>

#include <fileapi.h>
#include <wil/resource.h>
#include <wil/result.h>

namespace windows::devices
{
namespace detail
{
/**
 * @brief Helper concept to check whether a type has a reset() function for a
 * particular contained type.
 *
 * @tparam HandleSmartT The smart type managing a contained type.
 * @tparam ContainedT The contained type managed by the smart type.
 */
// clang-format off
template <typename HandleSmartT, typename ContainedT>
concept HasReset = requires (HandleSmartT handleSmart, ContainedT handle) {
    handleSmart.reset(handle);
};
// clang-format on
} // namespace detail

/**
 * @brief Open a HANDLE to a device interface path, using an RAII-wrapper.
 *
 * @tparam HandleSmartT The RAII-wrapper type to hold the HANDLE. This must
 * contain a reset() function which takes a HANDLE, such as the smart wil
 * RAII types like wil::unique_hfile and wil::shared_hfile.
 *
 * @param driverHandle The driver smart handle to update.
 * @param deviceName The device interface path to open.
 * @param isOverlapped Whether the handle should be opened in overlapped (async) mode or not.
 */
// clang-format off
template <typename HandleSmartT>
requires detail::HasReset<HandleSmartT, HANDLE>
// clang-format on
HRESULT
OpenDriverHandle(HandleSmartT &driverHandle, const char *deviceName, bool isOverlapped = false)
{
    driverHandle.reset(CreateFileA(
        deviceName,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        nullptr,
        OPEN_EXISTING,
        isOverlapped ? FILE_FLAG_OVERLAPPED : FILE_ATTRIBUTE_NORMAL,
        nullptr));
    RETURN_LAST_ERROR_IF(driverHandle.get() == INVALID_HANDLE_VALUE);
    return S_OK;
}

/**
 * @brief Open a shared HANDLE to a device interface path, using an RAII-wrapper.
 *
 * @param driverHandle The RAII-wrapper instance to store the HANDLE.
 * @param deviceName The device interface path to open.
 * @param isOverlapped Whether the HANDLE should be opened in overlapped (async) mode.
 * @return HRESULT S_OK if successful.
 */
HRESULT
OpenDriverHandleShared(wil::shared_hfile &driverHandle, const char *deviceName, bool isOverlapped = false);

/**
 * @brief Open a unique HANDLE to a device interface path, using an RAII-wrapper.
 *
 * @param driverHandle The RAII-wrapper instance to store the HANDLE.
 * @param deviceName The device interface path to open.
 * @param isOverlapped Whether the HANDLE should be opened in overlapped (async) mode.
 * @return HRESULT S_OK if successful.
 */
HRESULT
OpenDriverHandleUnique(wil::unique_hfile &driverHandle, const char *deviceName, bool isOverlapped = false);

} // namespace windows::devices

#endif // DEVICE_HANDLE_HXX
