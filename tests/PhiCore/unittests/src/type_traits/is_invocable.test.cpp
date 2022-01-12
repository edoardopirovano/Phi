#include <phi/test/test_macros.hpp>

#include <phi/core/nullptr_t.hpp>
#include <phi/type_traits/is_invocable.hpp>
#include <memory>
#include <vector>

struct Tag
{};

struct DerFromTag : Tag
{};

struct Implicit
{
    Implicit(int)
    {}
};

struct Explicit
{
    explicit Explicit(int)
    {}
};

struct NotCallableWithInt
{
    int operator()(int) = delete;
    int operator()(Tag)
    {
        return 42;
    }
};

struct Sink
{
    template <class... Args>
    void operator()(Args&&...) const
    {}
};

template <typename FunctionT, typename... ArgsT>
void test_is_invocable()
{
    STATIC_REQUIRE(phi::is_invocable<FunctionT, ArgsT...>::value);
    STATIC_REQUIRE_FALSE(phi::is_not_invocable<FunctionT, ArgsT...>::value);

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()
    STATIC_REQUIRE(phi::is_invocable_v<FunctionT, ArgsT...>);
    STATIC_REQUIRE_FALSE(phi::is_not_invocable_v<FunctionT, ArgsT...>);
#endif
}

template <typename FunctionT, typename... ArgsT>
void test_is_not_invocable()
{
    STATIC_REQUIRE_FALSE(phi::is_invocable<FunctionT, ArgsT...>::value);
    STATIC_REQUIRE(phi::is_not_invocable<FunctionT, ArgsT...>::value);

#if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()
    STATIC_REQUIRE_FALSE(phi::is_invocable_v<FunctionT, ArgsT...>);
    STATIC_REQUIRE(phi::is_not_invocable_v<FunctionT, ArgsT...>);
#endif
}

