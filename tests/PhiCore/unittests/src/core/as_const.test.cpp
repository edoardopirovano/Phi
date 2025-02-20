#include <phi/test/test_macros.hpp>

#include <phi/compiler_support/extended_attributes.hpp>
#include <phi/compiler_support/warning.hpp>
#include <phi/core/as_const.hpp>
#include <phi/type_traits/is_const.hpp>
#include <phi/type_traits/remove_reference.hpp>

PHI_CLANG_SUPPRESS_WARNING_PUSH()
PHI_CLANG_SUPPRESS_WARNING("-Wfloat-equal")

PHI_GCC_SUPPRESS_WARNING_PUSH()
PHI_GCC_SUPPRESS_WARNING("-Wfloat-equal")

struct S
{
    int i;
};

PHI_ATTRIBUTE_PURE bool operator==(const S& x, const S& y)
{
    return x.i == y.i;
}

bool operator==(const volatile S& x, const volatile S& y)
{
    return x.i == y.i;
}

template <typename T>
void test_as_const(T& t)
{
    STATIC_REQUIRE(
            phi::is_const<typename phi::remove_reference<decltype(phi::as_const(t))>::type>::value);
    STATIC_REQUIRE(phi::is_const<
                   typename phi::remove_reference<decltype(phi::as_const<T>(t))>::type>::value);
    STATIC_REQUIRE(phi::is_const<typename phi::remove_reference<decltype(phi::as_const<const T>(
                           t))>::type>::value);
    STATIC_REQUIRE(phi::is_const<typename phi::remove_reference<decltype(phi::as_const<volatile T>(
                           t))>::type>::value);
    STATIC_REQUIRE(phi::is_const<typename phi::remove_reference<
                           decltype(phi::as_const<const volatile T>(t))>::type>::value);

    CHECK(phi::as_const(t) == t);
    CHECK(phi::as_const<T>(t) == t);
    CHECK(phi::as_const<const T>(t) == t);
    CHECK(phi::as_const<volatile T>(t) == t);
    CHECK(phi::as_const<const volatile T>(t) == t);

    CHECK_NOEXCEPT(phi::as_const(t));
    CHECK_NOEXCEPT(phi::as_const<T>(t));
    CHECK_NOEXCEPT(phi::as_const<const T>(t));
    CHECK_NOEXCEPT(phi::as_const<volatile T>(t));
    CHECK_NOEXCEPT(phi::as_const<const volatile T>(t));
}

TEST_CASE("as_const")
{
    bool   b = true;
    char   c = 'a';
    int    i = 3;
    double d = 4.0;
    S      s{2};

    test_as_const(b);
    test_as_const(c);
    test_as_const(i);
    test_as_const(d);
    test_as_const(s);
}

PHI_GCC_SUPPRESS_WARNING_POP()
PHI_CLANG_SUPPRESS_WARNING_POP()
