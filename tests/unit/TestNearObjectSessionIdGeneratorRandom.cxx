
#include <iostream> // TODO: remove me
#include <memory>
#include <unordered_set>

#include <catch2/catch_test_macros.hpp>

#include <nearobject/service/NearObjectSessionIdGenerator.hxx>
#include <nearobject/service/NearObjectSessionIdGeneratorRandom.hxx>

namespace nearobject::service::test
{
    static constexpr auto NumSessionIdsToGenerate = 10'000;
} // namespace nearobject::service::test

TEST_CASE("session id generator can be created and destroyed")
{
    using namespace nearobject::service;

    SECTION("creation doesn't cause a crash")
    {
        REQUIRE_NOTHROW(std::make_unique<NearObjectSessionIdGeneratorRandom>());
    }

    const auto verifyNoThrowOnDestruction = [](std::unique_ptr<NearObjectSessionIdGenerator> generator)
    {
        REQUIRE_NOTHROW([&]() {
            generator.reset();
        }());
    };

    SECTION("destruction following creation doesn't cause a crash")
    {
        auto generator = std::make_unique<NearObjectSessionIdGeneratorRandom>();
        verifyNoThrowOnDestruction(std::move(generator));
    }

    SECTION("destruction following use doesn't cause a crash")
    {
        uint32_t sessionId = 0;
        auto generator = std::make_unique<NearObjectSessionIdGeneratorRandom>();
        for (auto i = 0; i < test::NumSessionIdsToGenerate; i++) {
            sessionId = generator->GetNext();
        }
        verifyNoThrowOnDestruction(std::move(generator));
    }
}

TEST_CASE("session identifiers can be generated", "[basic]")
{
    using namespace nearobject::service;

    SECTION("sequence of session ids can be generated")
    {
        uint32_t sessionId = 0;
        NearObjectSessionIdGeneratorRandom generator{};

        for (auto i = 0; i < test::NumSessionIdsToGenerate; i++) {
            REQUIRE_NOTHROW(sessionId = generator.GetNext());
        }
    }

    SECTION("session of session ids has low repeat ratio")
    {
        // The impl uses std::uniform_int_distribution<uint32_t> which
        // uniformly distributes the generated values according to the discrete
        // probability function P(i|2^32) = 1/2^32. Thus, using 5 here per
        // 10,000 iterations should virtually assure that if implemented
        // correctly, the test should always pass. While unlikely, it is
        // possible to get a false positive, so such incidences should be
        // ignored.
        static constexpr auto NumSessionIdsDuplicatedAllowed = 5;

        std::size_t numSessionIdsDuplicated = 0;
        std::unordered_set<uint32_t> sessionIds{};
        NearObjectSessionIdGeneratorRandom generator{};

        for (auto i = 0; i < test::NumSessionIdsToGenerate; i++) {
            const auto sessionId = generator.GetNext();
            const auto [_, sessionIdWasUnique] = sessionIds.insert(sessionId);
            if (!sessionIdWasUnique) {
                REQUIRE(++numSessionIdsDuplicated < NumSessionIdsDuplicatedAllowed);
            }
        }
    }
}
