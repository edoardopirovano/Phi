#include <phi/test/test_macros.hpp>

TEST_CASE("REQUIRE_FALSE")
{
    REQUIRE_FALSE(false);

    REQUIRE_FALSE(0);
}
