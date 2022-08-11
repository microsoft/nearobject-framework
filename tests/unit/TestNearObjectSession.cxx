
#include <array>
#include <memory>

#include <catch2/catch.hpp>

#include <nearobject/NearObjectSession.hxx>
#include <nearobject/NearObjectSessionEventCallbacks.hxx>

namespace nearobject
{
namespace test
{
static constexpr NearObjectCapabilities AllCapabilitiesSupported = {
    true, // SupportsRanging
    true, // SupportsPositioning
    true, // SupportsSecureDevice
    true, // SupportsSecureChannels
};

struct NearObjectSessionEventCallbacksNoop final :
    public NearObjectSessionEventCallbacks
{
    void
    OnSessionEnded(NearObjectSession *) override
    {}

    void
    OnRangingStarted(NearObjectSession *) override
    {}

    void
    OnRangingStopped(NearObjectSession *) override
    {}

    void
    OnNearObjectPropertiesChanged(NearObjectSession *, const std::vector<std::shared_ptr<NearObject>>) override
    {}

    void
    OnSessionMembershipChanged(NearObjectSession *, const std::vector<std::shared_ptr<NearObject>>, const std::vector<std::shared_ptr<NearObject>>) override
    {}
};

} // namespace test
} // namespace nearobject

TEST_CASE("near object peers can be added at session creation", "[basic]")
{
    using namespace nearobject;

    const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();

    SECTION("creation with 0 peers doesn't cause a crash")
    {
        NearObjectSession session(test::AllCapabilitiesSupported, {}, callbacksNoop);
    }

    SECTION("creation with 1 peer doesn't cause a crash")
    {
        auto nearObject = std::make_shared<NearObject>();
        NearObjectSession session(test::AllCapabilitiesSupported, { nearObject }, callbacksNoop);
    }

    SECTION("creation with 1+ peers doesn't cause a crash")
    {
        const std::vector<std::shared_ptr<NearObject>> nearObjects{
            std::make_shared<NearObject>(),
            std::make_shared<NearObject>(),
            std::make_shared<NearObject>(),
        };

        NearObjectSession session(test::AllCapabilitiesSupported, nearObjects, callbacksNoop);
    }
}

TEST_CASE("near object session capabilities are accurate", "[basic]")
{
    using namespace nearobject;

    const auto &capabilities = test::AllCapabilitiesSupported;

    SECTION("capabilities match post-creation")
    {
        const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(capabilities, {}, callbacksNoop);
        REQUIRE(session.GetCapabilities() == capabilities);
    }

    SECTION("capabilities don't change during a session")
    {
        const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(capabilities, {}, callbacksNoop);
        session.StartRanging();
        REQUIRE(session.GetCapabilities() == capabilities);
        session.StopRanging();
        REQUIRE(session.GetCapabilities() == capabilities);
    }
}

TEST_CASE("near object event handlers can be registered", "[basic]")
{
    using namespace nearobject;

    SECTION("registering a handler doesn't cause a crash")
    {
        const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(test::AllCapabilitiesSupported, {}, callbacksNoop);
    }

    SECTION("starting and ending a ranging session doesn't cause a crash")
    {
        const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(test::AllCapabilitiesSupported, {}, callbacksNoop);
        session.StartRanging();
        session.StopRanging();
    }

    SECTION("handler being deleted doesn't cause a crash")
    {
        auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(test::AllCapabilitiesSupported, {}, callbacksNoop);
        session.StartRanging();
        callbacksNoop.reset();
        session.StopRanging();
    }

    SECTION("callbacks provide the session pointer for which it was registered")
    {
        struct NearObjectSessionEventCallbacksCheckSessionPointer final :
            public NearObjectSessionEventCallbacks
        {
            void
            OnSessionEnded(NearObjectSession *session) override
            {
                CHECK(Session == session);
            }

            void
            OnRangingStarted(NearObjectSession *session) override
            {
                CHECK(Session == session);
            }

            void
            OnRangingStopped(NearObjectSession *session) override
            {
                CHECK(Session == session);
            }

            void
            OnNearObjectPropertiesChanged(NearObjectSession *session, const std::vector<std::shared_ptr<NearObject>> /* nearObjectsChanged */) override
            {
                CHECK(Session == session);
            }

            void
            OnSessionMembershipChanged(NearObjectSession *session, const std::vector<std::shared_ptr<NearObject>> nearObjectsAdded, const std::vector<std::shared_ptr<NearObject>> nearObjectsRemoved) override
            {
                CHECK(Session == session);
            }

            NearObjectSession *Session{ nullptr };
        };

        const auto callbacks = std::make_shared<NearObjectSessionEventCallbacksCheckSessionPointer>();
        NearObjectSession session(test::AllCapabilitiesSupported, {}, callbacks);
        callbacks->Session = &session;
        session.StartRanging();
        session.StopRanging();
        // TODO: trigger NO property changes
        // TODO: trigger NO membership changes
    }
}
