
#ifndef UWB_SIMULATOR_DISPATCH_ENTRY_HXX
#define UWB_SIMULATOR_DISPATCH_ENTRY_HXX

#include <concepts>
#include <cstddef>
#include <functional>
#include <limits>
#include <span>
#include <type_traits>

#include <windows.h>

#include <wdf.h>

namespace windows::devices::uwb::simulator
{
/**
 * @brief Represents an entry used to dispatch WDF requests to a handler
 * implementation.
 *
 * @tparam ClassT The type of the containing class which defines the
 * implementation handler functions. This limits the member functions which can
 * be used to dispatch execution.
 */
template <typename ClassT>
struct UwbSimulatorDispatchEntry
{
    using HandlerFuncT = NTSTATUS (ClassT::*)(WDFREQUEST, std::span<uint8_t>, std::span<uint8_t>);

    HandlerFuncT Handler;
    ULONG IoControlCode;
    std::size_t InputSizeMinimum{ 0 };
    std::size_t OutputSizeMinimum{ 0 };

    /**
     * @brief Determines whether the specified request parameters are valid for
     * this dispatch entry.
     *
     * The last two parameters, [input|output]BufferVariableSize are intended to
     * be used for IOCTLs which expect types that vary in size. In particular,
     * this includes types that have a flex-array member (member[1]) at the end
     * of the structure that references trailing data beyond its compile-time
     * size. The value of these arguments will be added to the entry's minimum
     * size to account for the variable portion.
     *
     * Eg.
     * <in app>
     * std::size_t paramLength = 20;
     * std::size_t paramLengthTotal = sizeof (UWB_CAPABILITY_PARAM) + paramLength - 1;
     * auto paramsBuffer = std::make_unique<uint8_t[]>(paramLenghtTotal);
     * UWB_CAPABILITY_PARAM *params = reinterpret_cast<UWB_CAPABILITY_PARAM *>(paramsBuffer.get());
     * auto hr = DeviceIoControl(handle, ..., paramsBuffer, paramLengthTotal, ...);
     *
     * <in driver>
     * std::size_t paramLengthExpected = 10;
     * auto status = <dispatchEntry>.GetRequestValidityStatus(inputBufferSize, 0, paramLengthExpected, 0);
     *
     * @param inputBufferSize The size of the request input buffer, in bytes.
     * @param outputBufferSize The size of the request output buffer, in bytes.
     * @param inputBufferVariableSize The variable portion of the input buffer.
     * @param outputBufferVariableSize The variable portion of the output buffer.
     * @return NTSTATUS
     */
    NTSTATUS
    GetRequestValidityStatus(std::size_t inputBufferSize, std::size_t outputBufferSize, std::size_t inputBufferVariableSize = 0, std::size_t outputBufferVariableSize = 0) const noexcept
    {
        return (inputBufferSize >= (InputSizeMinimum + inputBufferVariableSize)) &&
            (outputBufferSize >= (OutputSizeMinimum + outputBufferVariableSize));
    }
};

/**
 * @brief Helper function to create a UwbSimulatorDispatchEntry.
 *
 * This isn't strictly needed since the same effect can be achieved by
 * performing aggregrate-initialization directly. However, this exists to enable
 * consistency in the UwbSimulatorDispatchEntry creational pattern.
 *
 * @tparam ClassT The type of class that defines the handler function.
 * @param ioControlCode The control code for the entry.
 * @param handler The handler for the IOCTL.
 * @param inputSizeMinimum The minimum input buffer size.
 * @param outputSizeMinimum The minimum output buffer size.
 * @return UwbSimulatorDispatchEntry<ClassT>
 */
template <typename ClassT>
UwbSimulatorDispatchEntry<ClassT>
MakeDispatchEntry(ULONG ioControlCode, typename UwbSimulatorDispatchEntry<ClassT>::HandlerFuncT handler, std::size_t inputSizeMinimum, std::size_t outputSizeMinimum)
{
    return UwbSimulatorDispatchEntry<ClassT>{
        .Handler = handler,
        .IoControlCode = ioControlCode,
        .InputSizeMinimum = inputSizeMinimum,
        .OutputSizeMinimum = outputSizeMinimum,
    };
}

namespace detail
{
/**
 * @brief Type trait to define the minimum size for a dispatch entry.
 *
 * By default, this returns the size of the type specified.
 *
 * @tparam T The expected type contained in the buffer.
 */
template <typename BufferT>
struct DispatchEntryMinimumSizeImpl
{
    static constexpr std::size_t value = sizeof(BufferT);
};

/**
 * @brief Sentinel type to denote that the buffer size should be unrestricted.
 */
struct Unrestricted
{};

/**
 * @brief Specialization to enforce no minimum buffer size (0).
 *
 * @tparam Sentinel Unrestricted type.
 */
template <>
struct DispatchEntryMinimumSizeImpl<Unrestricted>
{
    static constexpr std::size_t value = 0;
};

/**
 * @brief Helper alias to resolve the type value of the
 * DispatchEntryMinimumSizeImpl trait, which denotes the size of the buffer.
 *
 * @tparam T The expected type contained in the buffer.
 */
template <typename BufferT>
inline constexpr std::size_t DispatchEntryMinimumSizeImpl_v = DispatchEntryMinimumSizeImpl<BufferT>::value;

} // namespace detail

/**
 * @brief Helper type to denote that the buffer size should not be restricted.
 */
using Unrestricted = detail::DispatchEntryMinimumSizeImpl<detail::Unrestricted>;

/**
 * @brief Helper function to create a UwbSimulatorDispatchEntry that uses the
 * size of a structure to determine the minimum size of the expected input
 * and/or output buffers for an IOCTL.
 *
 * When either of the InputT or OutputT types are not directly specified, the
 * associated buffer size shall be unrestricted (no minimum required).
 *
 * @tparam ClassT The type of class that defines the handler function.
 * @tparam InputT The expected type that is contained in the input buffer.
 * @tparam OutputT The expected type that is contained in the output buffer.
 * @param ioControlCode The control code for the entry.
 * @param handler The handler for the IOCTL.
 * @return UwbSimulatorDispatchEntry<ClassT>
 */
template <
    typename ClassT,
    typename InputT = Unrestricted,
    typename OutputT = Unrestricted>
UwbSimulatorDispatchEntry<ClassT>
MakeDispatchEntry(ULONG ioControlCode, typename UwbSimulatorDispatchEntry<ClassT>::HandlerFuncT handler)
{
    return MakeDispatchEntry<ClassT>(
        ioControlCode,
        handler,
        detail::DispatchEntryMinimumSizeImpl_v<InputT>,
        detail::DispatchEntryMinimumSizeImpl_v<OutputT>);
}
} // namespace windows::devices::uwb::simulator

#endif // UWB_SIMULATOR_DISPATCH_ENTRY_HXX
