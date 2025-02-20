#include <phi/test/test_macros.hpp>

#include "constexpr_helper.hpp"
#include "test_types.hpp"
#include <phi/compiler_support/warning.hpp>
#include <phi/container/array.hpp>
#include <phi/core/max_align_t.hpp>

PHI_CLANG_AND_GCC_SUPPRESS_WARNING_PUSH()
PHI_CLANG_AND_GCC_SUPPRESS_WARNING("-Wunreachable-code")
PHI_CLANG_AND_GCC_SUPPRESS_WARNING("-Wunused-variable")
PHI_CLANG_AND_GCC_SUPPRESS_WARNING("-Wunused-result")

PHI_MSVC_SUPPRESS_WARNING_PUSH()
PHI_MSVC_SUPPRESS_WARNING(4189) // 'x': local variable is initialized but not referenced
PHI_MSVC_SUPPRESS_WARNING(4834) // discarding return value of function with 'nodiscard' attribute
PHI_MSVC_SUPPRESS_WARNING(4702) // unreachable code
PHI_MSVC_SUPPRESS_WARNING(
        5246) // 'member': the initialization of a subobject should be wrapped in braces

TEST_CASE("Array")
{
    using T   = phi::array<int, 3>;
    using TC  = phi::array<const int, 3>;
    using T0  = phi::array<int, 0>;
    using TC0 = phi::array<const int, 0>;

    SECTION("at/operator[]")
    {
        SECTION("Constexpr")
        {
            EXT_CONSTEXPR_RUNTIME T a = {1, 2, 4};

            EXT_STATIC_REQUIRE(a.at(0u) == 1);
            EXT_STATIC_REQUIRE(a[0u] == 1);
            EXT_STATIC_REQUIRE(a.at(1u) == 2);
            EXT_STATIC_REQUIRE(a[1u] == 2);
            EXT_STATIC_REQUIRE(a.at(2u) == 4);
            EXT_STATIC_REQUIRE(a[2u] == 4);
        }

        SECTION("non const")
        {
            T a = {1, 2, 4};

            CHECK_SAME_TYPE(decltype(a.at(0u)), T::reference);
            CHECK_SAME_TYPE(decltype(a.at(0u)), int&);
            CHECK_SAME_TYPE(decltype(a[0u]), T::reference);
            CHECK_SAME_TYPE(decltype(a[0u]), int&);
            CHECK_NOEXCEPT(a.at(0u));
            CHECK_NOEXCEPT(a[0u]);

            typename T::reference r1 = a.at(0u);
            CHECK(r1 == 1);
            r1 = 5;
            CHECK(r1 == 5);
            CHECK(a.at(0u) == 5);
            CHECK(a[0u] == 5);

            typename T::reference r2 = a.at(1u);
            CHECK(r2 == 2);
            r2 = 21;
            CHECK(r2 == 21);
            CHECK(a.at(1u) == 21);
            CHECK(a[1u] == 21);
        }

        SECTION("const")
        {
            const T a = {1, 2, 4};

            CHECK_SAME_TYPE(decltype(a.at(0u)), T::const_reference);
            CHECK_SAME_TYPE(decltype(a.at(0u)), const int&);
            CHECK_SAME_TYPE(decltype(a[0u]), T::const_reference);
            CHECK_SAME_TYPE(decltype(a[0u]), const int&);
            CHECK_NOEXCEPT(a.at(0u));
            CHECK_NOEXCEPT(a[0u]);

            typename T::const_reference r3 = a.at(0u);
            CHECK(r3 == 1);

            typename T::const_reference r4 = a.at(1u);
            CHECK(r4 == 2);
        }

        SECTION("Zero sized non const")
        {
            {
                T0 a = {};

                CHECK_SAME_TYPE(decltype(a.at(0u)), T0::reference);
                CHECK_SAME_TYPE(decltype(a.at(0u)), int&);
                CHECK_SAME_TYPE(decltype(a[0u]), T0::reference);
                CHECK_SAME_TYPE(decltype(a[0u]), int&);
                CHECK_NOEXCEPT(a.at(0u));
                CHECK_NOEXCEPT(a[0u]);

                if (a.size() > 0u) // always false
                {
                    T0::reference r1 = a.at(0u);
                    T0::reference r2 = a[0u];
                }
            }

            {
                TC0 a = {};

                CHECK_SAME_TYPE(decltype(a.at(0u)), TC0::reference);
                CHECK_SAME_TYPE(decltype(a.at(0u)), const int&);
                CHECK_SAME_TYPE(decltype(a[0u]), TC0::reference);
                CHECK_SAME_TYPE(decltype(a[0u]), const int&);
                CHECK_NOEXCEPT(a.at(0u));
                CHECK_NOEXCEPT(a[0u]);

                if (a.size() > 0u) // always false
                {
                    TC0::reference r1 = a.at(0u);
                    TC0::reference r2 = a[0u];
                }
            }
        }

        SECTION("Zero sized const")
        {
            {
                const T0 a = {};

                CHECK_SAME_TYPE(decltype(a.at(0u)), T0::const_reference);
                CHECK_SAME_TYPE(decltype(a.at(0u)), const int&);
                CHECK_SAME_TYPE(decltype(a[0u]), T0::const_reference);
                CHECK_SAME_TYPE(decltype(a[0u]), const int&);
                CHECK_NOEXCEPT(a.at(0u));
                CHECK_NOEXCEPT(a[0u]);

                if (a.size() > 0u) // always false
                {
                    T0::const_reference r1 = a.at(0u);
                    T0::const_reference r2 = a[0u];
                }
            }

            {
                const TC0 a = {};

                CHECK_SAME_TYPE(decltype(a.at(0u)), TC0::const_reference);
                CHECK_SAME_TYPE(decltype(a.at(0u)), const int&);
                CHECK_SAME_TYPE(decltype(a[0u]), TC0::const_reference);
                CHECK_SAME_TYPE(decltype(a[0u]), const int&);
                CHECK_NOEXCEPT(a.at(0u));
                CHECK_NOEXCEPT(a[0u]);

                if (a.size() > 0u) // always false
                {
                    TC0::const_reference r1 = a.at(0u);
                    TC0::const_reference r2 = a[0u];
                }
            }
        }

        SECTION("tracked")
        {
            phi::array<tracked, 3> tracked_array;

            CHECK(tracked_array.at(0u).value() == 0);

            tracked_array.at(0u).set_value(25);

            CHECK(tracked_array.at(0u).value() == 25);
        }

        SECTION("Traps")
        {
            phi::array<trap_constructible, 3> a = {};

            trap_constructible& a_ref = a.at(0u);

            phi::array<trap_implicit_conversion, 3> b = {};

            trap_implicit_conversion& b_ref = b.at(0u);

            phi::array<trap_comma, 3> c = {};

            trap_comma& c_ref = c.at(0u);

            phi::array<trap_call, 3> d = {};

            trap_call& d_ref = d.at(0u);

            phi::array<trap_self_assign, 3> e = {};

            trap_self_assign& e_ref = e.at(0u);
        }
    }

    SECTION("front")
    {
        SECTION("Constexpr")
        {
            EXT_CONSTEXPR_RUNTIME T constexpr_array = {1, 2, 4};

            EXT_STATIC_REQUIRE(constexpr_array.front() == 1);
        }

        SECTION("non const")
        {
            T a = {1, 2, 4};

            CHECK_SAME_TYPE(decltype(a.front()), T::reference);
            CHECK_SAME_TYPE(decltype(a.front()), int&);
            CHECK_NOEXCEPT(a.front());

            typename T::reference r1 = a.front();
            CHECK(r1 == 1);
            CHECK(a.front() == 1);
            CHECK(a.at(0u) == 1);
            CHECK(a[0u] == 1);
            r1 = 5;
            CHECK(r1 == 5);
            CHECK(a.front() == 5);
            CHECK(a.at(0u) == 5);
            CHECK(a[0u] == 5);
        }

        SECTION("const")
        {
            const T a = {1, 2, 4};

            CHECK_SAME_TYPE(decltype(a.front()), T::const_reference);
            CHECK_SAME_TYPE(decltype(a.front()), const int&);
            CHECK_NOEXCEPT(a.front());

            typename T::const_reference r1 = a.front();
            CHECK(r1 == 1);
            CHECK(a.front() == 1);
            CHECK(a.at(0u) == 1);
            CHECK(a[0u] == 1);
        }

        SECTION("Zero size non const")
        {
            T0 a = {};
            CHECK_SAME_TYPE(decltype(a.front()), T0::reference);
            CHECK_SAME_TYPE(decltype(a.front()), int&);
            CHECK_NOEXCEPT(a.front());

            if (a.size() > 0u) // always false
            {
                a.front();
            }

            TC0 b = {};
            CHECK_SAME_TYPE(decltype(b.front()), TC0::reference);
            CHECK_SAME_TYPE(decltype(b.front()), const int&);
            CHECK_NOEXCEPT(b.front());

            if (b.size() > 0u) // always false
            {
                b.front();
            }
        }

        SECTION("Zero size const")
        {
            const T0 a = {};
            CHECK_SAME_TYPE(decltype(a.front()), T0::const_reference);
            CHECK_SAME_TYPE(decltype(a.front()), const int&);
            CHECK_NOEXCEPT(a.front());

            if (a.size() > 0u) // always false
            {
                a.front();
            }

            const TC0 b = {};
            CHECK_SAME_TYPE(decltype(b.front()), TC0::const_reference);
            CHECK_SAME_TYPE(decltype(b.front()), const int&);
            CHECK_NOEXCEPT(b.front());

            if (b.size() > 0u) // always false
            {
                b.front();
            }
        }

        SECTION("tracked")
        {
            phi::array<tracked, 3> tracked_array;

            CHECK(tracked_array.front().value() == 0);

            tracked_array.front().set_value(25);

            CHECK(tracked_array.front().value() == 25);
        }

        SECTION("Traps")
        {
            phi::array<trap_constructible, 3> a = {};

            trap_constructible& a_ref = a.front();

            phi::array<trap_implicit_conversion, 3> b = {};

            trap_implicit_conversion& b_ref = b.front();

            phi::array<trap_comma, 3> c = {};

            trap_comma& c_ref = c.front();

            phi::array<trap_call, 3> d = {};

            trap_call& d_ref = d.front();

            phi::array<trap_self_assign, 3> e = {};

            trap_self_assign& e_ref = e.front();
        }
    }

    SECTION("back")
    {
        SECTION("Constexpr")
        {
            EXT_CONSTEXPR_RUNTIME T constexpr_array = {1, 2, 4};

            EXT_STATIC_REQUIRE(constexpr_array.back() == 4);
        }

        SECTION("non const")
        {
            T a = {1, 2, 4};

            CHECK_SAME_TYPE(decltype(a.back()), T::reference);
            CHECK_SAME_TYPE(decltype(a.back()), int&);
            CHECK_NOEXCEPT(a.back());

            typename T::reference r1 = a.back();
            CHECK(r1 == 4);
            CHECK(a.back() == 4);
            CHECK(a.at(2u) == 4);
            CHECK(a[2u] == 4);
            r1 = 5;
            CHECK(r1 == 5);
            CHECK(a.back() == 5);
            CHECK(a.at(2u) == 5);
            CHECK(a[2u] == 5);
        }

        SECTION("const")
        {
            const T a = {1, 2, 4};

            CHECK_SAME_TYPE(decltype(a.back()), T::const_reference);
            CHECK_SAME_TYPE(decltype(a.back()), const int&);
            CHECK_NOEXCEPT(a.back());

            typename T::const_reference r1 = a.back();
            CHECK(r1 == 4);
            CHECK(a.back() == 4);
            CHECK(a.at(2u) == 4);
            CHECK(a[2u] == 4);
        }

        SECTION("Zero size non const")
        {
            T0 a = {};
            CHECK_SAME_TYPE(decltype(a.back()), T0::reference);
            CHECK_SAME_TYPE(decltype(a.back()), int&);
            CHECK_NOEXCEPT(a.back());

            if (a.size() > 0u) // always false
            {
                a.back();
            }

            TC0 b = {};
            CHECK_SAME_TYPE(decltype(b.back()), TC0::reference);
            CHECK_SAME_TYPE(decltype(b.back()), const int&);
            CHECK_NOEXCEPT(b.back());

            if (b.size() > 0u) // always false
            {
                b.back();
            }
        }

        SECTION("Zero size const")
        {
            const T0 a = {};
            CHECK_SAME_TYPE(decltype(a.back()), T0::const_reference);
            CHECK_SAME_TYPE(decltype(a.back()), const int&);
            CHECK_NOEXCEPT(a.back());

            if (a.size() > 0u) // always false
            {
                a.back();
            }

            const TC0 b = {};
            CHECK_SAME_TYPE(decltype(b.back()), TC0::const_reference);
            CHECK_SAME_TYPE(decltype(b.back()), const int&);
            CHECK_NOEXCEPT(b.back());

            if (b.size() > 0u) // always false
            {
                b.back();
            }
        }

        SECTION("tracked")
        {
            phi::array<tracked, 3> tracked_array;

            CHECK(tracked_array.back().value() == 0);

            tracked_array.back().set_value(25);

            CHECK(tracked_array.back().value() == 25);
        }

        SECTION("Traps")
        {
            phi::array<trap_constructible, 3> a = {};

            trap_constructible& a_ref = a.back();

            phi::array<trap_implicit_conversion, 3> b = {};

            trap_implicit_conversion& b_ref = b.back();

            phi::array<trap_comma, 3> c = {};

            trap_comma& c_ref = c.back();

            phi::array<trap_call, 3> d = {};

            trap_call& d_ref = d.back();

            phi::array<trap_self_assign, 3> e = {};

            trap_self_assign& e_ref = e.back();
        }
    }

    SECTION("data")
    {
        SECTION("non const")
        {
            {
                T    a = {1, 2, 4};
                int* p = a.data();

                CHECK_SAME_TYPE(decltype(a.data()), T::pointer);
                CHECK_SAME_TYPE(decltype(a.data()), int*);
                CHECK_NOEXCEPT(a.data());

                CHECK(a.data() == &a[0u]);
                CHECK(*a.data() == a[0u]);

                CHECK(p != nullptr);
                CHECK(*p == 1);
                CHECK(p[0] == 1);
                CHECK(p[1] == 2);
                CHECK(p[2] == 4);

                p[0] = 5;

                CHECK(*p == 5);
                CHECK(p[0] == 5);
                CHECK(*a.data() == 5);
                CHECK(a.data()[0] == 5);
                CHECK(a.at(0u) == 5);
                CHECK(a[0u] == 5);
            }

            {
                TC         a = {1, 2, 4};
                const int* p = a.data();

                CHECK_SAME_TYPE(decltype(a.data()), TC::pointer);
                CHECK_SAME_TYPE(decltype(a.data()), const int*);
                CHECK_NOEXCEPT(a.data());

                CHECK(a.data() == &a[0u]);
                CHECK(*a.data() == a[0u]);

                CHECK(p != nullptr);
                CHECK(*p == 1);
                CHECK(p[0] == 1);
                CHECK(p[1] == 2);
                CHECK(p[2] == 4);
            }
        }

        SECTION("const")
        {
            {
                const T    a = {1, 2, 4};
                const int* p = a.data();

                CHECK_SAME_TYPE(decltype(a.data()), T::const_pointer);
                CHECK_SAME_TYPE(decltype(a.data()), const int*);
                CHECK_NOEXCEPT(a.data());

                CHECK(a.data() == &a[0u]);
                CHECK(*a.data() == a[0u]);

                CHECK(p != nullptr);
                CHECK(*p == 1);
                CHECK(p[0] == 1);
                CHECK(p[1] == 2);
                CHECK(p[2] == 4);
            }

            {
                const TC   a = {1, 2, 4};
                const int* p = a.data();

                CHECK_SAME_TYPE(decltype(a.data()), TC::const_pointer);
                CHECK_SAME_TYPE(decltype(a.data()), const int*);
                CHECK_NOEXCEPT(a.data());

                CHECK(a.data() == &a[0u]);
                CHECK(*a.data() == a[0u]);

                CHECK(p != nullptr);
                CHECK(*p == 1);
                CHECK(p[0] == 1);
                CHECK(p[1] == 2);
                CHECK(p[2] == 4);
            }
        }

        SECTION("zero sized non const")
        {
            {
                T0   a = {};
                int* p = a.data();

                CHECK_SAME_TYPE(decltype(a.data()), T0::pointer);
                CHECK_SAME_TYPE(decltype(a.data()), int*);
                CHECK_NOEXCEPT(a.data());

                CHECK(p == nullptr);
            }

            {
                TC0        a = {};
                const int* p = a.data();

                CHECK_SAME_TYPE(decltype(a.data()), TC0::pointer);
                CHECK_SAME_TYPE(decltype(a.data()), const int*);
                CHECK_NOEXCEPT(a.data());

                CHECK(p == nullptr);
            }

            {
                using T2                     = phi::array<non_default_constructible, 0>;
                T2                         a = {};
                non_default_constructible* p = a.data();

                CHECK_SAME_TYPE(decltype(a.data()), T2::pointer);
                CHECK_SAME_TYPE(decltype(a.data()), non_default_constructible*);
                CHECK_NOEXCEPT(a.data());

                CHECK(p == nullptr);
            }
        }

        SECTION("zero sized const")
        {
            {
                const T0   a = {};
                const int* p = a.data();

                CHECK_SAME_TYPE(decltype(a.data()), T0::const_pointer);
                CHECK_SAME_TYPE(decltype(a.data()), const int*);
                CHECK_NOEXCEPT(a.data());

                CHECK(p == nullptr);
            }

            {
                const TC0  a = {};
                const int* p = a.data();

                CHECK_SAME_TYPE(decltype(a.data()), TC0::const_pointer);
                CHECK_SAME_TYPE(decltype(a.data()), const int*);
                CHECK_NOEXCEPT(a.data());

                CHECK(p == nullptr);
            }

            {
                using T2                           = phi::array<non_default_constructible, 0>;
                const T2                         a = {};
                const non_default_constructible* p = a.data();

                CHECK_SAME_TYPE(decltype(a.data()), T2::const_pointer);
                CHECK_SAME_TYPE(decltype(a.data()), const non_default_constructible*);
                CHECK_NOEXCEPT(a.data());

                CHECK(p == nullptr);
            }
        }

        SECTION("alignment")
        {
            {
                using T2                       = phi::array<natural_alignment, 0>;
                const T2                 a     = {};
                const natural_alignment* p     = a.data();
                std::uintptr_t           p_int = reinterpret_cast<std::uintptr_t>(p);

                CHECK(p_int % alignof(natural_alignment) == 0);
            }

            {
                using T2                       = phi::array<natural_alignment, 3>;
                const T2                 a     = {};
                const natural_alignment* p     = a.data();
                std::uintptr_t           p_int = reinterpret_cast<std::uintptr_t>(p);

                CHECK(p_int % alignof(natural_alignment) == 0);
            }

            {
                using T2                      = phi::array<phi::max_align_t, 0>;
                const T2                a     = {};
                const phi::max_align_t* p     = a.data();
                std::uintptr_t          p_int = reinterpret_cast<std::uintptr_t>(p);

                CHECK(p_int % alignof(phi::max_align_t) == 0);
            }

            {
                using T2                      = phi::array<phi::max_align_t, 3>;
                const T2                a     = {};
                const phi::max_align_t* p     = a.data();
                std::uintptr_t          p_int = reinterpret_cast<std::uintptr_t>(p);

                CHECK(p_int % alignof(phi::max_align_t) == 0);
            }
        }

        SECTION("tracked")
        {
            phi::array<tracked, 3> tracked_array;
            tracked*               p = tracked_array.data();

            CHECK(p->value() == 0);

            p->set_value(25);

            CHECK(p->value() == 25);
        }

        SECTION("Traps")
        {
            phi::array<trap_constructible, 3> a = {};

            trap_constructible* a_p = a.data();

            phi::array<trap_implicit_conversion, 3> b = {};

            trap_implicit_conversion* b_p = b.data();

            phi::array<trap_comma, 3> c = {};

            trap_comma* c_p = c.data();

            phi::array<trap_call, 3> d = {};

            trap_call* d_p = d.data();

            phi::array<trap_self_assign, 3> e = {};

            trap_self_assign* e_p = e.data();
        }
    }
}

PHI_MSVC_SUPPRESS_WARNING_POP()
PHI_CLANG_AND_GCC_SUPPRESS_WARNING_POP()
