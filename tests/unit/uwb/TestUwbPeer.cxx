
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
const std::array<UwbMacAddress, 2> UwbMacAddressesRandom = {
    UwbMacAddress::Random<UwbMacAddressType::Short>(),
    UwbMacAddress::Random<UwbMacAddressType::Extended>(),
};
const std::array<UwbPeer, 2> PeersRandom = {
    UwbPeer{ UwbMacAddressesRandom[0] },
    UwbPeer{ UwbMacAddressesRandom[1] },
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

    constexpr UwbPeerSpatialProperties UwbPeerSpatialPropertiesAllEmpty {
        .Distance = std::nullopt,
        .AngleAzimuth = std::nullopt,
        .AngleElevation = std::nullopt,
        .Elevation = std::nullopt,
        .AngleAzimuthFom = std::nullopt,
        .AngleElevationFom = std::nullopt,
        .ElevationFom = std::nullopt,
    };

    constexpr UwbPeerSpatialProperties UwbPeerSpatialPropertiesAllPopulated {
        .Distance = 1.0,
        .AngleAzimuth = 2.0,
        .AngleElevation = 3.0,
        .Elevation = 4.0,
        .AngleAzimuthFom = 0xAA,
        .AngleElevationFom = 0xBB,
        .ElevationFom = 0xCC,
    };

    SECTION("all values empty is stable")
    {
        const auto json = nlohmann::json(UwbPeerSpatialPropertiesAllEmpty);
        const auto uwbPeerSpatialPropertiesAllEmptylDeserialized = json.get<UwbPeerSpatialProperties>();
        REQUIRE(uwbPeerSpatialPropertiesAllEmptylDeserialized == UwbPeerSpatialPropertiesAllEmpty);
    }

    SECTION("all values populated is stable")
    {
        const auto json = nlohmann::json(UwbPeerSpatialPropertiesAllPopulated);
        const auto uwbPeerSpatialPropertiesAllPopulatedDeserialized = json.get<UwbPeerSpatialProperties>();
        REQUIRE(uwbPeerSpatialPropertiesAllPopulatedDeserialized == UwbPeerSpatialPropertiesAllPopulated);
    }
}

TEST_CASE("uwb peer serialization is stable", "[basic][uwb][serialize]")
{
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
