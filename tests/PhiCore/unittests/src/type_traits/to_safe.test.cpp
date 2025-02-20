#include <phi/test/test_macros.hpp>

#include "constexpr_helper.hpp"
#include <phi/core/boolean.hpp>
#include <phi/core/floating_point.hpp>
#include <phi/core/integer.hpp>
#include <phi/core/move.hpp>
#include <phi/type_traits/to_safe.hpp>

TEST_CASE("to_safe")
{
    // boolean
    STATIC_REQUIRE((phi::to_safe(true) == phi::boolean(true)));

    CONSTEXPR_RUNTIME bool a = true;
    STATIC_REQUIRE((phi::to_safe(a) == phi::boolean(true)));
    STATIC_REQUIRE((phi::to_safe(phi::move(a)) == phi::boolean(true)));

    // integer
    STATIC_REQUIRE((phi::to_safe(3) == phi::integer<int>(3)));

    CONSTEXPR_RUNTIME int b = 21;
    STATIC_REQUIRE((phi::to_safe(b) == phi::integer<int>(21)));
    STATIC_REQUIRE((phi::to_safe(phi::move(b)) == phi::integer<int>(21)));

    // Floating Point
    STATIC_REQUIRE((phi::to_safe(3.0) >= phi::floating_point<double>(3.0)));

    CONSTEXPR_RUNTIME float c = 3.0;
    STATIC_REQUIRE((phi::to_safe(c) >= phi::floating_point<double>(3.0)));
    STATIC_REQUIRE((phi::to_safe(phi::move(c)) >= phi::floating_point<double>(3.0)));
}
