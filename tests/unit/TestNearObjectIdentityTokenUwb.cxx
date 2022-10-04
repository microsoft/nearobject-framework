
#include <algorithm>
#include <array>
#include <memory>
#include <type_traits>
#include <utility>

#include <notstd/tostring.hxx>

#include <catch2/catch.hpp>

#include <nearobject/NearObjectIdentityToken.hxx>
#include <nearobject/NearObjectIdentityTokenUwb.hxx>
#include <nearobject/serialization/NearObjectIdentityTokenUwbJsonSerializer.hxx>
#include <nearobject/serialization/UwbMacAddressJsonSerializer.hxx>

using namespace strings::ostream_operators;

namespace nearobject::test
{
/**
 * @brief Helper function which expands the parameter pack for the array values.
 * 
 * @tparam N The number of array values to expand.
 * @tparam values The array values.
 * @param sequence 
 * @return uwb::UwbMacAddress 
 */
template<std::size_t N, uint8_t... values>
uwb::UwbMacAddress
MakeAddressImpl(std::integer_sequence<uint8_t, values...> sequence)
{
    return uwb::UwbMacAddress{ std::array<uint8_t, N>{values...} };
}

/**
 * @brief Create a UwbMacAddress whose value is an increasing sequence of
 * integers, 0, 1, 2, ...
 * 
 * This is useful in tests where the value isn't relevant.
 * 
 * @tparam N The number of integers to assign to the value.
 * @tparam Values The vales to assign.
 * @return requires Constraint for value length.
 */
template<std::size_t N, typename Values = std::make_integer_sequence<uint8_t, N>>
requires uwb::ValidUwbMacAddressLength<N>
uwb::UwbMacAddress
MakeAddress()
{
    return MakeAddressImpl<N>(Values{});
}

/**
 * @brief Create a default short UwbMacAddress with an unspecified sequence of
 * integer values.
 * 
 * @return uwb::UwbMacAddress 
 */
uwb::UwbMacAddress
MakeAddressShort()
{
    return MakeAddress<uwb::UwbMacAddressLength::Short>();
}

/**
 * @brief Create a default extended UwbMacAddress with an unspecified sequence
 * of integer values.
 * 
 * @return uwb::UwbMacAddress 
 */
uwb::UwbMacAddress
MakeAddressExtended()
{
    return MakeAddress<uwb::UwbMacAddressLength::Extended>();
}

/**
 * @brief Create a default uwb-based token with an unspecified short mac address.
 * 
 * @return NearObjectIdentityTokenUwb 
 */
NearObjectIdentityTokenUwb 
MakeTokenShort()
{
    return NearObjectIdentityTokenUwb{ MakeAddressShort() };
}

/**
 * @brief Create a default uwb-based token with an unspecified extended mac address.
 * 
 * @return NearObjectIdentityTokenUwb 
 */
NearObjectIdentityTokenUwb 
MakeTokenExtended()
{
    return NearObjectIdentityTokenUwb{ MakeAddressExtended() };
}

/**
 * @brief Helper to compare equality of two tokens. 
 * 
 * @param lhs 
 * @param rhs 
 * @return true 
 * @return false 
 */
bool
AreTokensEqual(const std::span<const uint8_t>& lhs, const std::span<const uint8_t>& rhs)
{
    return std::ranges::equal(lhs, rhs);
}

/**
 * @brief Default number of local operations to perform in token stability
 * verification. The value was chosen more-or-less at random.
 */
constexpr std::size_t NumStabilityVerificationsDefault = 25;

/**
 * @brief Validate stability of a token, locally.
 * 
 * Local refers to its representation within the object which produces it.
 * Stability refers to the value being invariant.
 * 
 * @tparam NearObjectIdentityType The type of token object.
 * @param identityToken The token object instance to validate.
 * @param numVerificationsTotal The number of internal operations to perform.
 * @return requires The concept restricting the input to NearObjectIdentityToken base class.
 */
template <typename NearObjectIdentityType>
requires std::is_base_of_v<NearObjectIdentityToken, NearObjectIdentityType>
void
ValidateTokenStabilityLocal(const NearObjectIdentityType& identityToken, const std::size_t numVerificationsTotal = NumStabilityVerificationsDefault)
{
    std::span<const uint8_t> tokenLast;
    std::span<const uint8_t> tokenNext = identityToken.GetToken();

    for (std::size_t numVerifications = 0; numVerifications < numVerificationsTotal; numVerifications++) {
        tokenLast = tokenNext;
        tokenNext = identityToken.GetToken();
        REQUIRE(AreTokensEqual(tokenLast, tokenNext));
    }
}

void
ValidateTokenStabilityExternal(const NearObjectIdentityTokenUwb& identityTokenUwbOne)
{
    const auto tokenValueOne = identityTokenUwbOne.GetToken();
    const auto identityTokenUwbTwo = NearObjectIdentityTokenUwb::FromToken(tokenValueOne);
    const auto tokenValueTwo = identityTokenUwbTwo.GetToken();
    REQUIRE(identityTokenUwbOne == identityTokenUwbTwo);
}
} // namespace nearobject::test

TEST_CASE("uwb mac address serialization is stable", "[basic][uwb][serialize]")
{
    using namespace uwb;
    using namespace nearobject; 
    using namespace nearobject::test;

    SECTION("uwb short mac address serialization is stable")
    {
        const auto uwbMacAddressShortOne = MakeAddressShort();
        const auto json = nlohmann::json(uwbMacAddressShortOne);
        const auto uwbMacAddressShortTwo = json.get<UwbMacAddress>();
        REQUIRE(uwbMacAddressShortOne == uwbMacAddressShortTwo);
    }
}

