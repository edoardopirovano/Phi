#include <phi/test/test_macros.hpp>

#include <phi/core/move.hpp>
#include <phi/core/ref_ptr.hpp>
#include <phi/math/vector2.hpp>
#include <phi/type_traits/is_copy_assignable.hpp>
#include <phi/type_traits/is_copy_constructible.hpp>
#include <phi/type_traits/is_default_constructible.hpp>
#include <phi/type_traits/is_move_assignable.hpp>
#include <phi/type_traits/is_move_constructible.hpp>
#include <thread>
#include <vector>

struct A
{
    bool f()
    {
        return true;
    }
};

TEST_CASE("ref_ptr")
{
    SECTION("Layout")
    {
        STATIC_REQUIRE(sizeof(phi::ref_ptr<int>) == sizeof(int*) * 2);

        STATIC_REQUIRE(phi::is_default_constructible<phi::ref_ptr<int>>::value);
        STATIC_REQUIRE(phi::is_copy_constructible<phi::ref_ptr<int>>::value);
        STATIC_REQUIRE(phi::is_move_constructible<phi::ref_ptr<int>>::value);
        STATIC_REQUIRE(phi::is_copy_assignable<phi::ref_ptr<int>>::value);
        STATIC_REQUIRE(phi::is_move_assignable<phi::ref_ptr<int>>::value);
    }

    SECTION("Type aliases")
    {
        CHECK_SAME_TYPE(phi::ref_ptr<int>::this_type, phi::ref_ptr<int>);
        CHECK_SAME_TYPE(phi::ref_ptr<int>::value_type, int);
        CHECK_SAME_TYPE(phi::ref_ptr<int>::reference, int&);
        CHECK_SAME_TYPE(phi::ref_ptr<int>::const_reference, const int&);
        CHECK_SAME_TYPE(phi::ref_ptr<int>::pointer, int*);
        CHECK_SAME_TYPE(phi::ref_ptr<int>::const_pointer, const int*);
    }

    SECTION("ref_ptr()")
    {
        phi::ref_ptr<int> ptr;

        CHECK(ptr.get() == nullptr);
        CHECK_FALSE(ptr);
    }

    SECTION("ref_ptr(nullptr_t)")
    {
        phi::ref_ptr<int> ptr(nullptr);

        CHECK(ptr.get() == nullptr);
        CHECK_FALSE(ptr);
    }

    SECTION("ref_ptr(TypeT*)")
    {
        int*              raw_ptr = new int(22);
        phi::ref_ptr<int> ptr(raw_ptr);

        REQUIRE(ptr);
        CHECK(ptr.get() == raw_ptr);
    }

    SECTION("ref_ptr(const ref_ptr&)")
    {
        int*              raw_ptr = new int(33);
        phi::ref_ptr<int> ptr(raw_ptr);
        phi::ref_ptr<int> copy(ptr);

        REQUIRE(ptr);
        REQUIRE(copy);
        CHECK(copy.get() == raw_ptr);
        CHECK(ptr == copy);
    }

    SECTION("ref_ptr(ref_ptr&&)")
    {
        int*              raw_ptr = new int(12);
        phi::ref_ptr<int> base(raw_ptr);
        phi::ref_ptr<int> copy(phi::move(base));

        CHECK_FALSE(base);
        CHECK(base.get() == nullptr);
        CHECK(copy);
        CHECK(copy.get() == raw_ptr);
    }

    SECTION("ref_ptr(const not_null_ref_ptr&)")
    {
        int*                       raw_ptr = new int(21);
        phi::not_null_ref_ptr<int> not_null(raw_ptr);
        phi::ref_ptr<int>          ptr(not_null);

        CHECK(ptr);
        CHECK(ptr.get() == raw_ptr);
        CHECK(ptr.get() == not_null.get());
    }

    SECTION("ref_ptr(not_null_ref_ptr&&)")
    {
        int*                       raw_ptr = new int(21);
        phi::not_null_ref_ptr<int> not_null(raw_ptr);
        phi::ref_ptr<int>          ptr(phi::move(not_null));

        CHECK(ptr);
        CHECK(ptr.get() == raw_ptr);
    }

    SECTION("operator=(nullptr_t)")
    {
        phi::ref_ptr<int> ptr(new int(21));

        CHECK(ptr);

        ptr = nullptr;

        CHECK_FALSE(ptr);
        CHECK(ptr.get() == nullptr);

        ptr = nullptr;

        CHECK_FALSE(ptr);
        CHECK(ptr.get() == nullptr);
    }

    SECTION("operator=(TypeT*)")
    {
        phi::ref_ptr<int> ptr(new int(21));
        int*              raw_ptr = new int(33);
        int*              null    = nullptr;

        ptr = raw_ptr;

        CHECK(ptr);
        CHECK(ptr.get() == raw_ptr);

        ptr = null;

        CHECK_FALSE(ptr);
        CHECK(ptr.get() == nullptr);

        int* raw_ptr2 = new int(44);
        ptr           = raw_ptr2;

        CHECK(ptr);
        CHECK(ptr.get() == raw_ptr2);
    }

    SECTION("operator=(const ref_ptr&)")
    {
        int*              raw_ptr  = new int(21);
        int*              raw_ptr2 = new int(22);
        phi::ref_ptr<int> ptr(raw_ptr);
        phi::ref_ptr<int> ptr2(raw_ptr2);

        ptr = ptr2;

        CHECK(ptr);
        CHECK(ptr.get() == raw_ptr2);

        ptr = phi::ref_ptr<int>();

        CHECK_FALSE(ptr);
        CHECK(ptr.get() == nullptr);

        int* raw_ptr3 = new int(44);
        ptr           = raw_ptr3;

        CHECK(ptr);
        CHECK(ptr.get() == raw_ptr3);
    }

    SECTION("operator=(ref_ptr&&)")
    {
        int*              raw_ptr  = new int(21);
        int*              raw_ptr2 = new int(22);
        phi::ref_ptr<int> ptr(raw_ptr);
        phi::ref_ptr<int> ptr2(raw_ptr2);

        ptr = phi::move(ptr2);

        CHECK(ptr);
        CHECK(ptr.get() == raw_ptr2);

        int* raw_ptr3 = new int(23);
        ptr           = phi::ref_ptr<int>(raw_ptr3);

        CHECK(ptr);
        CHECK(ptr.get() == raw_ptr3);

        ptr = phi::ref_ptr<int>();

        CHECK_FALSE(ptr);
        CHECK(ptr.get() == nullptr);

        int* raw_ptr4 = new int(24);

        ptr = phi::ref_ptr<int>(raw_ptr4);

        CHECK(ptr);
        CHECK(ptr.get() == raw_ptr4);
    }

    SECTION("operator=(const not_null_ref_ptr&)")
    {
        int*                       raw_ptr  = new int(21);
        int*                       raw_ptr2 = new int(22);
        phi::ref_ptr<int>          ptr(raw_ptr);
        phi::not_null_ref_ptr<int> not_null(raw_ptr2);

        ptr = not_null;

        CHECK(ptr);
        CHECK(ptr.get() == raw_ptr2);

        int* raw_ptr3 = new int(44);
        ptr           = phi::not_null_ref_ptr<int>(raw_ptr3);

        CHECK(ptr);
        CHECK(ptr.get() == raw_ptr3);
    }

    SECTION("operator=(not_null_ref_ptr&&)")
    {
        // int*                    raw_ptr  = new int(21);
        // int*                    raw_ptr2 = new int(22);
        // phi::ref_ptr<int>        ptr(raw_ptr);
        // phi::not_null_ref_ptr<int> not_null(raw_ptr2);

        // ptr = phi::move(not_null);

        // CHECK(ptr);
        // CHECK(ptr.get() == raw_ptr2);

        // int*                    raw_ptr3 = new int(23);
        // phi::not_null_ref_ptr<int> not_null2(raw_ptr3);
        // ptr = phi::move(not_null2);

        // CHECK(ptr);
        // CHECK(ptr.get() == raw_ptr3);

        // ptr.clear();

        // int*                    raw_ptr4 = new int(24);
        // phi::not_null_ref_ptr<int> not_null3(raw_ptr4);
        // ptr = phi::move(not_null3);

        // CHECK(ptr);
        // CHECK(ptr.get() == raw_ptr4);
    }

    SECTION("clear")
    {
        phi::ref_ptr<int> ptr;

        ptr.clear();

        CHECK_FALSE(ptr);
        CHECK(ptr.get() == nullptr);

        ptr = new int(23);
        ptr.clear();

        CHECK(ptr.get() == nullptr);
    }

    SECTION("reset")
    {
        phi::ref_ptr<int> ptr;

        int* raw_ptr = new int(21);
        ptr.reset(raw_ptr);

        CHECK(ptr.get() == raw_ptr);

        int* raw_ptr2 = new int(22);
        ptr.reset(raw_ptr2);

        CHECK(ptr.get() == raw_ptr2);

        ptr.reset(raw_ptr2);

        CHECK(ptr.get() == raw_ptr2);

        ptr.reset(nullptr);

        CHECK(ptr.get() == nullptr);
    }

    SECTION("swap")
    {
        int*              raw_ptr  = new int(21);
        int*              raw_ptr2 = new int(22);
        phi::ref_ptr<int> ptr(raw_ptr);
        phi::ref_ptr<int> ptr2(raw_ptr2);

        ptr.swap(ptr2);

        CHECK(ptr.get() == raw_ptr2);
        CHECK(ptr2.get() == raw_ptr);

        phi::swap(ptr, ptr2);

        CHECK(ptr.get() == raw_ptr);
        CHECK(ptr2.get() == raw_ptr2);
    }

    SECTION("use_count")
    {
        int*              raw_ptr = new int(22);
        phi::ref_ptr<int> ptr(raw_ptr);

        REQUIRE(ptr);
        CHECK(ptr.use_count() == 1u);

        {
            phi::ref_ptr<int> shared_ptr_1(ptr);

            CHECK(ptr.use_count() == 2u);

            {
                phi::ref_ptr<int> shared_ptr_2(ptr);

                CHECK(ptr.use_count() == 3u);
                {
                    phi::ref_ptr<int> shared_ptr_3(ptr);

                    CHECK(ptr.use_count() == 4u);
                }

                CHECK(ptr.use_count() == 3u);

                phi::ref_ptr<int> shared_ptr_4(ptr);

                CHECK(ptr.use_count() == 4u);
            }

            CHECK(ptr.use_count() == 2u);
        }

        CHECK(ptr.use_count() == 1u);

        phi::ref_ptr<int> null;

        CHECK(null.use_count() == 0u);
    }

    SECTION("operator TypeT*")
    {
        int*              raw_ptr = new int(12);
        phi::ref_ptr<int> ptr(raw_ptr);

        int* new_ptr = static_cast<int*>(ptr);

        CHECK(new_ptr == raw_ptr);
    }

    SECTION("operator->")
    {
        phi::ref_ptr<A> ptr(new A);

        CHECK(ptr->f());
    }

    SECTION("operator*")
    {
        int*              raw_ptr = new int(0);
        phi::ref_ptr<int> ptr(raw_ptr);

        CHECK(*ptr == 0);

        *raw_ptr = 12;

        CHECK(*ptr == 12);

        *ptr = 9;

        CHECK(*ptr == 9);
        CHECK(*raw_ptr == 9);
    }

    SECTION("operator==")
    {
        phi::ref_ptr<int> ptr1(new int(1));
        phi::ref_ptr<int> ptr2(new int(2));
        phi::ref_ptr<int> null;

        CHECK_FALSE(ptr1 == ptr2);
        CHECK_FALSE(ptr1 == null);
        CHECK_FALSE(ptr2 == null);

        CHECK_FALSE(ptr1 == nullptr);
        CHECK_FALSE(ptr2 == nullptr);
        CHECK(null == nullptr);

        CHECK_FALSE(nullptr == ptr1);
        CHECK_FALSE(nullptr == ptr2);
        CHECK(nullptr == null);

        ptr2 = ptr1;

        CHECK(ptr1 == ptr2);
    }

    SECTION("operator!=")
    {
        phi::ref_ptr<int> ptr1(new int(1));
        phi::ref_ptr<int> ptr2(new int(2));
        phi::ref_ptr<int> null;

        CHECK(ptr1 != ptr2);
        CHECK(ptr1 != null);
        CHECK(ptr2 != null);

        CHECK(ptr1 != nullptr);
        CHECK(ptr2 != nullptr);
        CHECK_FALSE(null != nullptr);

        CHECK(nullptr != ptr1);
        CHECK(nullptr != ptr2);
        CHECK_FALSE(nullptr != null);

        ptr2 = ptr1;

        CHECK_FALSE(ptr1 != ptr2);
    }

    SECTION("make_ref")
    {
        phi::ref_ptr<int> int_ptr  = phi::make_ref<int>();
        phi::ref_ptr<int> int_ptr2 = phi::make_ref<int>(3);

        CHECK(*int_ptr2 == 3);

        phi::ref_ptr<phi::vector2i32> vec2_ptr = phi::make_ref<phi::vector2i32>(3, 12);

        CHECK((vec2_ptr->x == 3));
        CHECK((vec2_ptr->y == 12));
    }
}

