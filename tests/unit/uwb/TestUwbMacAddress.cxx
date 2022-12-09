
#include <algorithm>
#include <array>
#include <cctype>
#include <istream>
#include <ostream>
#include <ranges>
#include <sstream>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <catch2/catch_test_macros.hpp>

#include <uwb/UwbMacAddress.hxx>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

namespace uwb::test
{
static constexpr std::array<uint8_t, 2> AddressShortValueZero{
    0x00,
    0x00,
};

static constexpr std::array<uint8_t, 2> AddressShortValueAllOnes{
    0xff,
    0xff,
};

static constexpr std::array<uint8_t, 8> AddressExtendedValueZero{
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
    0x00,
};

static constexpr std::array<uint8_t, 8> AddressExtendedValueAllOnes{
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
    0xff,
};

static constexpr std::array<std::array<uint8_t, 2>, 2> AddressShortValues{
    AddressShortValueZero,
    AddressShortValueAllOnes,
};

static constexpr std::array<std::array<uint8_t, 8>, 2> AddressExtendedValues{
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

/**
 * @brief Validates that a uwb mac address can be parsed from an input stream. 
 * 
 * @param uwbMacAddressString The string to encode in the stream.
 * @param uwbMacAddressExpected The expected parsed uwb mac address.
 */
void
ValidateUwbMacAddressParsing(const std::string& uwbMacAddressString, const uwb::UwbMacAddress uwbMacAddressExpected)
{
    uwb::UwbMacAddress uwbMacAddressActual;
    std::stringstream ss{ uwbMacAddressString };
    ss >> uwbMacAddressActual;
    REQUIRE(uwbMacAddressActual == uwbMacAddressExpected);
}

/**
 * @brief Validates that an invalid uwb mac address encoding does not output a
 * valid UwbMacAddress object.
 * 
 * @param uwbMacAddressString The invalid uwb mac address string.
 */
void
ValidateUwbMacAddressParsingFailed(const std::string& uwbMacAddressString)
{
    uwb::UwbMacAddress uwbMacAddressActual;
    std::stringstream ss{ uwbMacAddressString };
    ss >> uwbMacAddressActual;
    // Validate parsing flagged an error.
    REQUIRE(ss.fail());
    // Validate the destination object is unchanged.
    REQUIRE(uwbMacAddressActual == UwbMacAddress{});
}

/**
 * @brief Convert an input string to lower-case.
 * 
 * @param str The string to convert.
 * @return std::string The converted string.
 */
std::string
ToLower(const std::string& str)
{
    std::string lower(str);
    std::transform(std::cbegin(lower), std::cend(lower), std::begin(lower), [](auto&& c) {
       return std::tolower(c); 
    });

    return lower;
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
        REQUIRE(UwbMacAddress{ AddressShortValueZero } == UwbMacAddress{ AddressShortValueZero });
    }

    SECTION("same extended addresses match")
    {
        REQUIRE(UwbMacAddress{ AddressExtendedValueZero } == UwbMacAddress{ AddressExtendedValueZero });
    }

    SECTION("differing short addresses don't match")
    {
        REQUIRE(UwbMacAddress{ AddressShortValueZero } != UwbMacAddress{ AddressShortValueAllOnes });
    }

    SECTION("differing extended addresses don't match")
    {
        REQUIRE(UwbMacAddress{ AddressExtendedValueZero } != UwbMacAddress{ AddressExtendedValueAllOnes });
    }

    SECTION("differing address types don't match")
    {
        REQUIRE(UwbMacAddress{ AddressShortValueZero } != UwbMacAddress{ AddressExtendedValueZero });
        REQUIRE(UwbMacAddress{ AddressShortValueAllOnes } != UwbMacAddress{ AddressExtendedValueAllOnes });
    }
}

TEST_CASE("uwb address type is accurate")
{
    using namespace uwb;
    using namespace uwb::test;

    SECTION("short type is accurate")
    {
        REQUIRE(UwbMacAddress{ AddressShortValueZero }.GetType() == UwbMacAddressType::Short);
    }

    SECTION("extended type is accurate")
    {
        REQUIRE(UwbMacAddress{ AddressExtendedValueZero }.GetType() == UwbMacAddressType::Extended);
    }
}

TEST_CASE("uwb address length is accurate")
{
    using namespace uwb;
    using namespace uwb::test;

    SECTION("short length is accurate")
    {
        const UwbMacAddress addressShort{ AddressShortValueZero };
        ValidateLength(addressShort, UwbMacAddressLength::Short);
    }

    SECTION("extended length is accurate")
    {
        const UwbMacAddress addressExtrended{ AddressExtendedValueZero };
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
            const UwbMacAddress addressShort{ addressShortValue };
            const auto& value = addressShort.GetValue();
            REQUIRE(std::equal(std::cbegin(value), std::cend(value), std::cbegin(addressShortValue)));
        }
    }

