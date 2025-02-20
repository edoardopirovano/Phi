#include <phi/test/test_macros.hpp>

#include <phi/core/declval.hpp>
#include <phi/type_traits/common_type.hpp>
#include <phi/type_traits/decay.hpp>
#include <phi/type_traits/is_same.hpp>
#include <phi/type_traits/void_t.hpp>
#include <type_traits>

template <typename T, typename Expected>
void test_common_type()
{
    CHECK_SAME_TYPE(typename phi::common_type<T>::type, Expected);
    CHECK_SAME_TYPE(phi::common_type_t<T>, Expected);

    // Standard compatibility
    CHECK_SAME_TYPE(typename phi::common_type<T>::type, typename std::common_type<T>::type);
    CHECK_SAME_TYPE(typename std::common_type<T>::type, Expected);
}

template <typename T1, typename T2, typename Expected>
void test_common_type_impl()
{
    CHECK_SAME_TYPE(typename phi::common_type<T1, T2>::type, Expected);
    CHECK_SAME_TYPE(phi::common_type_t<T1, T2>, Expected);

    // Standard compatibility
    CHECK_SAME_TYPE(typename phi::common_type<T1, T2>::type,
                    typename std::common_type<T1, T2>::type);
    CHECK_SAME_TYPE(typename std::common_type<T1, T2>::type, Expected);
}

template <typename T1, typename T2, typename Expected>
void test_common_type_no_std_impl()
{
    CHECK_SAME_TYPE(typename phi::common_type<T1, T2>::type, Expected);
    CHECK_SAME_TYPE(phi::common_type_t<T1, T2>, Expected);
}

template <typename T1, typename T2, typename Expected>
void test_common_type()
{
    test_common_type_impl<T1, T2, Expected>();
    test_common_type_impl<T2, T1, Expected>();
}

template <typename T1, typename T2, typename Expected>
void test_common_type_new()
{
#if (PHI_COMPILER_IS_ATLEAST(CLANG, 10, 0, 0) || PHI_COMPILER_IS_ATLEAST(GCC, 10, 0, 0)) &&        \
        PHI_CPP_STANDARD_IS_ATLEAST(20)
    test_common_type_impl<T1, T2, Expected>();
    test_common_type_impl<T2, T1, Expected>();
#else
    test_common_type_no_std_impl<T1, T2, Expected>();
    test_common_type_no_std_impl<T2, T1, Expected>();
#endif
}

template <typename T1, typename T2, typename Expected>
void test_common_type_cv()
{
    test_common_type<T1, T2, Expected>();
    test_common_type<T1, const T2, Expected>();
    test_common_type<T1, volatile T2, Expected>();
    test_common_type<T1, const volatile T2, Expected>();

    test_common_type<const T1, T2, Expected>();
    test_common_type<const T1, const T2, Expected>();
    test_common_type<const T1, volatile T2, Expected>();
    test_common_type<const T1, const volatile T2, Expected>();

    test_common_type<volatile T1, T2, Expected>();
    test_common_type<volatile T1, const T2, Expected>();
    test_common_type<volatile T1, volatile T2, Expected>();
    test_common_type<volatile T1, const volatile T2, Expected>();

    test_common_type<const volatile T1, T2, Expected>();
    test_common_type<const volatile T1, const T2, Expected>();
    test_common_type<const volatile T1, volatile T2, Expected>();
    test_common_type<const volatile T1, const volatile T2, Expected>();
}

template <typename T1, typename T2, typename T3, typename Expected>
void test_common_type_impl()
{
    CHECK_SAME_TYPE(typename phi::common_type<T1, T2, T3>::type, Expected);
    CHECK_SAME_TYPE(phi::common_type_t<T1, T2, T3>, Expected);

    // Standard compatibility
    CHECK_SAME_TYPE(typename phi::common_type<T1, T2, T3>::type,
                    typename std::common_type<T1, T2, T3>::type);
    CHECK_SAME_TYPE(typename std::common_type<T1, T2, T3>::type, Expected);
}

template <typename T1, typename T2, typename T3, typename Expected>
void test_common_type()
{
    test_common_type_impl<T1, T2, T3, Expected>();
    test_common_type_impl<T1, T3, T2, Expected>();
    test_common_type_impl<T2, T1, T3, Expected>();
    test_common_type_impl<T2, T3, T1, Expected>();
    test_common_type_impl<T3, T1, T2, Expected>();
    test_common_type_impl<T3, T2, T1, Expected>();
}

