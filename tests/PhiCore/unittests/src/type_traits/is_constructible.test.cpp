#include <phi/test/test_macros.hpp>

#include "test_types.hpp"
#include "type_traits_helper.hpp"
#include <phi/compiler_support/compiler.hpp>
#include <phi/type_traits/is_constructible.hpp>
#include <type_traits>

struct A
{
    explicit A(int);
    A(int, double);
    A(int, long, double);

private:
    A(char);
};

class AbstractFunc
{
    virtual ~AbstractFunc();
    virtual void foo() = 0;
};

class AbstractDestructor
{
    virtual ~AbstractDestructor() = 0;
};

struct PrivateDtor
{
    PrivateDtor(int)
    {}

private:
    ~PrivateDtor()
    {}
};

struct S
{
    template <typename T>
    explicit operator T() const;
};

template <typename T, typename... ArgsT>
void test_is_constructible()
{
#if PHI_HAS_WORKING_IS_CONSTRUCTIBLE()
    STATIC_REQUIRE(phi::is_constructible<T, ArgsT...>::value);
    STATIC_REQUIRE_FALSE(phi::is_not_constructible<T, ArgsT...>::value);

#    if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()
    STATIC_REQUIRE(phi::is_constructible_v<T, ArgsT...>);
    STATIC_REQUIRE_FALSE(phi::is_not_constructible_v<T, ArgsT...>);
#    endif

    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_constructible<T, ArgsT...>);
    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_not_constructible<T, ArgsT...>);

    // Standard compatibility
    STATIC_REQUIRE(std::is_constructible<T, ArgsT...>::value);
#endif
}

template <typename T, typename... ArgsT>
void test_is_not_constructible()
{
#if PHI_HAS_WORKING_IS_CONSTRUCTIBLE()
    STATIC_REQUIRE_FALSE(phi::is_constructible<T, ArgsT...>::value);
    STATIC_REQUIRE(phi::is_not_constructible<T, ArgsT...>::value);

#    if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()
    STATIC_REQUIRE_FALSE(phi::is_constructible_v<T, ArgsT...>);
    STATIC_REQUIRE(phi::is_not_constructible_v<T, ArgsT...>);
#    endif

    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_constructible<T, ArgsT...>);
    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_not_constructible<T, ArgsT...>);

    // Standard compatibility
    STATIC_REQUIRE_FALSE(std::is_constructible<T, ArgsT...>::value);
#endif
}