TEST_CASE("near object uwb identity token can be created", "[basic]")
{
    using namespace nearobject;
    using namespace nearobject::test;

    SECTION("can be created with short address without causing a crash")
    {
        REQUIRE_NOTHROW(std::make_unique<NearObjectIdentityTokenUwb>(MakeAddressShort()));
    }

    SECTION("can be created with extended address without causing a crash")
    {
        REQUIRE_NOTHROW(std::make_unique<NearObjectIdentityTokenUwb>(MakeAddressExtended()));
    }
}

TEST_CASE("near object uwb identity token binds uwb mac address", "[basic]")
{
    using namespace nearobject;
    using namespace nearobject::test;

    SECTION("token binds short uwb mac addresses")
    {
        auto uwbMacAddressShort = MakeAddressShort();
        NearObjectIdentityTokenUwb identityTokenUwb{ uwbMacAddressShort };
        REQUIRE(identityTokenUwb.GetMacAddress() == uwbMacAddressShort);
    }

    SECTION("token retrieval does not mutate short uwb addresses")
    {
        auto uwbMacAddressShort = MakeAddressShort();
        NearObjectIdentityTokenUwb identityTokenUwb{ uwbMacAddressShort };
        REQUIRE(identityTokenUwb.GetMacAddress() == uwbMacAddressShort);
        auto token = identityTokenUwb.GetToken();
        REQUIRE(identityTokenUwb.GetMacAddress() == uwbMacAddressShort);
    }

    SECTION("token binds extended uwb addresses")
    {
        auto uwbMacAddressExtended = MakeAddressExtended();
        NearObjectIdentityTokenUwb identityTokenUwb{ uwbMacAddressExtended };
        REQUIRE(identityTokenUwb.GetMacAddress() == uwbMacAddressExtended);
    }

    SECTION("token retrieval does not mutate extended uwb addresses")
    {
        auto uwbMacAddressExtended = MakeAddressExtended();
        NearObjectIdentityTokenUwb identityTokenUwb{ uwbMacAddressExtended };
        REQUIRE(identityTokenUwb.GetMacAddress() == uwbMacAddressExtended);
        auto token = identityTokenUwb.GetToken();
        REQUIRE(identityTokenUwb.GetMacAddress() == uwbMacAddressExtended);
    }
}

TEST_CASE("near object uwb identity token retrieval does not mutate attributes", "[basic][serialize]")
{
    using namespace nearobject;
    using namespace nearobject::test;

    SECTION("token retrieval does not mutate attributes for short uwb mac address tokens")
    {
        auto uwbMacAddressShort = MakeAddressShort();
        NearObjectIdentityTokenUwb identityTokenUwb{ uwbMacAddressShort };
        REQUIRE(identityTokenUwb.GetMacAddress() == uwbMacAddressShort);
        auto token = identityTokenUwb.GetToken();
        REQUIRE(identityTokenUwb.GetMacAddress() == uwbMacAddressShort);
    }

    SECTION("token retrieval does not mutate attributes for extended uwb mac address tokens")
    {
        auto uwbMacAddressExtended = MakeAddressExtended();
        NearObjectIdentityTokenUwb identityTokenUwb{ uwbMacAddressExtended };
        REQUIRE(identityTokenUwb.GetMacAddress() == uwbMacAddressExtended);
        auto token = identityTokenUwb.GetToken();
        REQUIRE(identityTokenUwb.GetMacAddress() == uwbMacAddressExtended);
    }
}

TEST_CASE("near object uwb identity token serialization is locally stable", "[basic][serialize]")
{
    using namespace nearobject;
    using namespace nearobject::test;

    SECTION("token is stable for short uwb mac addresses")
    {
        const auto identityTokenUwb = MakeTokenShort();
        ValidateTokenStabilityLocal(identityTokenUwb);
    }

    SECTION("token is stable for extended uwb mac addresses")
    {
        const auto identityTokenUwb = MakeTokenExtended();
        ValidateTokenStabilityLocal(identityTokenUwb);
    }

    SECTION("token is stable for short uwb mac addresses through polymorphic base")
    {
        std::unique_ptr<NearObjectIdentityToken> identityToken;
        identityToken = std::make_unique<NearObjectIdentityTokenUwb>(MakeAddressShort());
        ValidateTokenStabilityLocal(*identityToken);
    }

    SECTION("token is stable for extended uwb mac addresses through polymorphic base")
    {
        std::unique_ptr<NearObjectIdentityToken> identityToken;
        identityToken = std::make_unique<NearObjectIdentityTokenUwb>(MakeAddressExtended());
        ValidateTokenStabilityLocal(*identityToken);
    }
}

TEST_CASE("near object uwb identity token serialization is externally stable", "[basic][serialize]")
{
    using namespace nearobject;
    using namespace nearobject::test;

    SECTION("direct token generation methods are stable")
    {
        const NearObjectIdentityTokenUwb identityTokenUwbOne{};
        const auto tokenValueOne = NearObjectIdentityTokenUwb::ToToken(identityTokenUwbOne);
        const auto identityTokenUwbTwo = NearObjectIdentityTokenUwb::FromToken(tokenValueOne);
        REQUIRE(identityTokenUwbOne == identityTokenUwbTwo);
    }

    SECTION("direct token generation methods are stable with temporaries")
    {
        const NearObjectIdentityTokenUwb identityTokenUwb{};
        REQUIRE(NearObjectIdentityTokenUwb::FromToken(NearObjectIdentityTokenUwb::ToToken(identityTokenUwb)) == identityTokenUwb);
    }

    SECTION("token is stable for short uwb mac addresses")
    {
        ValidateTokenStabilityExternal(MakeTokenShort());
    }

    SECTION("token is stable for extended uwb mac addresses")
    {
        ValidateTokenStabilityExternal(MakeTokenExtended());
    }
}