#if PHI_CPP_STANDARD_IS_BELOW(11)
template <typename Trait, typename = void>
struct no_common_type_imp : public phi::true_type
{};

template <typename Trait>
struct no_common_type_imp<Trait, typename phi::void_t<typename Trait::type>::type>
    : public phi::false_type
{};

struct NoArgTag;

template <typename Tp = NoArgTag, typename Up = NoArgTag, typename Vp = NoArgTag>
struct no_common_type : no_common_type_imp<phi::common_type<Tp, Up, Vp>>
{};

template <typename Tp, typename Up>
struct no_common_type<Tp, Up> : public no_common_type_imp<phi::common_type<Tp, Up>>
{};

template <typename Tp>
struct no_common_type<Tp> : public no_common_type_imp<phi::common_type<Tp>>
{};

template <>
struct no_common_type<> : public no_common_type_imp<phi::common_type<>>
{};

#else

template <typename Tp>
struct always_bool_imp
{
    using type = bool;
};

template <typename Tp>
using always_bool = typename always_bool_imp<Tp>::type;

template <typename... Args>
constexpr auto no_common_type_imp(int) -> always_bool<typename phi::common_type<Args...>::type>
{
    return false;
}

template <typename... Args>
constexpr bool no_common_type_imp(long)
{
    return true;
}

template <typename... Args>
using no_common_type = phi::bool_constant<no_common_type_imp<Args...>(0)>;
#endif

template <typename... Ts>
void test_no_common_type()
{
    STATIC_REQUIRE(no_common_type<Ts...>::value);
}

struct E
{};

template <typename T>
struct X
{
    explicit X(T const&)
    {}
};

template <typename T>
struct S
{
    explicit S(T const&)
    {}
};

template <typename T>
struct bad_reference_wrapper
{
    bad_reference_wrapper(T&);
    bad_reference_wrapper(T&&) = delete;
    operator T&() const;
};

namespace phi
{
    template <typename T>
    struct common_type<T, ::S<T>>
    {
        using type = S<T>;
    };

    template <typename T>
    struct common_type<::S<T>, T>
    {
        using type = S<T>;
    };

    //  P0548
    template <typename T>
    struct common_type<::S<T>, ::S<T>>
    {
        using type = S<T>;
    };

    template <>
    struct common_type<::S<long>, long>
    {};

    template <>
    struct common_type<long, ::S<long>>
    {};

    template <>
    struct common_type<::X<double>, ::X<double>>
    {};
} // namespace phi

template <typename T1, typename T2>
struct TernaryOp
{
    static_assert((phi::is_same<typename phi::decay<T1>::type, T1>::value), "must be same");
    static_assert((phi::is_same<typename phi::decay<T2>::type, T2>::value), "must be same");
    using type =
            typename phi::decay<decltype(false ? phi::declval<T1>() : phi::declval<T2>())>::type;
};

// (4.1)
// -- If sizeof...(T) is zero, there shall be no member type.
void test_bullet_one()
{
    test_no_common_type<>();
}

// (4.2)
// -- If sizeof...(T) is one, let T0 denote the sole type constituting the pack
//    T. The member typedef-name type shall denote the same type, if any, as
//    common_type_t<T0, T0>; otherwise there shall be no member type.
void test_bullet_two()
{
    //test_common_type<void, void>();
    //test_common_type<int, int>();
    //test_common_type<const int, int>();
    //test_common_type<int volatile[], int volatile*>();
    //test_common_type<void (&)(), void (*)()>();

    test_no_common_type<X<double>>();
}

template <typename T, typename U, typename Expect>
void test_bullet_three_one_imp()
{
    using DT = typename phi::decay<T>::type;
    using DU = typename phi::decay<U>::type;
    STATIC_REQUIRE(phi::is_not_same<T, DT>::value || phi::is_not_same<U, DU>::value);

    /*
    test_common_type<T, U, Expect>();
    test_common_type<U, T, Expect>();
    test_common_type<T, U, typename phi::common_type<DT, DU>::type>();
    test_common_type<T, U, phi::common_type_t<DT, DU>>();
    */
}

