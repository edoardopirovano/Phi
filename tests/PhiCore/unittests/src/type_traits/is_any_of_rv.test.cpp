#include <phi/test/test_macros.hpp>

#include "type_traits_helper.hpp"
#include <phi/type_traits/is_any_of_rv.hpp>

template <typename T, typename... Args>
void test_is_any_of_rv()
{
    STATIC_REQUIRE(phi::is_any_of_rv<T, Args...>::value);

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()
    STATIC_REQUIRE(phi::is_any_of_rv_v<T, Args...>);
#endif

    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_any_of_rv<T, Args...>);
}

template <typename T, typename... Args>
void test_is_not_any_of_rv()
{
    STATIC_REQUIRE_FALSE(phi::is_any_of_rv<T, Args...>::value);

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()
    STATIC_REQUIRE_FALSE(phi::is_any_of_rv_v<T, Args...>);
#endif

    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_any_of_rv<T, Args...>);
}

struct A
{};

TEST_CASE("is_any_of_rv")
{
    test_is_any_of_rv<int, int>();
    test_is_any_of_rv<int, unsigned, int>();
    test_is_any_of_rv<int, float, unsigned, int>();
    test_is_any_of_rv<int, A, float, unsigned, int>();

    test_is_any_of_rv<int, int, void>();
    test_is_any_of_rv<int, void, int>();
    test_is_any_of_rv<int, int, void, void>();
    test_is_any_of_rv<int, void, int, void>();
    test_is_any_of_rv<int, void, void, int>();
    test_is_any_of_rv<int, int, void, void, void>();
    test_is_any_of_rv<int, void, int, void, void>();
    test_is_any_of_rv<int, void, void, int, void>();
    test_is_any_of_rv<int, void, void, void, int>();

    test_is_not_any_of_rv<int, void>();
    test_is_not_any_of_rv<int, void, void>();
    test_is_not_any_of_rv<int, void, void, void>();

    // volatile
    test_is_any_of_rv<int, volatile int>();

    // const, ref
    test_is_not_any_of_rv<int, const int, const volatile int, int&, int&&, const int&, const int&&,
                          volatile int&, volatile int&&, const volatile int&,
                          const volatile int&&>();
}
