
#include <array>
#include <cstdint>
#include <memory>

#include <catch2/catch_test_macros.hpp>
#include <nearobject/service/NearObjectService.hxx>

namespace nearobject::service::test
{
struct NearObjectSessionIdGeneratorTest :
    public NearObjectSessionIdGenerator
{
    uint32_t 
    GetNext() noexcept override
    {
        return m_id++;
    }

private:
    uint32_t m_id{0};
};
} // namespace nearobject::service::test

TEST_CASE("near object service can be created", "[basic][service]")
{
    using namespace nearobject::service;

    SECTION("service doesn't cause a crash when created with default-initialized injector")
    {
        auto service = NearObjectService::Create({});
    }

    SECTION("service doesn't case a crash when created with missing components")
    {
        {   // missing profile manager
            auto service = NearObjectService::Create({
                nullptr,
                NearObjectDeviceControllerManager::Create(),
            });
        }
        {
            // missing device manager
            auto service = NearObjectService::Create({
                std::make_shared<NearObjectProfileManager>(),
                nullptr,
                nullptr,
            });
        }
        {
            // missing session id generator
            auto service = NearObjectService::Create({
                std::make_shared<NearObjectProfileManager>(),
                NearObjectDeviceControllerManager::Create(),
                nullptr,
            });
        }
        {
            // missing profile manager, device manager, and session id generator 
            auto service = NearObjectService::Create({
                nullptr,
                nullptr,
                nullptr,
            });
        }
    }

    SECTION("service exposes same components as provided by the injector")
    {
        std::shared_ptr<NearObjectProfileManager> profileManager;
        std::shared_ptr<NearObjectDeviceControllerManager> deviceManager;
        NearObjectSessionIdGenerator *sessionIdGeneratorPtr = nullptr;

        {
            profileManager = nullptr;
            deviceManager = nullptr;
            std::unique_ptr<NearObjectSessionIdGenerator> sessionIdGenerator = std::make_unique<test::NearObjectSessionIdGeneratorTest>();
            sessionIdGeneratorPtr = sessionIdGenerator.get();

            auto service = NearObjectService::Create({ profileManager, deviceManager, std::move(sessionIdGenerator) });
            REQUIRE(service->ProfileManager == profileManager);
            REQUIRE(service->DeviceManager == deviceManager);
            REQUIRE(service->SessionIdGenerator.get() == sessionIdGeneratorPtr);
        }
        {
            std::unique_ptr<NearObjectSessionIdGenerator> sessionIdGenerator = std::make_unique<test::NearObjectSessionIdGeneratorTest>();
            sessionIdGeneratorPtr = sessionIdGenerator.get();
            profileManager = std::make_shared<NearObjectProfileManager>();
            deviceManager = NearObjectDeviceControllerManager::Create();

            auto service = NearObjectService::Create({ profileManager, deviceManager, std::move(sessionIdGenerator) });
            REQUIRE(service->ProfileManager == profileManager);
            REQUIRE(service->DeviceManager == deviceManager);
            REQUIRE(service->SessionIdGenerator.get() == sessionIdGeneratorPtr);
        }
    }

    SECTION("service provides a default session id generator when one is not specified")
    {
        // missing session id generator
        auto service = NearObjectService::Create({
            std::make_shared<NearObjectProfileManager>(),
            NearObjectDeviceControllerManager::Create(),
            nullptr,
        });

        REQUIRE(service->SessionIdGenerator != nullptr);
    }
}
