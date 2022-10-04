
#include <algorithm>
#include <array> 
#include <unordered_set>

#include <catch2/catch.hpp>

#include <uwb/UwbMacAddress.hxx>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

namespace uwb::test
{
static constexpr std::array<uint8_t, 2> AddressShortValueZero{
    0x00, 0x00,
};

static constexpr std::array<uint8_t, 2> AddressShortValueAllOnes{
    0xff, 0xff,
};

static constexpr std::array<uint8_t, 8> AddressExtendedValueZero{
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

static constexpr std::array<uint8_t, 8> AddressExtendedValueAllOnes{
    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
};

static constexpr std::array<std::array<uint8_t, 2>, 2> AddressShortValues {
    AddressShortValueZero,
    AddressShortValueAllOnes,
};

static constexpr std::array<std::array<uint8_t, 8>, 2> AddressExtendedValues {
    AddressExtendedValueZero,
    AddressExtendedValueAllOnes,
};

/**
 * @brief Validates the length of the mac address.
 * 
 * @param uwbMacAddress The mac address to validate the length.
 * @param lengthExpected The expected length.
 */
void 
ValidateLength(const UwbMacAddress& uwbMacAddress, std::size_t lengthExpected) noexcept
{
    REQUIRE(uwbMacAddress.GetLength() == lengthExpected);
    REQUIRE(std::size(uwbMacAddress.GetValue()) == lengthExpected);
}
} // namespace uwb::test

TEST_CASE("uwb short mac address can be created", "[basic]")
{
    using namespace uwb;
}

TEST_CASE("uwb extended mac address can be created", "[basic]")
{
    using namespace uwb;
}

TEST_CASE("uwb addresses can be checked for equality correctly", "[basic]")
{
    using namespace uwb;
    using namespace uwb::test;

    SECTION("same short addresses match")
    {
        REQUIRE(UwbMacAddress{AddressShortValueZero} == UwbMacAddress{AddressShortValueZero});
    }

    SECTION("same extended addresses match")
    {
        REQUIRE(UwbMacAddress{AddressExtendedValueZero} == UwbMacAddress{AddressExtendedValueZero});
    }

    SECTION("differing short addresses don't match")
    {
        REQUIRE(UwbMacAddress{AddressShortValueZero} != UwbMacAddress{AddressShortValueAllOnes});
    }

    SECTION("differing extended addresses don't match")
    {
        REQUIRE(UwbMacAddress{AddressExtendedValueZero} != UwbMacAddress{AddressExtendedValueAllOnes});
    }

    SECTION("differing address types don't match")
    {
        REQUIRE(UwbMacAddress{AddressShortValueZero} != UwbMacAddress{AddressExtendedValueZero});
        REQUIRE(UwbMacAddress{AddressShortValueAllOnes} != UwbMacAddress{AddressExtendedValueAllOnes});
    }
}

TEST_CASE("uwb address type is accurate")
{
    using namespace uwb;
    using namespace uwb::test;

    SECTION("short type is accurate")
    {
        REQUIRE(UwbMacAddress{AddressShortValueZero}.GetType() == UwbMacAddressType::Short);
    }

    SECTION("extended type is accurate")
    {
        REQUIRE(UwbMacAddress{AddressExtendedValueZero}.GetType() == UwbMacAddressType::Extended);
    }
}

TEST_CASE("uwb address length is accurate")
{
    using namespace uwb;
    using namespace uwb::test;

    SECTION("short length is accurate")
    {
        const UwbMacAddress addressShort{AddressShortValueZero};
        ValidateLength(addressShort, UwbMacAddressLength::Short);
    }

    SECTION("extended length is accurate")
    {
        const UwbMacAddress addressExtrended{AddressExtendedValueZero};
        ValidateLength(addressExtrended, UwbMacAddressLength::Extended);
    }
}

TEST_CASE("uwb address value is accurate")
{
    using namespace uwb;
    using namespace uwb::test;

    SECTION("short value is accurate")
    {
        for (const auto& addressShortValue : AddressShortValues) {
            const UwbMacAddress addressShort{addressShortValue};
            const auto& value = addressShort.GetValue();
            REQUIRE(std::equal(std::cbegin(value), std::cend(value), std::cbegin(addressShortValue)));
        }
    }

    SECTION("extended value is accurate")
    {
        for (const auto& addressExtendedValue : AddressExtendedValues) {
            const UwbMacAddress addressExtended{addressExtendedValue};
            const auto& value = addressExtended.GetValue();
            REQUIRE(std::equal(std::cbegin(value), std::cend(value), std::cbegin(addressExtendedValue)));
        }
    }
}

TEST_CASE("uwb address type traits are accurate")
{
    using namespace uwb;
    using namespace uwb::test;

    SECTION("short type trait can be used to initialize short address")
    {
        // This test doesn't do anything other than to ensure compilation.
        const UwbMacAddress::ShortType addressShortValue{ AddressShortValueZero };
        const UwbMacAddress addressShort{ addressShortValue };
    }

    SECTION("extended type trait can be used to initialize extended address")
    {
        // This test doesn't do anything other than to ensure compilation.
        const UwbMacAddress::ExtendedType addressExtendedValue{ AddressExtendedValueZero };
        const UwbMacAddress addressExtended{ addressExtendedValue };
    }
}

TEST_CASE("uwb address can be used in unordered_set", "[basic][container]")
{
    using namespace uwb;

    std::vector<std::pair<UwbMacAddress, bool>> macAddressesIn{
        { UwbMacAddress{ std::array<uint8_t, 2>{ 0xAA, 0xBB } }, true },
        { UwbMacAddress{ std::array<uint8_t, 2>{ 0xBB, 0xAA } }, true },
        { UwbMacAddress{ std::array<uint8_t, 2>{ 0xBB, 0xAA } }, false },
    };

    std::unordered_set<UwbMacAddress> macAddresses{};

    for (const auto& [macAddress, expected] : macAddressesIn) {
        auto [_, inserted] = macAddresses.insert(macAddress);
        REQUIRE(inserted == expected);
    }
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
