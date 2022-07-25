
#include <catch2/catch.hpp>
#include <memory>

#include <nearobject/NearObjectSession.hxx>
#include <nearobject/NearObjectSessionEventCallbacks.hxx>

TEST_CASE("Handlers can be registered", "[basic]")
{
    using namespace nearobject;

    struct NearObjectSessionEventCallbacksForTest :
        public NearObjectSessionEventCallbacks
    {
        void
        OnNearObjectSessionEnded(NearObjectSession *session) override
        {
            // TODO
        }
    };

    NearObjectSession session{};
    auto callbacks = std::make_shared<NearObjectSessionEventCallbacksForTest>();

    SECTION("registering a handler doesn't cause a crash") {
        session.RegisterCallbacks(callbacks);
    }
}
