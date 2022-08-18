
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

struct NearObjectSessionEventCallbacksNoop :
    public NearObjectSessionEventCallbacks
{
    virtual void
    OnSessionEnded(NearObjectSession *) override
    {}

    virtual void
    OnRangingStarted(NearObjectSession *) override
    {}

    virtual void
    OnRangingStopped(NearObjectSession *) override
    {}

    virtual void
    OnNearObjectPropertiesChanged(NearObjectSession *, const std::vector<std::shared_ptr<NearObject>>) override
    {}

    virtual void
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
        REQUIRE_NOTHROW(std::make_unique<NearObjectSession>(test::AllCapabilitiesSupported, std::vector<std::shared_ptr<NearObject>>{}, callbacksNoop));
    }

    SECTION("creation with 1 peer doesn't cause a crash")
    {
        auto nearObject = std::make_shared<NearObject>();
        REQUIRE_NOTHROW(std::make_unique<NearObjectSession>(test::AllCapabilitiesSupported, std::vector<std::shared_ptr<NearObject>>{ nearObject }, callbacksNoop));
    }

    SECTION("creation with 1+ peers doesn't cause a crash")
    {
        const std::vector<std::shared_ptr<NearObject>> nearObjects{
            std::make_shared<NearObject>(),
            std::make_shared<NearObject>(),
            std::make_shared<NearObject>(),
        };

        REQUIRE_NOTHROW(std::make_unique<NearObjectSession>(test::AllCapabilitiesSupported, nearObjects, callbacksNoop));
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

TEST_CASE("near object session ranging state is correctly reflected", "[basic]")
{
    using namespace nearobject;

    const auto &capabilities = test::AllCapabilitiesSupported;

    SECTION("session indicates ranging is inactive upon construction")
    {
        const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(capabilities, {}, callbacksNoop);
        REQUIRE(!session.IsRangingActive());
    }

    SECTION("session indicates ranging is active after ranging is started")
    {
        const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(capabilities, {}, callbacksNoop);
        session.StartRanging();
        REQUIRE(session.IsRangingActive());
    }

    SECTION("session indicates ranging is active after ranging is started repeatedly")
    {
        const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(capabilities, {}, callbacksNoop);
        session.StartRanging();
        REQUIRE(session.IsRangingActive());
        session.StartRanging();
        REQUIRE(session.IsRangingActive());
    }

    SECTION("session indicates ranging is inactive after ranging is stopped")
    {
        const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(capabilities, {}, callbacksNoop);
        session.StartRanging();
        session.StopRanging();
        REQUIRE(!session.IsRangingActive());
    }

    SECTION("session indicates ranging is active after ranging is stopped repeatedly")
    {
        const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(capabilities, {}, callbacksNoop);
        session.StartRanging();
        session.StopRanging();
        REQUIRE(!session.IsRangingActive());
        session.StopRanging();
        REQUIRE(!session.IsRangingActive());
    }

    SECTION("session indicates ranging is active after starting subsequent ranging is started")
    {
        const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(capabilities, {}, callbacksNoop);
        session.StartRanging();
        session.StopRanging();
        session.StartRanging();
        REQUIRE(session.IsRangingActive());
    }

    SECTION("session indicates consistent ranging state after ranging is started")
    {
        const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(capabilities, {}, callbacksNoop);
        session.StartRanging();
        REQUIRE(session.IsRangingActive());
        REQUIRE(session.IsRangingActive());
    }

    SECTION("session indicates consistent ranging state after ranging is stoppted")
    {
        const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(capabilities, {}, callbacksNoop);
        session.StartRanging();
        session.StopRanging();
        REQUIRE(!session.IsRangingActive());
        REQUIRE(!session.IsRangingActive());
    }
}

TEST_CASE("near object event handlers can be registered", "[basic]")
{
    using namespace nearobject;

    SECTION("registering a handler doesn't cause a crash")
    {
        const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        REQUIRE_NOTHROW(std::make_unique<NearObjectSession>(test::AllCapabilitiesSupported, std::vector<std::shared_ptr<NearObject>>{}, callbacksNoop));
    }

    SECTION("starting and ending a ranging session doesn't cause a crash")
    {
        const auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(test::AllCapabilitiesSupported, std::vector<std::shared_ptr<NearObject>>{}, callbacksNoop);
        REQUIRE_NOTHROW([&]() {
            session.StartRanging();
            session.StopRanging();
        }());
    }

    SECTION("handler being deleted doesn't cause a crash")
    {
        auto callbacksNoop = std::make_shared<test::NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(test::AllCapabilitiesSupported, std::vector<std::shared_ptr<NearObject>>{}, callbacksNoop);
        session.StartRanging();
        callbacksNoop.reset();
        REQUIRE_NOTHROW(session.StopRanging());
    }

    SECTION("callbacks provide the session pointer for which it was registered")
    {
        struct NearObjectSessionEventCallbacksCheckSessionPointer final :
            public NearObjectSessionEventCallbacks
        {
            void
            OnSessionEnded(NearObjectSession *session) override
            {
                REQUIRE(Session == session);
            }

            void
            OnRangingStarted(NearObjectSession *session) override
            {
                REQUIRE(Session == session);
            }

            void
            OnRangingStopped(NearObjectSession *session) override
            {
                REQUIRE(Session == session);
            }

            void
            OnNearObjectPropertiesChanged(NearObjectSession *session, const std::vector<std::shared_ptr<NearObject>> /* nearObjectsChanged */) override
            {
                REQUIRE(Session == session);
            }

            void
            OnSessionMembershipChanged(NearObjectSession *session, const std::vector<std::shared_ptr<NearObject>> nearObjectsAdded, const std::vector<std::shared_ptr<NearObject>> nearObjectsRemoved) override
            {
                REQUIRE(Session == session);
            }

            NearObjectSession *Session{ nullptr };
        };

        const auto callbacks = std::make_shared<NearObjectSessionEventCallbacksCheckSessionPointer>();
        NearObjectSession session(test::AllCapabilitiesSupported, std::vector<std::shared_ptr<NearObject>>{}, callbacks);
        callbacks->Session = &session;
        session.StartRanging();
        session.StopRanging();
        // TODO: trigger NO property changes
        // TODO: trigger NO membership changes
    }
}