// (4.3)
// -- If sizeof...(T) is two, let the first and second types constituting T be
//    denoted by T1 and T2, respectively, and let D1 and D2 denote the same types
//    as decay_t<T1> and decay_t<T2>, respectively.
// (4.3.1)
//    -- If is_same_v<T1, D1> is false or is_same_v<T2, D2> is false, let C
//       denote the same type, if any, as common_type_t<D1, D2>.
void test_bullet_three_one()
{
    // Test that the user provided specialization of common_type is used after
    // decaying T1.
    {
        using T1 = const S<int>;
        using T2 = int;
        test_bullet_three_one_imp<T1, T2, S<int>>();
    }
    // Test a user provided specialization that does not provide a typedef.
    {
        using T1 = const ::S<long>;
        using T2 = long;
        test_no_common_type<T1, T2>();
        test_no_common_type<T2, T1>();
    }
    // Test that the ternary operator is not applied when the types are the
    // same.
    {
            //using T1     = const void;
            //using Expect = void;
            //test_common_type<T1, T1, Expect>();
            //test_common_type<T1, T1, typename phi::common_type<T1>::type>();
            //test_common_type<T1, T1, phi::common_type_t<T1>>();
    } {
        //using T1     = int const[];
        //using Expect = const int*;
        //test_common_type<T1, T1, Expect>();
        //test_common_type<T1, T1, typename phi::common_type<T1>::type>();
        //test_common_type<T1, T1, phi::common_type_t<T1>>();
    }
}

// (4.3)
// -- If sizeof...(T) is two, let the first and second types constituting T be
//    denoted by T1 and T2, respectively, and let D1 and D2 denote the same types
//    as decay_t<T1> and decay_t<T2>, respectively.
// (4.3.1)
//    -- If [...]
// (4.3.2)
//    -- [Note: [...]
// (4.3.3)
//    -- Otherwise, if
//       decay_t<decltype(false ? declval<D1>() : declval<D2>())>
//       denotes a type, let C denote that type.
void test_bullet_three_three()
{
    {
            //using T1     = const int*;
            //using T2     = int*;
            //using Expect = TernaryOp<T1, T2>::type;
            //test_common_type<T1, T2, Expect>();
            //test_common_type<T2, T1, Expect>();
    } // Test that there is no ::type member when the ternary op is ill-formed
    {
        using T1 = int;
        using T2 = void;
        test_no_common_type<T1, T2>();
        test_no_common_type<T2, T1>();
    }
    {
        using T1 = int;
        using T2 = X<int>;
        test_no_common_type<T1, T2>();
        test_no_common_type<T2, T1>();
    }
    // Test that the ternary operator is not applied when the types are the
    // same.
    {
        //using T1     = void;
        //using Expect = void;
        //test_common_type<T1, T1, Expect>();
        //test_common_type<T1, T1, typename phi::common_type<T1>::type>();
        //test_common_type<T1, T1, phi::common_type_t<T1>>();
    }
}

// (4.3)
// -- If sizeof...(T) is two, let the first and second types constituting T be
//    denoted by T1 and T2, respectively, and let D1 and D2 denote the same types
//    as decay_t<T1> and decay_t<T2>, respectively.
// (4.3.1)
//    -- If [...]
// (4.3.2)
//    -- [Note: [...]
// (4.3.3)
//    -- Otherwise
// (4.3.4)
//    -- Otherwise, if COND-RES(CREF(D1), CREF(D2)) denotes a type, let C
//       denote the type decay_t<COND-RES(CREF(D1), CREF(D2))>.
void test_bullet_three_four()
{
    test_common_type_new<int, bad_reference_wrapper<int>, int>();
    test_common_type_new<bad_reference_wrapper<double>, double, double>();
    test_common_type_new<const bad_reference_wrapper<double>, double, double>();
    test_common_type_new<volatile bad_reference_wrapper<double>, double, double>();
    test_common_type_new<const volatile bad_reference_wrapper<double>, double, double>();

    test_common_type_new<bad_reference_wrapper<double>, const double, double>();
    test_common_type_new<bad_reference_wrapper<double>, volatile double, double>();
    test_common_type_new<bad_reference_wrapper<double>, const volatile double, double>();

    test_common_type_new<bad_reference_wrapper<double>&, double, double>();
    test_common_type_new<bad_reference_wrapper<double>, double&, double>();
}

// (4.4)
// -- If sizeof...(T) is greater than two, let T1, T2, and R, respectively,
// denote the first, second, and (pack of) remaining types constituting T.
// Let C denote the same type, if any, as common_type_t<T1, T2>. If there is
// such a type C, the member typedef-name type shall denote the
// same type, if any, as common_type_t<C, R...>. Otherwise, there shall be
// no member type.
void test_bullet_four()
{
    {
        // test that there is no ::type member
        test_no_common_type<int, E>();
        test_no_common_type<int, int, E>();
        test_no_common_type<int, int, E, int>();
        test_no_common_type<int, int, int, E>();
    }
}

