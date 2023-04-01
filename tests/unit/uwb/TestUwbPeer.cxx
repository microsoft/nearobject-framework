
#include <array>
#include <iterator>
#include <memory>
#include <type_traits>

#include <uwb/UwbPeer.hxx>

#include <catch2/catch_test_macros.hpp>

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
