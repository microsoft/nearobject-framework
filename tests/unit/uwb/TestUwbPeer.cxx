
#include <array>
#include <iterator>
#include <memory>
#include <type_traits>

#include <uwb/UwbPeer.hxx>
#include <uwb/UwbPeerJsonSerializer.hxx>

#include <catch2/catch_test_macros.hpp>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

namespace uwb::test
{
constexpr UwbPeerSpatialProperties UwbPeerSpatialPropertiesAllEmpty{
    .Distance = std::nullopt,
    .AngleAzimuth = std::nullopt,
    .AngleElevation = std::nullopt,
    .Elevation = std::nullopt,
    .AngleAzimuthFom = std::nullopt,
    .AngleElevationFom = std::nullopt,
    .ElevationFom = std::nullopt,
};
constexpr UwbPeerSpatialProperties UwbPeerSpatialPropertiesAllPopulated{
    .Distance = 1.1,
    .AngleAzimuth = 2.22,
    .AngleElevation = 3.333,
    .Elevation = 4.4444,
    .AngleAzimuthFom = 0xAA,
    .AngleElevationFom = 0xBB,
    .ElevationFom = 0xCC,
};
const std::array<UwbMacAddress, 2> UwbMacAddressesRandom = {
    UwbMacAddress::Random<UwbMacAddressType::Short>(),
    UwbMacAddress::Random<UwbMacAddressType::Extended>(),
};
const auto UwbMacAddressRandomShort = UwbMacAddressesRandom[0];
const auto UwbMacAddressRandomExtended = UwbMacAddressesRandom[1];

const std::array<UwbPeer, 2> PeersRandom = {
    UwbPeer{ UwbMacAddressesRandom[0] },
    UwbPeer{ UwbMacAddressesRandom[1] },
};
const std::array<UwbPeer, 2> PeersWithFullyPopulatedSpatialProperties{
    UwbPeer{ UwbMacAddressesRandom[0], UwbPeerSpatialPropertiesAllPopulated },
    UwbPeer{ UwbMacAddressesRandom[1], UwbPeerSpatialPropertiesAllPopulated },
};
const std::array<UwbPeer, 2> PeersWithFullyEmptySpatialProperties{
    UwbPeer{ UwbMacAddressesRandom[0], UwbPeerSpatialPropertiesAllEmpty },
    UwbPeer{ UwbMacAddressesRandom[1], UwbPeerSpatialPropertiesAllEmpty },
};

} // namespace uwb::test

TEST_CASE("uwb peer objects can be created", "[basic]")
{
    using namespace uwb;

    SECTION("creation doesn't cause a crash")
    {
        for (const auto& macAddress : test::UwbMacAddressesRandom) {
            REQUIRE_NOTHROW(std::make_unique<UwbPeer>(macAddress));
        }
    }

    SECTION("destruction doesn't cause a crash")
    {
        std::unique_ptr<UwbPeer> uwbPeer;

        for (const auto& macAddress : test::UwbMacAddressesRandom) {
            auto peer = std::make_unique<UwbPeer>(macAddress);
            REQUIRE_NOTHROW([&] {
                peer.reset();
            });
        }
    }
}

TEST_CASE("uwb peer reflects static properties", "[basic]")
{
    using namespace uwb;

    SECTION("mac address is reflected")
    {
        for (const auto& macAddress : test::UwbMacAddressesRandom) {
            UwbPeer peer{ macAddress };
            REQUIRE(peer.GetAddress() == macAddress);
        }
    }

    SECTION("spatial properties are reflected")
    {
        for (const auto& peer : test::PeersRandom) {
            REQUIRE(peer.GetSpatialProperties() == UwbPeerSpatialProperties{});
        }
    }
}

TEST_CASE("uwb peers can be copied", "[basic]")
{
    using namespace uwb;

    SECTION("copy constructor works")
    {
        for (const auto& macAddress : test::UwbMacAddressesRandom) {
            UwbPeer peerOriginal{ macAddress };
            UwbPeer peerCopy{ peerOriginal };
            REQUIRE(peerOriginal == peerCopy);
        }
    }

    SECTION("copy-assignment operator works")
    {
        for (const auto& peer : test::PeersRandom) {
            const auto peerCopy = peer;
            REQUIRE(peer == peerCopy);
        }
    }
}

