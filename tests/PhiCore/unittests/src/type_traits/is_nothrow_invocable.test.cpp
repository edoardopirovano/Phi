#include <phi/test/test_macros.hpp>

#include <phi/compiler_support/warning.hpp>
#include <phi/core/nullptr_t.hpp>
#include <phi/generated/compiler_support/features.hpp>
#include <phi/type_traits/is_invocable.hpp>
#include <phi/type_traits/is_nothrow_invocable.hpp>
#include <vector>

struct Tag
{};

struct Implicit
{
    Implicit(int) noexcept
    {}
};

struct ThrowsImplicit
{
    ThrowsImplicit(int)
    {}
};

struct Explicit
{
    explicit Explicit(int) noexcept
    {}
};

template <bool IsNoexcept, typename Ret, typename... Args>
struct CallObject
{
    Ret operator()(Args&&...) const noexcept(IsNoexcept);
};

struct Sink
{
    template <typename... Args>
    void operator()(Args&&...) const noexcept
    {}
};

template <typename Fn, typename... Args>
constexpr bool throws_invocable()
{
    return phi::is_invocable<Fn, Args...>::value && !phi::is_nothrow_invocable<Fn, Args...>::value;
}

PHI_CLANG_SUPPRESS_WARNING_PUSH()
PHI_CLANG_SUPPRESS_WARNING("-Wunneeded-member-function")

// FIXME(EricWF) Don't test the where noexcept is *not* part of the type system
// once implementations have caught up.
void test_noexcept_function_pointers()
{
    struct Dummy
    {
        void foo() noexcept
        {}
        static void bar() noexcept
        {}
    };
#if !PHI_HAS_FEATURE_NOEXCEPT_FUNCTION_TYPE()
    {
        // Check that PMF's and function pointers *work*. is_nothrow_invocable will always
        // return false because 'noexcept' is not part of the function type.
        STATIC_REQUIRE(throws_invocable<decltype(&Dummy::foo), Dummy&>());
        STATIC_REQUIRE(throws_invocable<decltype(&Dummy::bar)>());
    }
#else
    {
        // Check that PMF's and function pointers actually work and that
        // is_nothrow_invocable returns true for noexcept PMF's and function
        // pointers.
        STATIC_REQUIRE(phi::is_nothrow_invocable<decltype(&Dummy::foo), Dummy&>::value);
        STATIC_REQUIRE(phi::is_nothrow_invocable<decltype(&Dummy::bar)>::value);
    }
#endif
}

PHI_CLANG_SUPPRESS_WARNING_POP()

template <typename FunctionT, typename... ArgsT>
void test_is_nothrow_invocable()
{
    STATIC_REQUIRE(phi::is_nothrow_invocable<FunctionT, ArgsT...>::value);
    STATIC_REQUIRE_FALSE(phi::is_not_nothrow_invocable<FunctionT, ArgsT...>::value);

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()
    STATIC_REQUIRE(phi::is_nothrow_invocable_v<FunctionT, ArgsT...>);
    STATIC_REQUIRE_FALSE(phi::is_not_nothrow_invocable_v<FunctionT, ArgsT...>);
#endif
}

template <typename FunctionT, typename... ArgsT>
void test_is_not_nothrow_invocable()
{
    STATIC_REQUIRE_FALSE(phi::is_nothrow_invocable<FunctionT, ArgsT...>::value);
    STATIC_REQUIRE(phi::is_not_nothrow_invocable<FunctionT, ArgsT...>::value);

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()
    STATIC_REQUIRE_FALSE(phi::is_nothrow_invocable_v<FunctionT, ArgsT...>);
    STATIC_REQUIRE(phi::is_not_nothrow_invocable_v<FunctionT, ArgsT...>);
#endif
}

TEST_CASE("is_nothrow_invocable")
{
#if PHI_HAS_FEATURE_NOEXCEPT_FUNCTION_TYPE()
    using AbominableFunc = void(...) const noexcept;
#else
    using AbominableFunc = void(...) const;
#endif
    //  Non-callable things
    {
        test_is_not_nothrow_invocable<void>();
        test_is_not_nothrow_invocable<const void>();
        test_is_not_nothrow_invocable<volatile void>();
        test_is_not_nothrow_invocable<const volatile void>();
        test_is_not_nothrow_invocable<phi::nullptr_t>();
        test_is_not_nothrow_invocable<int>();
        test_is_not_nothrow_invocable<double>();

        test_is_not_nothrow_invocable<int[]>();
        test_is_not_nothrow_invocable<int[3]>();

        test_is_not_nothrow_invocable<int*>();
        test_is_not_nothrow_invocable<const int*>();
        test_is_not_nothrow_invocable<int const*>();

        test_is_not_nothrow_invocable<int&>();
        test_is_not_nothrow_invocable<const int&>();
        test_is_not_nothrow_invocable<int&&>();

        test_is_not_nothrow_invocable<int, std::vector<int>>();
        test_is_not_nothrow_invocable<int, std::vector<int*>>();
        test_is_not_nothrow_invocable<int, std::vector<int**>>();

        test_is_not_nothrow_invocable<int, AbominableFunc>();

        //  with parameters
        test_is_not_nothrow_invocable<int, int>();
        test_is_not_nothrow_invocable<int, double, float>();
        test_is_not_nothrow_invocable<int, char, float, double>();
        test_is_not_nothrow_invocable<Sink, AbominableFunc>();
        test_is_not_nothrow_invocable<Sink, void>();
        test_is_not_nothrow_invocable<Sink, const void>();
        test_is_not_nothrow_invocable<Sink, volatile void>();
        test_is_not_nothrow_invocable<Sink, const volatile void>();
    }

    {
        // Check that the conversion to the return type is properly checked
        using Fn = CallObject<true, int>;
        test_is_not_nothrow_invocable<Fn(), Explicit>();
    }
    {
        // Check that the conversion to the parameters is properly checked
        using Fn = CallObject<true, void, const Implicit&, const ThrowsImplicit&>;
        test_is_nothrow_invocable<Fn, Implicit&, ThrowsImplicit&>();
        test_is_nothrow_invocable<Fn, int, ThrowsImplicit&>();
        STATIC_REQUIRE(throws_invocable<Fn, int, int>());
        test_is_not_nothrow_invocable<Fn>();
    }
    {
        // Check that the noexcept-ness of function objects is checked.
        using Fn  = CallObject<true, void>;
        using Fn2 = CallObject<false, void>;
        test_is_nothrow_invocable<Fn>();
        STATIC_REQUIRE(throws_invocable<Fn2>());
    }
    {
        // Check that PMD derefs are noexcept
        using Fn = int Tag::*;
        test_is_nothrow_invocable<Fn, Tag&>();
    }
    {
        // Check for is_nothrow_invocable_v
        using Fn = CallObject<true, int>;
        test_is_nothrow_invocable<Fn>();
        test_is_not_nothrow_invocable<Fn, int>();
    }

    test_noexcept_function_pointers();
}
