
#include <catch2/catch.hpp>
#include <memory>

#include <nearobject/NearObjectSession.hxx>
#include <nearobject/NearObjectSessionEventCallbacks.hxx>

static constexpr nearobject::NearObjectCapabilities AllCapabilitiesSupported = {
    true, // SupportsRanging
    true, // SupportsPositioning
    true, // SupportsSecureDevice
    true, // SupportsSecureChannels
};

struct NearObjectSessionEventCallbacksNoop :
    public nearobject::NearObjectSessionEventCallbacks {
    void
    OnNearObjectSessionEnded(nearobject::NearObjectSession *) override
    { }

    void
    OnNearObjectRangingSessionStarted(nearobject::NearObjectSession *) override
    { }

    void
    OnNearObjectRangingSessionEnded(nearobject::NearObjectSession *) override
    { }

    void
    OnNearObjectSessionNearObjectPropertiesChanged(nearobject::NearObjectSession *session, const std::vector<std::shared_ptr<nearobject::NearObject>> nearObjectsChanged) override
    { }

    void
    OnNearObjectSessionMembershipChanged(nearobject::NearObjectSession *session, const std::vector<std::shared_ptr<nearobject::NearObject>> nearObjectsAdded, const std::vector<std::shared_ptr<nearobject::NearObject>> nearObjectsRemoved) override
    { }
};

TEST_CASE("session capabilities are accurate", "[basic]")
{
    using namespace nearobject;

    SECTION("capabilities match post-creation")
    {
        const auto callbacksNoop = std::make_shared<NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(AllCapabilitiesSupported, {}, callbacksNoop);
        REQUIRE(session.Capabilities == AllCapabilitiesSupported);
    }
}

TEST_CASE("event handlers can be registered", "[basic]")
{
    using namespace nearobject;

    SECTION("registering a handler doesn't cause a crash")
    {
        const auto callbacksNoop = std::make_shared<NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(AllCapabilitiesSupported, {}, callbacksNoop);
    }

    SECTION("starting and ending a ranging session doesn't cause a crash")
    {
        const auto callbacksNoop = std::make_shared<NearObjectSessionEventCallbacksNoop>();
        NearObjectSession session(AllCapabilitiesSupported, {}, callbacksNoop);
        session.StartRangingSession();
        session.StopRangingSession();
    }

    SECTION("callbacks provide the session pointer for which it was registered")
    {
        struct NearObjectSessionEventCallbacksCheckSessionPointer :
            public NearObjectSessionEventCallbacks {
            void
            OnNearObjectSessionEnded(NearObjectSession *session) override
            {
                CHECK(Session == session);
            }

            void
            OnNearObjectRangingSessionStarted(NearObjectSession *session) override
            {
                CHECK(Session == session);
            }

            void
            OnNearObjectRangingSessionEnded(NearObjectSession *session) override
            {
                CHECK(Session == session);
            }

            void
            OnNearObjectSessionNearObjectPropertiesChanged(NearObjectSession *session, const std::vector<std::shared_ptr<NearObject>> /* nearObjectsChanged */) override
            {
                CHECK(Session == session);
            }

            void
            OnNearObjectSessionMembershipChanged(NearObjectSession *session, const std::vector<std::shared_ptr<NearObject>> nearObjectsAdded, const std::vector<std::shared_ptr<NearObject>> nearObjectsRemoved) override
            {
                CHECK(Session == session);
            }

            NearObjectSession *Session{ nullptr };
        };

        const auto callbacks = std::make_shared<NearObjectSessionEventCallbacksCheckSessionPointer>();
        NearObjectSession session(AllCapabilitiesSupported, {}, callbacks);
        callbacks->Session = &session;
        session.StartRangingSession();
        session.StopRangingSession();
        // TODO: trigger NO property changes
        // TODO: trigger NO membership changes
    }
}