TEST_CASE("is_constructible")
{
    using B = base;
    using D = derived;

    test_is_constructible<int>();
    test_is_constructible<int, const int>();
    test_is_constructible<A, int>();
    test_is_constructible<A, int, double>();
    test_is_constructible<A, int, long, double>();
    test_is_constructible<int&, int&>();

    test_is_not_constructible<A>();
    test_is_not_constructible<A, char>();

    test_is_not_constructible<A, void>();
    test_is_not_constructible<int, void()>();
    test_is_not_constructible<int, void (&)()>();
    test_is_not_constructible<int, void() const>();
    test_is_not_constructible<int&, void>();
    test_is_not_constructible<int&, void()>();
    test_is_not_constructible<int&, void() const>();
    test_is_not_constructible<int&, void (&)()>();

    test_is_not_constructible<void>();
    test_is_not_constructible<const void>(); // LWG 2738
    test_is_not_constructible<volatile void>();
    test_is_not_constructible<const volatile void>();
    test_is_not_constructible<int&>();
    test_is_not_constructible<abstract>();
    test_is_not_constructible<AbstractFunc>();
    test_is_not_constructible<AbstractDestructor>();
    test_is_constructible<int, S>();
    test_is_not_constructible<int&, S>();

    test_is_constructible<void (&)(), void (&)()>();
    test_is_constructible<void (&)(), void()>();
    test_is_constructible<void(&&)(), void(&&)()>();
    test_is_constructible<void(&&)(), void()>();
    test_is_constructible<void(&&)(), void (&)()>();

    test_is_constructible<int const&, int>();
    test_is_constructible<int const&, int&&>();

    test_is_constructible<int&&, double&>();
    test_is_constructible<void (&)(), void(&&)()>();

    test_is_not_constructible<int&, int>();
    test_is_not_constructible<int&, int const&>();
    test_is_not_constructible<int&, int&&>();

    test_is_constructible<int&&, int>();
    test_is_constructible<int&&, int&&>();
    test_is_not_constructible<int&&, int&>();
    test_is_not_constructible<int&&, int const&&>();

    test_is_constructible<base, derived>();
    test_is_constructible<base&, derived&>();
    test_is_not_constructible<derived&, base&>();
    test_is_constructible<base const&, derived const&>();
#if PHI_COMPILER_IS_NOT(GCC)
    test_is_not_constructible<derived const&, base const&>();
    test_is_not_constructible<derived const&, base>();
#endif

    test_is_constructible<base&&, derived>();
    test_is_constructible<base&&, derived&&>();
#if PHI_COMPILER_IS_NOT(GCC)
    test_is_not_constructible<derived&&, base&&>();
    test_is_not_constructible<derived&&, base>();
#endif

    // test that T must also be destructible
    test_is_constructible<PrivateDtor&, PrivateDtor&>();
    test_is_not_constructible<PrivateDtor, int>();

    test_is_not_constructible<void() const, void() const>();
    test_is_not_constructible<void() const, void*>();

    test_is_constructible<int&, implicit_to<int&>>();
    test_is_constructible<const int&, implicit_to<int&&>>();
    test_is_constructible<int&&, implicit_to<int&&>>();
    test_is_constructible<const int&, implicit_to<int>>();

    test_is_not_constructible<B&&, B&>();
    test_is_not_constructible<B&&, D&>();
    test_is_constructible<B&&, implicit_to<D&&>>();
    test_is_constructible<B&&, implicit_to<D&&>&>();
    test_is_constructible<int&&, double&>();
    test_is_constructible<const int&, implicit_to<int&>&>();
    test_is_constructible<const int&, implicit_to<int&>>();
#if PHI_SUPPORTS_IS_CONSTRUCTIBLE()
    test_is_constructible<const int&, explicit_to<int&>&>();
    test_is_constructible<const int&, explicit_to<int&>>();
#else
    test_is_not_constructible<const int&, explicit_to<int&>&>();
    test_is_not_constructible<const int&, explicit_to<int&>>();
#endif

#if PHI_SUPPORTS_IS_CONSTRUCTIBLE()
    test_is_constructible<const int&, explicit_to<int&>>();
    test_is_constructible<const int&, explicit_to<int&>&>();
#else
    test_is_not_constructible<const int&, explicit_to<int&>>();
    test_is_not_constructible<const int&, explicit_to<int&>&>();
#endif

// Binding through reference-compatible type is required to perform
// direct-initialization as described in [over.match.ref] p. 1 b. 1:
//
// But the rvalue to lvalue reference binding isn't allowed according to
// [over.match.ref] despite Clang accepting it.
#if PHI_COMPILER_IS(CLANG)
    test_is_constructible<int&, explicit_to<int&>>();
#endif
#if PHI_COMPILER_IS_NOT(GCC) && PHI_COMPILER_IS_NOT(MSVC)
    test_is_constructible<const int&, explicit_to<int&&>>();
#else
    test_is_not_constructible<const int&, explicit_to<int&&>>();
#endif

#if PHI_SUPPORTS_IS_CONSTRUCTIBLE()
    test_is_constructible<int&&, explicit_to<int&&>>();
#else
    test_is_not_constructible<int&&, explicit_to<int&&>>();
#endif

#if PHI_COMPILER_IS(CLANG_COMPAT)
    // FIXME Clang and GCC disagree on the validity of this expression.
    test_is_constructible<const int&, explicit_to<int>>();
    test_is_constructible<int&&, explicit_to<int>>();
#else
    test_is_not_constructible<const int&, explicit_to<int>>();
    test_is_not_constructible<int&&, explicit_to<int>>();
#endif

    // Binding through temporary behaves like copy-initialization,
    // see [dcl.init.ref] p. 5, very last sub-bullet:
    test_is_not_constructible<const int&, explicit_to<double&&>>();
    test_is_not_constructible<int&&, explicit_to<double&&>>();

    test_is_not_constructible<void()>();
    test_is_not_constructible<void() const>();
    test_is_not_constructible<void() volatile>();
    test_is_not_constructible<void()&>();
    test_is_not_constructible<void() &&>();

#if !PHI_HAS_BUG_GCC_102305()
    // GCC bug 102305 (https://gcc.gnu.org/bugzilla/show_bug.cgi?id=102305)
    test_is_not_constructible<abstract_template<int>>();
#endif
}
