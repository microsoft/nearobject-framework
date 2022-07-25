
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
    };

    NearObjectSession session{};
    const auto callbacks = std::make_shared<NearObjectSessionEventCallbacksNoop>();

    SECTION("registering a handler doesn't cause a crash")
    {
        session.RegisterCallbacks(callbacks);
    }

    SECTION("callbacks provide the session pointer for which it was registered")
    {
        struct NearObjectSessionEventCallbacksCheckSessionPointer :
            public NearObjectSessionEventCallbacks {
            NearObjectSessionEventCallbacksCheckSessionPointer(NearObjectSession *session) :
                Session(session)
            { }

            void
            OnNearObjectSessionEnded(NearObjectSession *session) override
            {
                CHECK(Session == session);
            }

            NearObjectSession *Session;
        };

        const auto callbacks = std::make_shared<NearObjectSessionEventCallbacksCheckSessionPointer>(&session);
        session.RegisterCallbacks(callbacks);
    }
}