TEST_CASE("uwb peers can be compared for equality", "[basic]")
{
    using namespace uwb;

    SECTION("identical peers compare equal")
    {
        for (const auto& peer : test::PeersRandom) {
            const auto peerCopy = peer;
            REQUIRE(peer == peer);
            REQUIRE(peer == peerCopy);
            REQUIRE(!(peer != peer));
            REQUIRE(!(peer != peerCopy));
        }
    }

    SECTION("differing instances compare unequal")
    {
        for (auto it = std::cbegin(test::PeersRandom); it != std::cend(test::PeersRandom); std::advance(it, 2)) {
            REQUIRE(*it != *std::next(it));
            REQUIRE(!(*it == *std::next(it)));
        }
    }
}

TEST_CASE("uwb peer spatial properties can be created", "[basic]")
{
    using namespace uwb;

    SECTION("creation doesn't cause a crash")
    {
        REQUIRE_NOTHROW(std::make_unique<UwbPeerSpatialProperties>());
    }

    SECTION("destruction doesn't cause a crash")
    {
        auto spatialProperties = std::make_unique<UwbPeerSpatialProperties>();
        REQUIRE_NOTHROW([&] {
            spatialProperties.reset();
        });
    }
}

TEST_CASE("uwb peer spatial properties serialization is stable", "[basic][uwb][serialize]")
{
    using namespace uwb;

    SECTION("all values empty is stable")
    {
        const auto json = nlohmann::json(test::UwbPeerSpatialPropertiesAllEmpty);
        const auto uwbPeerSpatialPropertiesAllEmptylDeserialized = json.get<UwbPeerSpatialProperties>();
        REQUIRE(uwbPeerSpatialPropertiesAllEmptylDeserialized == test::UwbPeerSpatialPropertiesAllEmpty);
    }

    SECTION("all values populated is stable")
    {
        const auto json = nlohmann::json(test::UwbPeerSpatialPropertiesAllPopulated);
        const auto uwbPeerSpatialPropertiesAllPopulatedDeserialized = json.get<UwbPeerSpatialProperties>();
        REQUIRE(uwbPeerSpatialPropertiesAllPopulatedDeserialized == test::UwbPeerSpatialPropertiesAllPopulated);
    }
}

TEST_CASE("uwb peer serialization is stable", "[basic][uwb][serialize]")
{
    using namespace uwb;

    SECTION("peer with short mac address and fully populated spatial properties is stable")
    {
        const UwbPeer uwbPeerOriginal{ test::UwbMacAddressRandomShort, test::UwbPeerSpatialPropertiesAllPopulated };
        const auto json = nlohmann::json(uwbPeerOriginal);
        const auto uwbPeerDeserialized = json.get<UwbPeer>();
        REQUIRE(uwbPeerOriginal == uwbPeerDeserialized);
    }

    SECTION("peer with short mac address and fully empty spatial properties is stable")
    {
        const UwbPeer uwbPeerOriginal{ test::UwbMacAddressRandomShort, test::UwbPeerSpatialPropertiesAllEmpty };
        const auto json = nlohmann::json(uwbPeerOriginal);
        const auto uwbPeerDeserialized = json.get<UwbPeer>();
        REQUIRE(uwbPeerOriginal == uwbPeerDeserialized);
    }

    SECTION("peer with extended mac address and fully populated spatial properties is stable")
    {
        const UwbPeer uwbPeerOriginal{ test::UwbMacAddressRandomExtended, test::UwbPeerSpatialPropertiesAllPopulated };
        const auto json = nlohmann::json(uwbPeerOriginal);
        const auto uwbPeerDeserialized = json.get<UwbPeer>();
        REQUIRE(uwbPeerOriginal == uwbPeerDeserialized);
    }

    SECTION("peer with extended mac address and fully empty spatial properties is stable")
    {
        const UwbPeer uwbPeerOriginal{ test::UwbMacAddressRandomExtended, test::UwbPeerSpatialPropertiesAllEmpty };
        const auto json = nlohmann::json(uwbPeerOriginal);
        const auto uwbPeerDeserialized = json.get<UwbPeer>();
        REQUIRE(uwbPeerOriginal == uwbPeerDeserialized);
    }
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
