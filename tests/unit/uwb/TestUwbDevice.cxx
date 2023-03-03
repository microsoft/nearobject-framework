
#include <cstdint>
#include <memory>

#include <uwb/UwbDevice.hxx>
#include <uwb/UwbSession.hxx>
#include <uwb/UwbSessionEventCallbacks.hxx>

#include <catch2/catch_test_macros.hpp>

// NOLINTBEGIN(cppcoreguidelines-avoid-magic-numbers)

namespace uwb::test
{
struct UwbDeviceTestBase : public uwb::UwbDevice
{
    explicit UwbDeviceTestBase(uint16_t id) :
        Id(id)
    {}

    uint16_t Id;

    std::shared_ptr<UwbSession>
    CreateSessionImpl(std::weak_ptr<UwbSessionEventCallbacks> /* callbacks */) override
    {
        return nullptr;
    }

    uwb::protocol::fira::UwbCapability
    GetCapabilitiesImpl() override
    {
        return {};
    }

    uwb::protocol::fira::UwbDeviceInformation
    GetDeviceInformationImpl() override
    {
        return {};
    }
};

struct UwbDeviceTestDerivedOne : UwbDeviceTestBase
{
    explicit UwbDeviceTestDerivedOne(uint8_t id) :
        UwbDeviceTestBase(static_cast<uint16_t>(id << 8UL))
    {}

    bool
    IsEqual(const UwbDevice& other) const noexcept override
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
        const auto& rhs = static_cast<const UwbDeviceTestDerivedOne&>(other);
        return (this->Id == rhs.Id);
    }
};
struct UwbDeviceTestDerivedTwo : UwbDeviceTestBase
{
    explicit UwbDeviceTestDerivedTwo(uint8_t id) :
        UwbDeviceTestBase(static_cast<uint16_t>(id))
    {}

    bool
    IsEqual(const UwbDevice& other) const noexcept override
    {
        // NOLINTNEXTLINE(cppcoreguidelines-pro-type-static-cast-downcast)
        const auto& rhs = static_cast<const UwbDeviceTestDerivedTwo&>(other);
        return (this->Id == rhs.Id);
    }
};
} // namespace uwb::test

TEST_CASE("uwb devices can be compared for equality", "[basic]")
{
    using namespace uwb;

    SECTION("identical devices compare equal directly")
    {
        test::UwbDeviceTestDerivedOne uwbDeviceInstanceOne{ 1 };
        test::UwbDeviceTestDerivedOne uwbDeviceInstanceTwo{ 1 };
        REQUIRE(uwbDeviceInstanceOne == uwbDeviceInstanceTwo);
    }

    SECTION("identical devices compare equal polymorphically")
    {
        std::unique_ptr<test::UwbDeviceTestBase> uwbDeviceInstanceOne = std::make_unique<test::UwbDeviceTestDerivedOne>(1);
        std::unique_ptr<test::UwbDeviceTestBase> uwbDeviceInstanceTwo = std::make_unique<test::UwbDeviceTestDerivedOne>(1);
        REQUIRE(*uwbDeviceInstanceOne == *uwbDeviceInstanceTwo);
    }

    SECTION("devices of different type compare unequal directly")
    {
        test::UwbDeviceTestDerivedOne uwbDeviceOne{ 1 };
        test::UwbDeviceTestDerivedTwo uwbDeviceTwo{ 1 };
        REQUIRE(uwbDeviceTwo != uwbDeviceOne);
    }

    SECTION("devices of different type compare unequal polymorphically")
    {
        std::unique_ptr<test::UwbDeviceTestBase> uwbDeviceInstanceOne = std::make_unique<test::UwbDeviceTestDerivedOne>(1);
        std::unique_ptr<test::UwbDeviceTestBase> uwbDeviceInstanceTwo = std::make_unique<test::UwbDeviceTestDerivedTwo>(1);
        REQUIRE(*uwbDeviceInstanceOne != *uwbDeviceInstanceTwo);
    }

    SECTION("devices of same type but different instance compare unequal directly")
    {
        test::UwbDeviceTestDerivedOne uwbDeviceInstanceOne{ 1 };
        test::UwbDeviceTestDerivedOne uwbDeviceInstanceTwo{ 2 };
        REQUIRE(uwbDeviceInstanceOne != uwbDeviceInstanceTwo);
    }

    SECTION("devices of same type but different instance compare unequal polymorphically")
    {
        std::unique_ptr<test::UwbDeviceTestBase> uwbDeviceInstanceOne = std::make_unique<test::UwbDeviceTestDerivedOne>(1);
        std::unique_ptr<test::UwbDeviceTestBase> uwbDeviceInstanceTwo = std::make_unique<test::UwbDeviceTestDerivedOne>(2);
        REQUIRE(*uwbDeviceInstanceOne != *uwbDeviceInstanceTwo);
    }

    SECTION("equal and not equal checks produce logically consistent values directly")
    {
        test::UwbDeviceTestDerivedOne uwbDeviceInstanceOne{ 1 };
        test::UwbDeviceTestDerivedOne uwbDeviceInstanceTwo{ 1 };
        REQUIRE(uwbDeviceInstanceOne == uwbDeviceInstanceTwo);
        REQUIRE(!(uwbDeviceInstanceOne != uwbDeviceInstanceTwo));
    }

    SECTION("equal and not equal checks produce logically consistent values polymorphically")
    {
        std::unique_ptr<test::UwbDeviceTestBase> uwbDeviceInstanceOne = std::make_unique<test::UwbDeviceTestDerivedOne>(1);
        std::unique_ptr<test::UwbDeviceTestBase> uwbDeviceInstanceTwo = std::make_unique<test::UwbDeviceTestDerivedOne>(1);
        REQUIRE(*uwbDeviceInstanceOne == *uwbDeviceInstanceTwo);
        REQUIRE(!(*uwbDeviceInstanceOne != *uwbDeviceInstanceTwo));
    }
}

// NOLINTEND(cppcoreguidelines-avoid-magic-numbers)