#if 0
struct A
{};

struct B
{};

struct C : B
{};

template <>
struct phi::common_type<A, phi::tuple<B>>
{
    using type = tuple<B>;
};
#endif

TEST_CASE("common_type")
{
    //test_common_type<int, int>();
    //test_common_type<char, char>();

    //test_common_type<const int, int>();
    //test_common_type<volatile int, int>();
    //test_common_type<const volatile int, int>();

    //test_common_type<int, int, int>();
    //test_common_type<int, const int, int>();

    //test_common_type_cv<long, int, long>();

    //test_common_type<double, char, double>();
    //test_common_type<short, char, int>();

    //test_common_type<double, char, long long, double>();
    //test_common_type<unsigned, char, long long, long long>();

    //test_common_type<void, void>();
    //test_common_type<const void, void>();
    //test_common_type<volatile void, void>();
    //test_common_type<const volatile void, void>();

    //test_common_type_cv<void, void, void>();

#if 0
    test_common_type<int, S<int>, S<int>>();
    test_common_type<int, S<int>, S<int>, S<int>>();
    test_common_type<int, int, S<int>, S<int>>();
#endif

    test_bullet_one();
    test_bullet_two();
    test_bullet_three_one();
    test_bullet_three_three();
    test_bullet_three_four();
    test_bullet_four();

    // P0548
#if 0
    test_common_type<S<int>, S<int>>();
    test_common_type<S<int>, S<int>, S<int>>();
#endif

    //test_common_type<int, int>();
    //test_common_type<const int, int>();
    //test_common_type<volatile int, int>();
    //test_common_type<const volatile int, int>();

    //test_common_type<int, int, int>();
    //test_common_type<const int, int, int>();
    //test_common_type<int, const int, int>();
    //test_common_type<const int, const int, int>();

    // Test that we're really variadic in C++11
    //CHECK_SAME_TYPE(typename phi::common_type<int, int, int, int, int, int, int, int>::type, int);
    //CHECK_SAME_TYPE(phi::common_type_t<int, int, int, int, int, int, int, int>, int);
    //CHECK_SAME_TYPE(typename std::common_type<int, int, int, int, int, int, int, int>::type, int);

#if 0
    // P2321
    static_assert(phi::is_same_v<phi::common_type_t<phi::tuple<int>>, phi::tuple<int>>);
    static_assert(phi::is_same_v<phi::common_type_t<phi::tuple<int>, phi::tuple<long>>,
                                 phi::tuple<long>>);
    static_assert(phi::is_same_v<phi::common_type_t<phi::tuple<const int>, phi::tuple<const int>>,
                                 phi::tuple<int>>);
    static_assert(phi::is_same_v<phi::common_type_t<phi::tuple<const int&>>, phi::tuple<int>>);
    static_assert(phi::is_same_v<phi::common_type_t<phi::tuple<const volatile int&>,
                                                    phi::tuple<const volatile long&>>,
                                 phi::tuple<long>>);

    static_assert(
            phi::is_same_v<phi::common_type_t<A, phi::tuple<B>, phi::tuple<C>>, phi::tuple<B>>);

    static_assert(phi::is_same_v<phi::common_type_t<phi::pair<int, int>>, phi::pair<int, int>>);
    static_assert(phi::is_same_v<phi::common_type_t<phi::pair<int, long>, phi::pair<long, int>>,
                                 phi::pair<long, long>>);
    static_assert(phi::is_same_v<phi::common_type_t<phi::pair<const int, const long>,
                                                    phi::pair<const int, const long>>,
                                 phi::pair<int, long>>);

    static_assert(phi::is_same_v<phi::common_type_t<phi::pair<const int&, const long&>>,
                                 phi::pair<int, long>>);
    static_assert(
            phi::is_same_v<phi::common_type_t<phi::pair<const volatile int&, const volatile long&>,
                                              phi::pair<const volatile long&, const volatile int&>>,
                           phi::pair<long, long>>);

    static_assert(
            phi::is_same_v<phi::common_type_t<A, phi::tuple<B>, phi::tuple<C>>, phi::tuple<B>>);
#endif
}
