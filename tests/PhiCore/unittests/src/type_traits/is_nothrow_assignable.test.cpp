#include <phi/test/test_macros.hpp>

#include "test_types.hpp"
#include "type_traits_helper.hpp"
#include <phi/core/nullptr_t.hpp>
#include <phi/type_traits/is_assignable.hpp>
#include <phi/type_traits/is_nothrow_assignable.hpp>
#include <type_traits>

template <typename T, typename U>
void test_is_nothrow_assignable()
{
    STATIC_REQUIRE(phi::is_nothrow_assignable<T, U>::value);
    STATIC_REQUIRE_FALSE(phi::is_not_nothrow_assignable<T, U>::value);
    STATIC_REQUIRE(phi::is_assignable<T, U>::value);

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()
    STATIC_REQUIRE(phi::is_nothrow_assignable_v<T, U>);
    STATIC_REQUIRE_FALSE(phi::is_not_nothrow_assignable_v<T, U>);
    STATIC_REQUIRE(phi::is_assignable_v<T, U>);
#endif

    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_nothrow_assignable<T, U>);
    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_not_nothrow_assignable<T, U>);

    // Standard compatibility
    STATIC_REQUIRE(std::is_nothrow_assignable<T, U>::value);
    STATIC_REQUIRE(std::is_assignable<T, U>::value);
}

template <typename T, typename U>
void test_is_nothrow_assignable_cv_impl()
{
    test_is_nothrow_assignable<T, U>();
    test_is_nothrow_assignable<T, const U>();
    test_is_nothrow_assignable<T, volatile U>();
    test_is_nothrow_assignable<T, const volatile U>();
}

template <typename T, typename U>
void test_is_nothrow_assignable_cv()
{
    test_is_nothrow_assignable_cv_impl<T, U>();
    test_is_nothrow_assignable_cv_impl<const T, U>();
    test_is_nothrow_assignable_cv_impl<volatile T, U>();
    test_is_nothrow_assignable_cv_impl<const volatile T, U>();
}

template <typename T, typename U>
void test_is_not_nothrow_assignable()
{
    STATIC_REQUIRE_FALSE(phi::is_nothrow_assignable<T, U>::value);
    STATIC_REQUIRE(phi::is_not_nothrow_assignable<T, U>::value);

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()
    STATIC_REQUIRE_FALSE(phi::is_nothrow_assignable_v<T, U>);
    STATIC_REQUIRE(phi::is_not_nothrow_assignable_v<T, U>);
#endif

    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_nothrow_assignable<T, U>);
    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_not_nothrow_assignable<T, U>);

    // Standard compatibility
    STATIC_REQUIRE_FALSE(std::is_nothrow_assignable<T, U>::value);
}

template <typename T, typename U>
void test_is_not_nothrow_assignable_cv_impl()
{
    test_is_not_nothrow_assignable<T, U>();
    test_is_not_nothrow_assignable<T, const U>();
    test_is_not_nothrow_assignable<T, volatile U>();
    test_is_not_nothrow_assignable<T, const volatile U>();
}

template <typename T, typename U>
void test_is_not_nothrow_assignable_cv()
{
    test_is_not_nothrow_assignable_cv_impl<T, U>();
    test_is_not_nothrow_assignable_cv_impl<const T, U>();
    test_is_not_nothrow_assignable_cv_impl<volatile T, U>();
    test_is_not_nothrow_assignable_cv_impl<const volatile T, U>();
}

struct A
{};

struct B
{
    B& operator=(A);
};

struct C
{
    C& operator=(C&); // not const
};

struct D
{
    D& operator=(const D&) noexcept;
    D& operator=(int);
    D& operator=(double) noexcept;
};

struct E
{
    E& operator=(int) = delete;
};

TEST_CASE("is_nothrow_assignable")
{
    test_is_not_nothrow_assignable_cv<int, int>();
    test_is_not_nothrow_assignable_cv<int, int&>();
    test_is_not_nothrow_assignable_cv<int, int&&>();
    test_is_not_nothrow_assignable_cv<int, double>();
    test_is_not_nothrow_assignable_cv<int, double&>();
    test_is_not_nothrow_assignable_cv<int, double&&>();

    test_is_nothrow_assignable_cv<int&, int>();
    test_is_nothrow_assignable_cv<int&, int&>();
    test_is_nothrow_assignable_cv<int&, int&&>();
    test_is_nothrow_assignable_cv<int&, double>();
    test_is_nothrow_assignable_cv<int&, double&>();
    test_is_nothrow_assignable_cv<int&, double&&>();

    test_is_not_nothrow_assignable_cv<int&&, int>();
    test_is_not_nothrow_assignable_cv<int&&, int&>();
    test_is_not_nothrow_assignable_cv<int&&, int&&>();
    test_is_not_nothrow_assignable_cv<int&&, double>();
    test_is_not_nothrow_assignable_cv<int&&, double&>();
    test_is_not_nothrow_assignable_cv<int&&, double&&>();

    test_is_nothrow_assignable<D, D&>();
    test_is_nothrow_assignable<D&, D>();
    test_is_nothrow_assignable<D&, D&>();
    test_is_nothrow_assignable<D&, const D&>();
    test_is_nothrow_assignable<D&, D&&>();
    test_is_nothrow_assignable<D&, double>();
    test_is_nothrow_assignable<D&, double&>();
    test_is_nothrow_assignable<D&, double&&>();

    test_is_not_nothrow_assignable<const D, D>();
    test_is_not_nothrow_assignable<const D, D&>();
    test_is_not_nothrow_assignable<const D, const D&>();
    test_is_not_nothrow_assignable<const D, D&&>();
    test_is_not_nothrow_assignable<const D, const D&&>();

    test_is_not_nothrow_assignable_cv<B, A>();
    test_is_not_nothrow_assignable_cv<A, B>();
    test_is_not_nothrow_assignable_cv<C, C&>();
    // not noexcept
    test_is_not_nothrow_assignable_cv<D&, int>();
    test_is_not_nothrow_assignable_cv<D&, int&>();
    test_is_not_nothrow_assignable_cv<D&, int&&>();

    test_is_not_nothrow_assignable_cv<E, int>();
    test_is_not_nothrow_assignable_cv<E&, int>();
}