TEST_CASE("not_null_ref_ptr", "[Core][ref_ptr][ref_ptr][not_null_ref_ptr]")
{
    SECTION("Layout")
    {
        STATIC_REQUIRE(sizeof(phi::not_null_ref_ptr<int>) == sizeof(int*) * 2);

        STATIC_REQUIRE_FALSE(phi::is_default_constructible<phi::not_null_ref_ptr<int>>::value);
        STATIC_REQUIRE(phi::is_copy_constructible<phi::not_null_ref_ptr<int>>::value);
        STATIC_REQUIRE(phi::is_move_constructible<phi::not_null_ref_ptr<int>>::value);
        STATIC_REQUIRE(phi::is_copy_assignable<phi::not_null_ref_ptr<int>>::value);
        STATIC_REQUIRE(phi::is_move_assignable<phi::not_null_ref_ptr<int>>::value);
    }

    SECTION("Type aliases")
    {
        CHECK_SAME_TYPE(phi::not_null_ref_ptr<int>::this_type, phi::not_null_ref_ptr<int>);
        CHECK_SAME_TYPE(phi::not_null_ref_ptr<int>::value_type, int);
        CHECK_SAME_TYPE(phi::not_null_ref_ptr<int>::reference, int&);
        CHECK_SAME_TYPE(phi::not_null_ref_ptr<int>::const_reference, const int&);
        CHECK_SAME_TYPE(phi::not_null_ref_ptr<int>::pointer, int*);
        CHECK_SAME_TYPE(phi::not_null_ref_ptr<int>::const_pointer, const int*);
    }

    SECTION("not_null_ref_ptr(TypeT*)")
    {
        int*                       raw_ptr = new int(22);
        phi::not_null_ref_ptr<int> ptr(raw_ptr);

        CHECK(ptr.get() == raw_ptr);
    }

    SECTION("not_null_ref_ptr(const not_null_ref_ptr&)")
    {
        int*                       raw_ptr = new int(33);
        phi::not_null_ref_ptr<int> ptr(raw_ptr);
        phi::not_null_ref_ptr<int> copy(ptr);

        CHECK(copy.get() == raw_ptr);
        CHECK(ptr == copy);
    }

    SECTION("not_null_ref_ptr(not_null_ref_ptr&&)")
    {
        int*                       raw_ptr = new int(12);
        phi::not_null_ref_ptr<int> base(raw_ptr);
        phi::not_null_ref_ptr<int> copy(phi::move(base));

        CHECK(copy.get() == raw_ptr);
    }

    SECTION("operator=(TypeT*)")
    {
        phi::not_null_ref_ptr<int> ptr(new int(21));
        int*                       raw_ptr = new int(33);

        ptr = raw_ptr;

        CHECK(ptr.get() == raw_ptr);

        int* raw_ptr2 = new int(44);
        ptr           = raw_ptr2;

        CHECK(ptr.get() == raw_ptr2);
    }

    SECTION("operator=(const not_null_ref_ptr&)")
    {
        int*                       raw_ptr  = new int(21);
        int*                       raw_ptr2 = new int(22);
        phi::not_null_ref_ptr<int> ptr(raw_ptr);
        phi::not_null_ref_ptr<int> ptr2(raw_ptr2);

        ptr = ptr2;

        CHECK(ptr.get() == raw_ptr2);

        int* raw_ptr3 = new int(44);
        ptr           = raw_ptr3;

        CHECK(ptr.get() == raw_ptr3);
    }

    SECTION("operator=(not_null_ref_ptr&&)")
    {
        int*                       raw_ptr  = new int(21);
        int*                       raw_ptr2 = new int(22);
        phi::not_null_ref_ptr<int> ptr(raw_ptr);
        phi::not_null_ref_ptr<int> ptr2(raw_ptr2);

        ptr = phi::move(ptr2);

        CHECK(ptr.get() == raw_ptr2);

        int* raw_ptr3 = new int(23);
        ptr           = phi::not_null_ref_ptr<int>(raw_ptr3);

        CHECK(ptr.get() == raw_ptr3);

        int* raw_ptr4 = new int(24);

        ptr = phi::not_null_ref_ptr<int>(raw_ptr4);

        CHECK(ptr.get() == raw_ptr4);
    }

    SECTION("reset")
    {
        phi::not_null_ref_ptr<int> ptr(new int(20));

        int* raw_ptr = new int(21);
        ptr.reset(raw_ptr);

        CHECK(ptr.get() == raw_ptr);

        int* raw_ptr2 = new int(22);
        ptr.reset(raw_ptr2);

        CHECK(ptr.get() == raw_ptr2);

        ptr.reset(raw_ptr2);

        CHECK(ptr.get() == raw_ptr2);
    }

    SECTION("swap")
    {
        int*                       raw_ptr  = new int(21);
        int*                       raw_ptr2 = new int(22);
        phi::not_null_ref_ptr<int> ptr(raw_ptr);
        phi::not_null_ref_ptr<int> ptr2(raw_ptr2);

        ptr.swap(ptr2);

        CHECK(ptr.get() == raw_ptr2);
        CHECK(ptr2.get() == raw_ptr);

        phi::swap(ptr, ptr2);

        CHECK(ptr.get() == raw_ptr);
        CHECK(ptr2.get() == raw_ptr2);
    }

    SECTION("use_count")
    {
        int*                       raw_ptr = new int(22);
        phi::not_null_ref_ptr<int> ptr(raw_ptr);

        CHECK(ptr.use_count() == 1u);

        {
            phi::not_null_ref_ptr<int> shared_ptr_1(ptr);

            CHECK(ptr.use_count() == 2u);

            {
                phi::not_null_ref_ptr<int> shared_ptr_2(ptr);

                CHECK(ptr.use_count() == 3u);
                {
                    phi::not_null_ref_ptr<int> shared_ptr_3(ptr);

                    CHECK(ptr.use_count() == 4u);
                }

                CHECK(ptr.use_count() == 3u);

                phi::not_null_ref_ptr<int> shared_ptr_4(ptr);

                CHECK(ptr.use_count() == 4u);
            }

            CHECK(ptr.use_count() == 2u);
        }

        CHECK(ptr.use_count() == 1u);
    }

    SECTION("operator TypeT*")
    {
        int*                       raw_ptr = new int(12);
        phi::not_null_ref_ptr<int> ptr(raw_ptr);

        int* new_ptr = static_cast<int*>(ptr);

        CHECK(new_ptr == raw_ptr);
    }

    SECTION("operator->")
    {
        phi::not_null_ref_ptr<A> ptr(new A);

        CHECK(ptr->f());
    }

    SECTION("operator*")
    {
        int*                       raw_ptr = new int(0);
        phi::not_null_ref_ptr<int> ptr(raw_ptr);

        CHECK(*ptr == 0);

        *raw_ptr = 12;

        CHECK(*ptr == 12);

        *ptr = 9;

        CHECK(*ptr == 9);
        CHECK(*raw_ptr == 9);
    }

    SECTION("operator==")
    {
        phi::not_null_ref_ptr<int> ptr1(new int(1));
        phi::not_null_ref_ptr<int> ptr2(new int(2));

        CHECK_FALSE(ptr1 == ptr2);

        ptr2 = ptr1;

        CHECK(ptr1 == ptr2);
    }

    SECTION("operator!=")
    {
        phi::not_null_ref_ptr<int> ptr1(new int(1));
        phi::not_null_ref_ptr<int> ptr2(new int(2));

        CHECK(ptr1 != ptr2);

        ptr2 = ptr1;

        CHECK_FALSE(ptr1 != ptr2);
    }

    SECTION("make_not_null_ref")
    {
        phi::not_null_ref_ptr<int> int_ptr = phi::make_not_null_ref<int>(3);

        CHECK(*int_ptr == 3);

        phi::not_null_ref_ptr<phi::vector2i32> vec2_ptr =
                phi::make_not_null_ref<phi::vector2i32>(3, 12);

        CHECK((vec2_ptr->x == 3));
        CHECK((vec2_ptr->y == 12));
    }
}
