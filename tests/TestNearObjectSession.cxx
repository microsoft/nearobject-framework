
#include <catch2/catch.hpp>
#include <memory>

#include <nearobject/NearObjectSession.hxx>
#include <nearobject/NearObjectSessionEventCallbacks.hxx>

TEST_CASE("event handlers can be registered", "[basic]")
{
    using namespace nearobject;

    struct NearObjectSessionEventCallbacksNoop :
        public NearObjectSessionEventCallbacks 
    {
        void
        OnNearObjectSessionEnded(NearObjectSession *) override
        {}

        void OnNearObjectRangingSessionStarted(NearObjectSession *) override
        {}

        void OnNearObjectRangingSessionEnded(NearObjectSession *) override
        {}
    };

    NearObjectSession session{};

    SECTION("registering a handler doesn't cause a crash")
    {
        const auto callbacksNoop = std::make_shared<NearObjectSessionEventCallbacksNoop>();
        session.RegisterCallbacks(callbacksNoop);
    }

    SECTION("starting and ending a ranging session cause a crash")
    {
        const auto callbacksNoop = std::make_shared<NearObjectSessionEventCallbacksNoop>();
        session.RegisterCallbacks(callbacksNoop);
        session.StartRangingSession();
        session.StopRangingSession();
    }

    SECTION("callbacks provide the session pointer for which it was registered")
    {
        struct NearObjectSessionEventCallbacksCheckSessionPointer :
            public NearObjectSessionEventCallbacks {
            NearObjectSessionEventCallbacksCheckSessionPointer(NearObjectSession *session) :
                Session(session)
            {}

            void
            OnNearObjectSessionEnded(NearObjectSession *session) override
            {
                CHECK(Session == session);
            }

            void OnNearObjectRangingSessionStarted(NearObjectSession *session) override
            {
                CHECK(Session == session);   
            }

            void OnNearObjectRangingSessionEnded(NearObjectSession *session) override
            {
                CHECK(Session == session);   
            }

            NearObjectSession *Session;
        };

        const auto callbacks = std::make_shared<NearObjectSessionEventCallbacksCheckSessionPointer>(&session);
        session.RegisterCallbacks(callbacks);
        session.StartRangingSession();
        session.StopRangingSession();
    }
}
