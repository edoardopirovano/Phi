#include <phi/test/test_macros.hpp>

TEST_CASE("STATIC_REQUIRE_FALSE")
{
    STATIC_REQUIRE_FALSE(false);
    STATIC_REQUIRE_FALSE(0);
}
