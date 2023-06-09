
#ifndef UWB_PEER_HXX
#define UWB_PEER_HXX

#include <mutex>
#include <optional>

#include <uwb/UwbMacAddress.hxx>
#include <uwb/protocols/fira/FiraDevice.hxx>

namespace uwb
{
/**
 * @brief Spatial properties of a UWB peer.
 */
struct UwbPeerSpatialProperties
{
    std::optional<double> Distance;
    std::optional<double> AngleAzimuth;
    std::optional<double> AngleElevation;
    std::optional<double> Elevation;

    std::optional<uint8_t> AngleAzimuthFom;
    std::optional<uint8_t> AngleElevationFom;
    std::optional<uint8_t> ElevationFom;

    std::string
    ToString() const;

    auto
    operator<=>(const UwbPeerSpatialProperties& other) const = default;
};

/**
 * @brief Represents a UWB peer device.
 */
class UwbPeer
{
public:
    /**
     * @brief Construct a new UwbPeer object.
     */
    UwbPeer() = default;

    /**
     * @brief Construct a new UwbPeer object.
     *
     * @param address
     */
    explicit UwbPeer(UwbMacAddress address);

    /**
     * @brief Construct a new UwbPeer object.
     * 
     * @param address 
     * @param spatialProperties 
     */
    explicit UwbPeer(UwbMacAddress address, UwbPeerSpatialProperties spatialProperties);

    /**
     * @brief Construct a new Uwb Peer object from UwbRangingMeasurement data
     * 
     * @param data 
     */
    explicit UwbPeer(const uwb::protocol::fira::UwbRangingMeasurement& data);

    /**
     * @brief Construct a new UwbPeer object from another.
     *
     * @param other
     */
    UwbPeer(const UwbPeer& other);

    /**
     * @brief Construct a new UwbPeer object.
     *
     * @param other
     */
    UwbPeer(UwbPeer&& other) noexcept;

    /**
     * @brief Copy-assignment operator.
     *
     * @param other
     * @return UwbPeer
     */
    UwbPeer&
    operator=(const UwbPeer& other);

    /**
     * @brief Get the peer's mac address.
     *
     * @return UwbMacAddress
     */
    UwbMacAddress
    GetAddress() const noexcept;

    /**
     * @brief Retrieves the latest spatial properties for this peer.
     *
     * @return UwbPeerSpatialProperties
     */
    UwbPeerSpatialProperties
    GetSpatialProperties() const noexcept;

    std::string
    ToString() const;

private:
    UwbMacAddress m_address;
    UwbPeerSpatialProperties m_spatialProperties{};
    mutable std::mutex m_spatialPropertiesGate;
};

bool
operator==(const UwbPeer&, const UwbPeer&) noexcept;

bool
operator!=(const UwbPeer&, const UwbPeer&) noexcept;

} // namespace uwb

#endif // UWB_PEER_HXX
