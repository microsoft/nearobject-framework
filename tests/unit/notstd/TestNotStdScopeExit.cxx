
#include <catch2/catch_test_macros.hpp>
#include <notstd/scope.hxx>

TEST_CASE("scope_exit creation doesn't cause a crash", "[notstd][shared][utility][scope]")
{
    REQUIRE_NOTHROW(notstd::scope_exit([]{}));
}

TEST_CASE("scope_exit apis don't cause a crash", "[notstd][shared][utility][scope]")
{
    SECTION("release() doesn't cause a crash")
    {
        REQUIRE_NOTHROW(notstd::scope_exit([]{}).release());
    }

    SECTION("reset() doesn't cause a crash")
    {
        REQUIRE_NOTHROW(notstd::scope_exit([]{}).reset());
    }

    SECTION("operator bool() doesn't cause a crash")
    {
        REQUIRE_NOTHROW(notstd::scope_exit([]{}) ? []{}() : []{}());
    }
}

TEST_CASE("scope_exit executes the exit function at the end of the owning scope", "[notstd][shared][utility][scope]")
{
    static constexpr auto ValueExpected = 100;

    int valueActual = 0;
    {
        auto onScopeExit = notstd::scope_exit([&valueActual]{
            valueActual = ValueExpected;
        });
    }

    // Containing scope is now destroyed, exit function should have run.
    REQUIRE(valueActual == ValueExpected);
}

TEST_CASE("scope_exit release() prevents executing exit function at the end of owning scope", "[notstd][shared][utility][scope]")
{
    static constexpr auto ValueExpected = 100;
    static constexpr auto ValueInitial = 0;

    int valueActual = ValueInitial;
    {
        auto onScopeExit = notstd::scope_exit([&valueActual]{
            valueActual = ValueExpected;
        });
        onScopeExit.release();
    }

    // Containing scope is now destroyed, exit function should have run.
    REQUIRE(valueActual == ValueInitial);
}