    SECTION("extended value is accurate")
    {
        for (const auto& addressExtendedValue : AddressExtendedValues) {
            const UwbMacAddress addressExtended{ addressExtendedValue };
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

TEST_CASE("uwb address can be used with stream operators", "[basic][io]")
{
    using namespace uwb;

    static const std::unordered_map<std::string, uwb::UwbMacAddress> UwbMacAddressStringMap = {
        { "00:00", uwb::UwbMacAddress{ std::array<uint8_t, 2>{ 0x00, 0x00 } } },
        { "BE:EF", uwb::UwbMacAddress{ std::array<uint8_t, 2>{ 0xBE, 0xEF } } },
        { "BE:EF:", uwb::UwbMacAddress{ std::array<uint8_t, 2>{ 0xBE, 0xEF } } },
        { "BE:EF:::::::", uwb::UwbMacAddress{ std::array<uint8_t, 2>{ 0xBE, 0xEF } } },
        { "BE:EF:garbage", uwb::UwbMacAddress{ std::array<uint8_t, 2>{ 0xBE, 0xEF } } },
        { "0F:F1:CE", uwb::UwbMacAddress{ std::array<uint8_t, 2>{ 0x0F, 0xF1 } } },
        { "CA:FE:D0:0D", uwb::UwbMacAddress{ std::array<uint8_t, 2>{ 0xCA, 0xFE } } },
        { "00:11:22:33:44:55:66:77", uwb::UwbMacAddress{ std::array<uint8_t, 8>{ 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77 } } },
        { "01:AB:CD:EF:FE:DC:BA:10", uwb::UwbMacAddress{ std::array<uint8_t, 8>{ 0x01, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x10 } } },
        { "01:AB:CD:EF:FE:DC:BA:10:", uwb::UwbMacAddress{ std::array<uint8_t, 8>{ 0x01, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x10 } } },
        { "01:AB:CD:EF:FE:DC:BA:10:junko", uwb::UwbMacAddress{ std::array<uint8_t, 8>{ 0x01, 0xAB, 0xCD, 0xEF, 0xFE, 0xDC, 0xBA, 0x10 } } }
    };

    static const auto UwbMacAddressStringsInvalid = {
        "",
        "0",
        "0011",
        ":01:01",
        "))((",
        "000000000000000000000000000",
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ",
        "aaaaaaaaaaaaaaaaaaaa",
        "!!@@##$$%%^^&&**",
    };

    SECTION("short addresses can be parsed from an istream")
    {
        // Create a view of short addresses.
        auto shortAddresses = UwbMacAddressStringMap | std::views::filter([](const auto& value) {
            const auto& [_, uwbMacAddress] = value;
            return (uwbMacAddress.GetType() == uwb::UwbMacAddressType::Short);
        });

        for (const auto& [uwbMacAddressString, uwbMacAddressExpected] : shortAddresses) {
            const auto uwbMacAddressStringLower = test::ToLower(uwbMacAddressString);
            test::ValidateUwbMacAddressParsing(uwbMacAddressString, uwbMacAddressExpected);
            test::ValidateUwbMacAddressParsing(uwbMacAddressStringLower, uwbMacAddressExpected);
        }
    }

    SECTION("extended addresses can be parsed from an istream")
    {
        // Create a view of extended addresses.
        auto extendedAddresses = UwbMacAddressStringMap | std::views::filter([](const auto& value) {
            const auto& [_, uwbMacAddress] = value;
            return (uwbMacAddress.GetType() == uwb::UwbMacAddressType::Short);
        });

        for (const auto& [uwbMacAddressString, uwbMacAddressExpected] : extendedAddresses) {
            const auto uwbMacAddressStringLower = test::ToLower(uwbMacAddressString);
            test::ValidateUwbMacAddressParsing(uwbMacAddressString, uwbMacAddressExpected);
            test::ValidateUwbMacAddressParsing(uwbMacAddressStringLower, uwbMacAddressExpected);
        }
    }

    SECTION("invalid addresses are not parsed")
    {
        for (const auto& uwbMacAddressStringInvalid : UwbMacAddressStringsInvalid) {
            test::ValidateUwbMacAddressParsingFailed(uwbMacAddressStringInvalid);
        }
    }
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