TEST_CASE("is_invocable")
{
    using AbominableFunc = void(...) const;

    //  Non-callable things
    {
        // is_invocable
        test_is_not_invocable<void>();
        test_is_not_invocable<const void>();
        test_is_not_invocable<volatile void>();
        test_is_not_invocable<const volatile void>();
        test_is_not_invocable<phi::nullptr_t>();
        test_is_not_invocable<short>();
        test_is_not_invocable<int>();
        test_is_not_invocable<float>();
        test_is_not_invocable<double>();

        test_is_not_invocable<int[]>();
        test_is_not_invocable<int[3]>();

        test_is_not_invocable<int*>();
        test_is_not_invocable<const int*>();
        test_is_not_invocable<volatile int*>();
        test_is_not_invocable<const volatile int*>();
        test_is_not_invocable<int const*>();
        test_is_not_invocable<int volatile*>();
        test_is_not_invocable<int const volatile*>();

        test_is_not_invocable<int**>();
        test_is_not_invocable<const int**>();
        test_is_not_invocable<volatile int**>();
        test_is_not_invocable<const volatile int**>();
        test_is_not_invocable<int const**>();
        test_is_not_invocable<int volatile**>();
        test_is_not_invocable<int const volatile**>();

        test_is_not_invocable<int&>();
        test_is_not_invocable<const int&>();
        test_is_not_invocable<volatile int&>();
        test_is_not_invocable<const volatile int&>();
        test_is_not_invocable<int&&>();
        test_is_not_invocable<const int&&>();
        test_is_not_invocable<volatile int&&>();
        test_is_not_invocable<const volatile int&&>();

        test_is_not_invocable<std::vector<int>>();
        test_is_not_invocable<std::vector<int*>>();
        test_is_not_invocable<std::vector<int**>>();

        test_is_not_invocable<AbominableFunc>();

        //  with parameters
        test_is_not_invocable<int, int>();
        test_is_not_invocable<int, double>();
        test_is_not_invocable<int, double, float>();
        test_is_not_invocable<int, char, float, double>();
        test_is_not_invocable<int, unsigned, short, double, float, float, double, signed char,
                              float, float, float, float, float, double, phi::nullptr_t,
                              AbominableFunc, AbominableFunc>();
        test_is_not_invocable<Sink, AbominableFunc>();
        test_is_not_invocable<Sink, void>();
        test_is_not_invocable<Sink, const void>();
        test_is_not_invocable<Sink, volatile void>();
        test_is_not_invocable<Sink, const volatile void>();
    }
    {
        using Fn  = int (Tag::*)(int);
        using RFn = int (Tag::*)(int)&&;
        // INVOKE bullet 1, 2 and 3
        {
            // Bullet 1
            test_is_invocable<Fn, Tag&, int>();
            test_is_invocable<Fn, DerFromTag&, int>();
            test_is_invocable<RFn, Tag&&, int>();
            test_is_not_invocable<RFn, Tag&, int>();
            test_is_not_invocable<Fn, Tag&>();
            test_is_not_invocable<Fn, Tag const&, int>();
        }
        {
            // Bullet 2
            using T  = std::reference_wrapper<Tag>;
            using DT = std::reference_wrapper<DerFromTag>;
            using CT = std::reference_wrapper<const Tag>;
            test_is_invocable<Fn, T&, int>();
            test_is_invocable<Fn, DT&, int>();
            test_is_invocable<Fn, const T&, int>();
            test_is_invocable<Fn, T&&, int>();
            test_is_not_invocable<Fn, CT&, int>();
            test_is_not_invocable<RFn, T, int>();
        }
        {
            // Bullet 3
            using T  = Tag*;
            using DT = DerFromTag*;
            using CT = const Tag*;
            using ST = std::unique_ptr<Tag>;
            test_is_invocable<Fn, T&, int>();
            test_is_invocable<Fn, DT&, int>();
            test_is_invocable<Fn, const T&, int>();
            test_is_invocable<Fn, T&&, int>();
            test_is_invocable<Fn, ST, int>();
            test_is_not_invocable<Fn, CT, int>();
            test_is_not_invocable<RFn, T, int>();
        }
    }
    {
        // Bullets 4, 5 and 6
        using Fn = int Tag::*;
        test_is_not_invocable<Fn>();
        {
            // Bullet 4
            test_is_invocable<Fn, Tag&>();
            test_is_invocable<Fn, DerFromTag&>();
            test_is_invocable<Fn, Tag&&>();
            test_is_invocable<Fn, Tag const&>();
        }
        {
            // Bullet 5
            using T  = std::reference_wrapper<Tag>;
            using DT = std::reference_wrapper<DerFromTag>;
            using CT = std::reference_wrapper<const Tag>;
            test_is_invocable<Fn, T&>();
            test_is_invocable<Fn, DT&>();
            test_is_invocable<Fn, const T&>();
            test_is_invocable<Fn, T&&>();
            test_is_invocable<Fn, CT&>();
        }
        {
            // Bullet 6
            using T  = Tag*;
            using DT = DerFromTag*;
            using CT = const Tag*;
            using ST = std::unique_ptr<Tag>;
            test_is_invocable<Fn, T&>();
            test_is_invocable<Fn, DT&>();
            test_is_invocable<Fn, const T&>();
            test_is_invocable<Fn, T&&>();
            test_is_invocable<Fn, ST>();
            test_is_invocable<Fn, CT&>();
        }
    }
    { // bullet 7
     {// Function pointer
      using Fp = void(*)(Tag&, int);
    test_is_invocable<Fp, Tag&, int>();
    test_is_invocable<Fp, DerFromTag&, int>();
    test_is_not_invocable<Fp, const Tag&, int>();
    test_is_not_invocable<Fp>();
    test_is_not_invocable<Fp, Tag&>();
}
{
    // Function reference
    using Fp = void (&)(Tag&, int);
    test_is_invocable<Fp, Tag&, int>();
    test_is_invocable<Fp, DerFromTag&, int>();
    test_is_not_invocable<Fp, const Tag&, int>();
    test_is_not_invocable<Fp>();
    test_is_not_invocable<Fp, Tag&>();
}
{
    // Function object
    using Fn = NotCallableWithInt;
    test_is_invocable<Fn, Tag>();
    test_is_not_invocable<Fn, int>();
}
}
{
    // Check for is_invocable_v
    using Fn = void (*)();
    test_is_invocable<Fn>();
    test_is_not_invocable<Fn, int>();
}
}
