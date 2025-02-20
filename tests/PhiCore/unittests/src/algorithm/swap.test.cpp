#include <phi/test/test_macros.hpp>

#include "test_types.hpp"
#include <phi/compiler_support/warning.hpp>
#include <phi/core/declval.hpp>
#include <phi/type_traits/is_same.hpp>

PHI_GCC_SUPPRESS_WARNING_PUSH()
PHI_GCC_SUPPRESS_WARNING("-Wnoexcept")

#include <phi/algorithm/swap.hpp>
#include <phi/core/scope_ptr.hpp>

struct CopyOnly
{
    CopyOnly()
    {}
    CopyOnly(CopyOnly const&) noexcept
    {}
    CopyOnly& operator=(CopyOnly const&)
    {
        return *this;
    }
};

struct MoveOnly
{
    MoveOnly()
    {}
    MoveOnly(MoveOnly&&)
    {}
    MoveOnly& operator=(MoveOnly&&) noexcept
    {
        return *this;
    }
};

struct NoexceptMoveOnly
{
    NoexceptMoveOnly()
    {}
    NoexceptMoveOnly(NoexceptMoveOnly&&) noexcept
    {}
    NoexceptMoveOnly& operator=(NoexceptMoveOnly&&) noexcept
    {
        return *this;
    }
};

struct NotMoveConstructible
{
    NotMoveConstructible& operator=(NotMoveConstructible&&)
    {
        return *this;
    }

private:
    NotMoveConstructible(NotMoveConstructible&&);
};

struct NotMoveAssignable
{
    NotMoveAssignable(NotMoveAssignable&&);

private:
    NotMoveAssignable& operator=(NotMoveAssignable&&);
};

template <typename TypeT>
auto can_swap_test(int) -> decltype(phi::swap(phi::declval<TypeT>(), phi::declval<TypeT>()));

template <typename TypeT>
auto can_swap_test(...) -> phi::false_type;

template <typename TypeT>
constexpr bool can_swap()
{
    return phi::is_same<decltype(can_swap_test<TypeT>(0)), void>::value;
}

#if PHI_HAS_FEATURE_EXTENDED_CONSTEXPR()
constexpr bool test_swap_constexpr()
{
    int i = 1;
    int j = 2;
    phi::swap(i, j);
    return i == 2 && j == 1;
}
#endif

TEST_CASE("swap")
{
    {
        int i = 1;
        int j = 2;
        phi::swap(i, j);
        CHECK(i == 2);
        CHECK(j == 1);
    }

    // phi::scope_ptr
    {
        phi::scope_ptr<int> i(new int(1));
        phi::scope_ptr<int> j(new int(2));
        phi::swap(i, j);
        CHECK(*i == 2);
        CHECK(*j == 1);
    }
    {
        // test that the swap
        STATIC_REQUIRE(can_swap<CopyOnly&>());
        STATIC_REQUIRE(can_swap<MoveOnly&>());
        STATIC_REQUIRE(can_swap<NoexceptMoveOnly&>());

        STATIC_REQUIRE_FALSE(can_swap<NotMoveConstructible&>());
        STATIC_REQUIRE_FALSE(can_swap<NotMoveAssignable&>());

        CopyOnly         c;
        MoveOnly         m;
        NoexceptMoveOnly nm;
        CHECK_NOT_NOEXCEPT(phi::swap(c, c));
        CHECK_NOT_NOEXCEPT(phi::swap(m, m));
        CHECK_NOEXCEPT(phi::swap(nm, nm));
    }

    {
        // Check that swap does not call any copy constructor
        non_copyable a;
        non_copyable b;

        phi::swap(a, b);
    }

    {
        tracked a;
        tracked b;

        phi::swap(a, b);
    }

    {
        trap_constructible a;
        trap_constructible b;

        phi::swap(a, b);
    }

    {
        trap_implicit_conversion a;
        trap_implicit_conversion b;

        phi::swap(a, b);
    }

    {
        trap_comma a;
        trap_comma b;

        phi::swap(a, b);
    }

    {
        trap_call a;
        trap_call b;

        phi::swap(a, b);
    }

    {
        trap_self_assign a;
        trap_self_assign b;

        phi::swap(a, b);
    }

    {
        trap_deref a;
        trap_deref b;

        phi::swap(a, b);
    }

    {
        trap_array_subscript a;
        trap_array_subscript b;

        phi::swap(a, b);
    }

#if PHI_HAS_FEATURE_EXTENDED_CONSTEXPR()
    STATIC_REQUIRE(test_swap_constexpr());
#endif
}

PHI_GCC_SUPPRESS_WARNING_POP()
