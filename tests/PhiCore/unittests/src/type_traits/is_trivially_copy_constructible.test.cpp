#include <phi/test/test_macros.hpp>

#include "test_types.hpp"
#include "type_traits_helper.hpp"
#include <phi/compiler_support/char8_t.hpp>
#include <phi/core/boolean.hpp>
#include <phi/core/floating_point.hpp>
#include <phi/core/integer.hpp>
#include <phi/core/nullptr_t.hpp>
#include <phi/core/scope_ptr.hpp>
#include <phi/type_traits/is_copy_constructible.hpp>
#include <phi/type_traits/is_trivially_copy_constructible.hpp>
#include <type_traits>
#include <vector>

template <typename T>
void test_is_trivially_copy_constructible_impl()
{
#if PHI_HAS_WORKING_IS_TRIVIALLY_COPY_CONSTRUCTIBLE()
    STATIC_REQUIRE(phi::is_trivially_copy_constructible<T>::value);
    STATIC_REQUIRE_FALSE(phi::is_not_trivially_copy_constructible<T>::value);
    STATIC_REQUIRE(phi::is_copy_constructible<T>::value);

#    if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()
    STATIC_REQUIRE(phi::is_trivially_copy_constructible_v<T>);
    STATIC_REQUIRE_FALSE(phi::is_not_trivially_copy_constructible_v<T>);
    STATIC_REQUIRE(phi::is_copy_constructible_v<T>);
#    endif

    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_trivially_copy_constructible<T>);
    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_not_trivially_copy_constructible<T>);

    // Standard compatibility
    STATIC_REQUIRE(std::is_trivially_copy_constructible<T>::value);
#endif
}

template <typename T>
void test_is_not_trivially_copy_constructible_impl()
{
#if PHI_HAS_WORKING_IS_TRIVIALLY_COPY_CONSTRUCTIBLE()
    STATIC_REQUIRE_FALSE(phi::is_trivially_copy_constructible<T>::value);
    STATIC_REQUIRE(phi::is_not_trivially_copy_constructible<T>::value);

#    if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()
    STATIC_REQUIRE_FALSE(phi::is_trivially_copy_constructible_v<T>);
    STATIC_REQUIRE(phi::is_not_trivially_copy_constructible_v<T>);
#    endif

    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_trivially_copy_constructible<T>);
    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_not_trivially_copy_constructible<T>);

    // Standard compatibility
    STATIC_REQUIRE_FALSE(std::is_trivially_copy_constructible<T>::value);
#endif
}

template <typename T>
void test_is_trivially_copy_constructible()
{
    test_is_trivially_copy_constructible_impl<T>();
    test_is_trivially_copy_constructible_impl<const T>();
    test_is_not_trivially_copy_constructible_impl<volatile T>();
    test_is_not_trivially_copy_constructible_impl<const volatile T>();
}

template <typename T>
void test_is_trivially_copy_constructible_cv()
{
    test_is_trivially_copy_constructible_impl<T>();
    test_is_trivially_copy_constructible_impl<const T>();
    test_is_trivially_copy_constructible_impl<volatile T>();
    test_is_trivially_copy_constructible_impl<const volatile T>();
}

template <typename T>
void test_is_not_trivially_copy_constructible()
{
    test_is_not_trivially_copy_constructible_impl<T>();
    test_is_not_trivially_copy_constructible_impl<const T>();
    test_is_not_trivially_copy_constructible_impl<volatile T>();
    test_is_not_trivially_copy_constructible_impl<const volatile T>();
}

struct A
{
    A(const A&);
};

struct B
{
    B(const B&)
    {}
};

struct C
{
    C& operator=(const C&) = default;
};

struct D
{
    D(const D&) = delete;
};

TEST_CASE("is_trivially_copy_constructible")
{
    test_is_not_trivially_copy_constructible<A>();
    test_is_not_trivially_copy_constructible<B>();
    test_is_trivially_copy_constructible<C>();
    test_is_not_trivially_copy_constructible<D>();

    test_is_not_trivially_copy_constructible<void>();
    test_is_trivially_copy_constructible_cv<phi::nullptr_t>();
    test_is_trivially_copy_constructible_cv<bool>();
    test_is_trivially_copy_constructible_cv<char>();
    test_is_trivially_copy_constructible_cv<signed char>();
    test_is_trivially_copy_constructible_cv<unsigned char>();
    test_is_trivially_copy_constructible_cv<short>();
    test_is_trivially_copy_constructible_cv<unsigned short>();
    test_is_trivially_copy_constructible_cv<int>();
    test_is_trivially_copy_constructible_cv<unsigned int>();
    test_is_trivially_copy_constructible_cv<long>();
    test_is_trivially_copy_constructible_cv<unsigned long>();
    test_is_trivially_copy_constructible_cv<long long>();
    test_is_trivially_copy_constructible_cv<unsigned long long>();
    test_is_trivially_copy_constructible_cv<float>();
    test_is_trivially_copy_constructible_cv<double>();
    test_is_trivially_copy_constructible_cv<long double>();
    test_is_trivially_copy_constructible_cv<char8_t>();
    test_is_trivially_copy_constructible_cv<char16_t>();
    test_is_trivially_copy_constructible_cv<char32_t>();
    test_is_trivially_copy_constructible_cv<wchar_t>();

    test_is_trivially_copy_constructible<phi::boolean>();
    test_is_trivially_copy_constructible<phi::integer<signed char>>();
    test_is_trivially_copy_constructible<phi::integer<unsigned char>>();
    test_is_trivially_copy_constructible<phi::integer<short>>();
    test_is_trivially_copy_constructible<phi::integer<unsigned short>>();
    test_is_trivially_copy_constructible<phi::integer<int>>();
    test_is_trivially_copy_constructible<phi::integer<unsigned int>>();
    test_is_trivially_copy_constructible<phi::integer<long>>();
    test_is_trivially_copy_constructible<phi::integer<unsigned long>>();
    test_is_trivially_copy_constructible<phi::integer<long long>>();
    test_is_trivially_copy_constructible<phi::integer<unsigned long long>>();
    test_is_trivially_copy_constructible<phi::floating_point<float>>();
    test_is_trivially_copy_constructible<phi::floating_point<double>>();
    test_is_trivially_copy_constructible<phi::floating_point<long double>>();

    test_is_not_trivially_copy_constructible<std::vector<int>>();
    test_is_not_trivially_copy_constructible<phi::scope_ptr<int>>();

    test_is_trivially_copy_constructible_cv<int&>();
    test_is_trivially_copy_constructible_cv<const int&>();
    test_is_trivially_copy_constructible_cv<volatile int&>();
    test_is_trivially_copy_constructible_cv<const volatile int&>();
    test_is_not_trivially_copy_constructible<int&&>();
    test_is_not_trivially_copy_constructible<const int&&>();
    test_is_not_trivially_copy_constructible<volatile int&&>();
    test_is_not_trivially_copy_constructible<const volatile int&&>();
    test_is_trivially_copy_constructible_cv<int*>();
    test_is_trivially_copy_constructible_cv<const int*>();
    test_is_trivially_copy_constructible_cv<volatile int*>();
    test_is_trivially_copy_constructible_cv<const volatile int*>();
    test_is_trivially_copy_constructible_cv<int**>();
    test_is_trivially_copy_constructible_cv<const int**>();
    test_is_trivially_copy_constructible_cv<volatile int**>();
    test_is_trivially_copy_constructible_cv<const volatile int**>();
    test_is_trivially_copy_constructible_cv<int*&>();
    test_is_trivially_copy_constructible_cv<const int*&>();
    test_is_trivially_copy_constructible_cv<volatile int*&>();
    test_is_trivially_copy_constructible_cv<const volatile int*&>();
    test_is_not_trivially_copy_constructible<int*&&>();
    test_is_not_trivially_copy_constructible<const int*&&>();
    test_is_not_trivially_copy_constructible<volatile int*&&>();
    test_is_not_trivially_copy_constructible<const volatile int*&&>();
    test_is_trivially_copy_constructible_cv<void*>();
    test_is_not_trivially_copy_constructible<char[3]>();
    test_is_not_trivially_copy_constructible<char[]>();
    test_is_not_trivially_copy_constructible<char* [3]>();
    test_is_not_trivially_copy_constructible<char*[]>();
    test_is_trivially_copy_constructible_cv<int(*)[3]>();
    test_is_trivially_copy_constructible_cv<int(*)[]>();
    test_is_trivially_copy_constructible_cv<int(&)[3]>();
    test_is_trivially_copy_constructible_cv<int(&)[]>();
    test_is_not_trivially_copy_constructible<int(&&)[3]>();
    test_is_not_trivially_copy_constructible<int(&&)[]>();
    test_is_not_trivially_copy_constructible<char[3][2]>();
    test_is_not_trivially_copy_constructible<char[][2]>();
    test_is_not_trivially_copy_constructible<char* [3][2]>();
    test_is_not_trivially_copy_constructible<char*[][2]>();
    test_is_trivially_copy_constructible_cv<int(*)[3][2]>();
    test_is_trivially_copy_constructible_cv<int(*)[][2]>();
    test_is_trivially_copy_constructible_cv<int(&)[3][2]>();
    test_is_trivially_copy_constructible_cv<int(&)[][2]>();
    test_is_not_trivially_copy_constructible<int(&&)[3][2]>();
    test_is_not_trivially_copy_constructible<int(&&)[][2]>();
    test_is_not_trivially_copy_constructible<class_type>();
    test_is_not_trivially_copy_constructible<class_type[]>();
    test_is_not_trivially_copy_constructible<class_type[2]>();
    test_is_trivially_copy_constructible<template_type<void>>();
    test_is_trivially_copy_constructible<template_type<int>>();
    test_is_trivially_copy_constructible<template_type<class_type>>();
    test_is_trivially_copy_constructible<template_type<incomplete_type>>();
    test_is_trivially_copy_constructible<variadic_template<>>();
    test_is_trivially_copy_constructible<variadic_template<void>>();
    test_is_trivially_copy_constructible<variadic_template<int>>();
    test_is_trivially_copy_constructible<variadic_template<class_type>>();
    test_is_trivially_copy_constructible<variadic_template<incomplete_type>>();
    test_is_trivially_copy_constructible<
            variadic_template<int, void, class_type, volatile char[]>>();
    test_is_trivially_copy_constructible<public_derived_from<base>>();
    test_is_trivially_copy_constructible<public_derived_from<derived>>();
    test_is_not_trivially_copy_constructible<public_derived_from<class_type>>();
    test_is_trivially_copy_constructible<private_derived_from<base>>();
    test_is_trivially_copy_constructible<private_derived_from<derived>>();
    test_is_not_trivially_copy_constructible<private_derived_from<class_type>>();
    test_is_trivially_copy_constructible<protected_derived_from<base>>();
    test_is_trivially_copy_constructible<protected_derived_from<derived>>();
    test_is_not_trivially_copy_constructible<protected_derived_from<class_type>>();
    test_is_trivially_copy_constructible<union_type>();
    test_is_trivially_copy_constructible<non_empty_union>();
    test_is_trivially_copy_constructible<empty>();
    test_is_not_trivially_copy_constructible<not_empty>();
    test_is_trivially_copy_constructible<bit_zero>();
    test_is_trivially_copy_constructible<bit_one>();
    test_is_trivially_copy_constructible<base>();
    test_is_trivially_copy_constructible<derived>();
    test_is_not_trivially_copy_constructible<abstract>();
    test_is_not_trivially_copy_constructible<public_abstract>();
    test_is_not_trivially_copy_constructible<private_abstract>();
    test_is_not_trivially_copy_constructible<protected_abstract>();
    test_is_not_trivially_copy_constructible<abstract_template<int>>();
    test_is_trivially_copy_constructible<abstract_template<double>>();
    test_is_not_trivially_copy_constructible<abstract_template<class_type>>();
    test_is_not_trivially_copy_constructible<abstract_template<incomplete_type>>();
    test_is_trivially_copy_constructible<final_type>();
    test_is_trivially_copy_constructible<public_destructor>();
    test_is_not_trivially_copy_constructible<protected_destructor>();
    test_is_not_trivially_copy_constructible<private_destructor>();
    test_is_not_trivially_copy_constructible<virtual_public_destructor>();
    test_is_not_trivially_copy_constructible<virtual_protected_destructor>();
    test_is_not_trivially_copy_constructible<virtual_private_destructor>();
    test_is_not_trivially_copy_constructible<pure_public_destructor>();
    test_is_not_trivially_copy_constructible<pure_protected_destructor>();
    test_is_not_trivially_copy_constructible<pure_private_destructor>();
    test_is_not_trivially_copy_constructible<deleted_public_destructor>();
    test_is_not_trivially_copy_constructible<deleted_protected_destructor>();
    test_is_not_trivially_copy_constructible<deleted_private_destructor>();
    test_is_not_trivially_copy_constructible<deleted_virtual_public_destructor>();
    test_is_not_trivially_copy_constructible<deleted_virtual_protected_destructor>();
    test_is_not_trivially_copy_constructible<deleted_virtual_private_destructor>();
    test_is_trivially_copy_constructible_cv<Enum>();
    test_is_trivially_copy_constructible_cv<EnumSigned>();
    test_is_trivially_copy_constructible_cv<EnumUnsigned>();
    test_is_trivially_copy_constructible_cv<EnumClass>();
    test_is_trivially_copy_constructible_cv<EnumStruct>();
    test_is_not_trivially_copy_constructible<function_type>();
    test_is_trivially_copy_constructible_cv<function_ptr>();
    test_is_trivially_copy_constructible_cv<member_object_ptr>();
    test_is_trivially_copy_constructible_cv<member_function_ptr>();
    test_is_trivially_copy_constructible_cv<int class_type::*>();
    test_is_trivially_copy_constructible_cv<float class_type::*>();
    test_is_trivially_copy_constructible_cv<void * class_type::*>();
    test_is_trivially_copy_constructible_cv<int * class_type::*>();
    test_is_trivially_copy_constructible_cv<int class_type::*&>();
    test_is_trivially_copy_constructible_cv<float class_type::*&>();
    test_is_trivially_copy_constructible_cv<void * class_type::*&>();
    test_is_trivially_copy_constructible_cv<int * class_type::*&>();
    test_is_not_trivially_copy_constructible<int class_type::*&&>();
    test_is_not_trivially_copy_constructible<float class_type::*&&>();
    test_is_not_trivially_copy_constructible<void * class_type::*&&>();
    test_is_not_trivially_copy_constructible<int * class_type::*&&>();
    test_is_trivially_copy_constructible_cv<int class_type::*const>();
    test_is_trivially_copy_constructible_cv<float class_type::*const>();
    test_is_trivially_copy_constructible_cv<void * class_type::*const>();
    test_is_trivially_copy_constructible_cv<int class_type::*const&>();
    test_is_trivially_copy_constructible_cv<float class_type::*const&>();
    test_is_trivially_copy_constructible_cv<void * class_type::*const&>();
    test_is_not_trivially_copy_constructible<int class_type::*const&&>();
    test_is_not_trivially_copy_constructible<float class_type::*const&&>();
    test_is_not_trivially_copy_constructible<void * class_type::*const&&>();
    test_is_trivially_copy_constructible_cv<int class_type::*volatile>();
    test_is_trivially_copy_constructible_cv<float class_type::*volatile>();
    test_is_trivially_copy_constructible_cv<void * class_type::*volatile>();
    test_is_trivially_copy_constructible_cv<int class_type::*volatile&>();
    test_is_trivially_copy_constructible_cv<float class_type::*volatile&>();
    test_is_trivially_copy_constructible_cv<void * class_type::*volatile&>();
    test_is_not_trivially_copy_constructible<int class_type::*volatile&&>();
    test_is_not_trivially_copy_constructible<float class_type::*volatile&&>();
    test_is_not_trivially_copy_constructible<void * class_type::*volatile&&>();
    test_is_trivially_copy_constructible_cv<int class_type::*const volatile>();
    test_is_trivially_copy_constructible_cv<float class_type::*const volatile>();
    test_is_trivially_copy_constructible_cv<void * class_type::*const volatile>();
    test_is_trivially_copy_constructible_cv<int class_type::*const volatile&>();
    test_is_trivially_copy_constructible_cv<float class_type::*const volatile&>();
    test_is_trivially_copy_constructible_cv<void * class_type::*const volatile&>();
    test_is_not_trivially_copy_constructible<int class_type::*const volatile&&>();
    test_is_not_trivially_copy_constructible<float class_type::*const volatile&&>();
    test_is_not_trivially_copy_constructible<void * class_type::*const volatile&&>();
    test_is_not_trivially_copy_constructible<non_copyable>();
    test_is_trivially_copy_constructible<non_moveable>();
    test_is_not_trivially_copy_constructible<non_constructible>();
    test_is_not_trivially_copy_constructible<tracked>();
    test_is_trivially_copy_constructible<trap_constructible>();
    test_is_trivially_copy_constructible<trap_implicit_conversion>();
    test_is_trivially_copy_constructible<trap_comma>();
    test_is_trivially_copy_constructible<trap_call>();
    test_is_trivially_copy_constructible<trap_self_assign>();
    test_is_trivially_copy_constructible<trap_deref>();
    test_is_trivially_copy_constructible<trap_array_subscript>();

    test_is_not_trivially_copy_constructible<void()>();
    test_is_not_trivially_copy_constructible<void()&>();
    test_is_not_trivially_copy_constructible<void() &&>();
    test_is_not_trivially_copy_constructible<void() const>();
    test_is_not_trivially_copy_constructible<void() const&>();
    test_is_not_trivially_copy_constructible<void() const&&>();
    test_is_not_trivially_copy_constructible<void() volatile>();
    test_is_not_trivially_copy_constructible<void() volatile&>();
    test_is_not_trivially_copy_constructible<void() volatile&&>();
    test_is_not_trivially_copy_constructible<void() const volatile>();
    test_is_not_trivially_copy_constructible<void() const volatile&>();
    test_is_not_trivially_copy_constructible<void() const volatile&&>();
    test_is_not_trivially_copy_constructible<void() noexcept>();
    test_is_not_trivially_copy_constructible<void()& noexcept>();
    test_is_not_trivially_copy_constructible<void()&& noexcept>();
    test_is_not_trivially_copy_constructible<void() const noexcept>();
    test_is_not_trivially_copy_constructible<void() const& noexcept>();
    test_is_not_trivially_copy_constructible<void() const&& noexcept>();
    test_is_not_trivially_copy_constructible<void() volatile noexcept>();
    test_is_not_trivially_copy_constructible<void() volatile& noexcept>();
    test_is_not_trivially_copy_constructible<void() volatile&& noexcept>();
    test_is_not_trivially_copy_constructible<void() const volatile noexcept>();
    test_is_not_trivially_copy_constructible<void() const volatile& noexcept>();
    test_is_not_trivially_copy_constructible<void() const volatile&& noexcept>();

    test_is_not_trivially_copy_constructible<void(int)>();
    test_is_not_trivially_copy_constructible<void(int)&>();
    test_is_not_trivially_copy_constructible<void(int) &&>();
    test_is_not_trivially_copy_constructible<void(int) const>();
    test_is_not_trivially_copy_constructible<void(int) const&>();
    test_is_not_trivially_copy_constructible<void(int) const&&>();
    test_is_not_trivially_copy_constructible<void(int) volatile>();
    test_is_not_trivially_copy_constructible<void(int) volatile&>();
    test_is_not_trivially_copy_constructible<void(int) volatile&&>();
    test_is_not_trivially_copy_constructible<void(int) const volatile>();
    test_is_not_trivially_copy_constructible<void(int) const volatile&>();
    test_is_not_trivially_copy_constructible<void(int) const volatile&&>();
    test_is_not_trivially_copy_constructible<void(int) noexcept>();
    test_is_not_trivially_copy_constructible<void(int)& noexcept>();
    test_is_not_trivially_copy_constructible<void(int)&& noexcept>();
    test_is_not_trivially_copy_constructible<void(int) const noexcept>();
    test_is_not_trivially_copy_constructible<void(int) const& noexcept>();
    test_is_not_trivially_copy_constructible<void(int) const&& noexcept>();
    test_is_not_trivially_copy_constructible<void(int) volatile noexcept>();
    test_is_not_trivially_copy_constructible<void(int) volatile& noexcept>();
    test_is_not_trivially_copy_constructible<void(int) volatile&& noexcept>();
    test_is_not_trivially_copy_constructible<void(int) const volatile noexcept>();
    test_is_not_trivially_copy_constructible<void(int) const volatile& noexcept>();
    test_is_not_trivially_copy_constructible<void(int) const volatile&& noexcept>();

    test_is_not_trivially_copy_constructible<void(...)>();
    test_is_not_trivially_copy_constructible<void(...)&>();
    test_is_not_trivially_copy_constructible<void(...) &&>();
    test_is_not_trivially_copy_constructible<void(...) const>();
    test_is_not_trivially_copy_constructible<void(...) const&>();
    test_is_not_trivially_copy_constructible<void(...) const&&>();
    test_is_not_trivially_copy_constructible<void(...) volatile>();
    test_is_not_trivially_copy_constructible<void(...) volatile&>();
    test_is_not_trivially_copy_constructible<void(...) volatile&&>();
    test_is_not_trivially_copy_constructible<void(...) const volatile>();
    test_is_not_trivially_copy_constructible<void(...) const volatile&>();
    test_is_not_trivially_copy_constructible<void(...) const volatile&&>();
    test_is_not_trivially_copy_constructible<void(...) noexcept>();
    test_is_not_trivially_copy_constructible<void(...)& noexcept>();
    test_is_not_trivially_copy_constructible<void(...)&& noexcept>();
    test_is_not_trivially_copy_constructible<void(...) const noexcept>();
    test_is_not_trivially_copy_constructible<void(...) const& noexcept>();
    test_is_not_trivially_copy_constructible<void(...) const&& noexcept>();
    test_is_not_trivially_copy_constructible<void(...) volatile noexcept>();
    test_is_not_trivially_copy_constructible<void(...) volatile& noexcept>();
    test_is_not_trivially_copy_constructible<void(...) volatile&& noexcept>();
    test_is_not_trivially_copy_constructible<void(...) const volatile noexcept>();
    test_is_not_trivially_copy_constructible<void(...) const volatile& noexcept>();
    test_is_not_trivially_copy_constructible<void(...) const volatile&& noexcept>();

    test_is_not_trivially_copy_constructible<void(int, ...)>();
    test_is_not_trivially_copy_constructible<void(int, ...)&>();
    test_is_not_trivially_copy_constructible<void(int, ...) &&>();
    test_is_not_trivially_copy_constructible<void(int, ...) const>();
    test_is_not_trivially_copy_constructible<void(int, ...) const&>();
    test_is_not_trivially_copy_constructible<void(int, ...) const&&>();
    test_is_not_trivially_copy_constructible<void(int, ...) volatile>();
    test_is_not_trivially_copy_constructible<void(int, ...) volatile&>();
    test_is_not_trivially_copy_constructible<void(int, ...) volatile&&>();
    test_is_not_trivially_copy_constructible<void(int, ...) const volatile>();
    test_is_not_trivially_copy_constructible<void(int, ...) const volatile&>();
    test_is_not_trivially_copy_constructible<void(int, ...) const volatile&&>();
    test_is_not_trivially_copy_constructible<void(int, ...) noexcept>();
    test_is_not_trivially_copy_constructible<void(int, ...)& noexcept>();
    test_is_not_trivially_copy_constructible<void(int, ...)&& noexcept>();
    test_is_not_trivially_copy_constructible<void(int, ...) const noexcept>();
    test_is_not_trivially_copy_constructible<void(int, ...) const& noexcept>();
    test_is_not_trivially_copy_constructible<void(int, ...) const&& noexcept>();
    test_is_not_trivially_copy_constructible<void(int, ...) volatile noexcept>();
    test_is_not_trivially_copy_constructible<void(int, ...) volatile& noexcept>();
    test_is_not_trivially_copy_constructible<void(int, ...) volatile&& noexcept>();
    test_is_not_trivially_copy_constructible<void(int, ...) const volatile noexcept>();
    test_is_not_trivially_copy_constructible<void(int, ...) const volatile& noexcept>();
    test_is_not_trivially_copy_constructible<void(int, ...) const volatile&& noexcept>();

    test_is_not_trivially_copy_constructible<int()>();
    test_is_not_trivially_copy_constructible<int()&>();
    test_is_not_trivially_copy_constructible<int() &&>();
    test_is_not_trivially_copy_constructible<int() const>();
    test_is_not_trivially_copy_constructible<int() const&>();
    test_is_not_trivially_copy_constructible<int() const&&>();
    test_is_not_trivially_copy_constructible<int() volatile>();
    test_is_not_trivially_copy_constructible<int() volatile&>();
    test_is_not_trivially_copy_constructible<int() volatile&&>();
    test_is_not_trivially_copy_constructible<int() const volatile>();
    test_is_not_trivially_copy_constructible<int() const volatile&>();
    test_is_not_trivially_copy_constructible<int() const volatile&&>();
    test_is_not_trivially_copy_constructible<int() noexcept>();
    test_is_not_trivially_copy_constructible<int()& noexcept>();
    test_is_not_trivially_copy_constructible<int()&& noexcept>();
    test_is_not_trivially_copy_constructible<int() const noexcept>();
    test_is_not_trivially_copy_constructible<int() const& noexcept>();
    test_is_not_trivially_copy_constructible<int() const&& noexcept>();
    test_is_not_trivially_copy_constructible<int() volatile noexcept>();
    test_is_not_trivially_copy_constructible<int() volatile& noexcept>();
    test_is_not_trivially_copy_constructible<int() volatile&& noexcept>();
    test_is_not_trivially_copy_constructible<int() const volatile noexcept>();
    test_is_not_trivially_copy_constructible<int() const volatile& noexcept>();
    test_is_not_trivially_copy_constructible<int() const volatile&& noexcept>();

    test_is_not_trivially_copy_constructible<int(int)>();
    test_is_not_trivially_copy_constructible<int(int)&>();
    test_is_not_trivially_copy_constructible<int(int) &&>();
    test_is_not_trivially_copy_constructible<int(int) const>();
    test_is_not_trivially_copy_constructible<int(int) const&>();
    test_is_not_trivially_copy_constructible<int(int) const&&>();
    test_is_not_trivially_copy_constructible<int(int) volatile>();
    test_is_not_trivially_copy_constructible<int(int) volatile&>();
    test_is_not_trivially_copy_constructible<int(int) volatile&&>();
    test_is_not_trivially_copy_constructible<int(int) const volatile>();
    test_is_not_trivially_copy_constructible<int(int) const volatile&>();
    test_is_not_trivially_copy_constructible<int(int) const volatile&&>();
    test_is_not_trivially_copy_constructible<int(int) noexcept>();
    test_is_not_trivially_copy_constructible<int(int)& noexcept>();
    test_is_not_trivially_copy_constructible<int(int)&& noexcept>();
    test_is_not_trivially_copy_constructible<int(int) const noexcept>();
    test_is_not_trivially_copy_constructible<int(int) const& noexcept>();
    test_is_not_trivially_copy_constructible<int(int) const&& noexcept>();
    test_is_not_trivially_copy_constructible<int(int) volatile noexcept>();
    test_is_not_trivially_copy_constructible<int(int) volatile& noexcept>();
    test_is_not_trivially_copy_constructible<int(int) volatile&& noexcept>();
    test_is_not_trivially_copy_constructible<int(int) const volatile noexcept>();
    test_is_not_trivially_copy_constructible<int(int) const volatile& noexcept>();
    test_is_not_trivially_copy_constructible<int(int) const volatile&& noexcept>();

    test_is_not_trivially_copy_constructible<int(...)>();
    test_is_not_trivially_copy_constructible<int(...)&>();
    test_is_not_trivially_copy_constructible<int(...) &&>();
    test_is_not_trivially_copy_constructible<int(...) const>();
    test_is_not_trivially_copy_constructible<int(...) const&>();
    test_is_not_trivially_copy_constructible<int(...) const&&>();
    test_is_not_trivially_copy_constructible<int(...) volatile>();
    test_is_not_trivially_copy_constructible<int(...) volatile&>();
    test_is_not_trivially_copy_constructible<int(...) volatile&&>();
    test_is_not_trivially_copy_constructible<int(...) const volatile>();
    test_is_not_trivially_copy_constructible<int(...) const volatile&>();
    test_is_not_trivially_copy_constructible<int(...) const volatile&&>();
    test_is_not_trivially_copy_constructible<int(...) noexcept>();
    test_is_not_trivially_copy_constructible<int(...)& noexcept>();
    test_is_not_trivially_copy_constructible<int(...)&& noexcept>();
    test_is_not_trivially_copy_constructible<int(...) const noexcept>();
    test_is_not_trivially_copy_constructible<int(...) const& noexcept>();
    test_is_not_trivially_copy_constructible<int(...) const&& noexcept>();
    test_is_not_trivially_copy_constructible<int(...) volatile noexcept>();
    test_is_not_trivially_copy_constructible<int(...) volatile& noexcept>();
    test_is_not_trivially_copy_constructible<int(...) volatile&& noexcept>();
    test_is_not_trivially_copy_constructible<int(...) const volatile noexcept>();
    test_is_not_trivially_copy_constructible<int(...) const volatile& noexcept>();
    test_is_not_trivially_copy_constructible<int(...) const volatile&& noexcept>();

    test_is_not_trivially_copy_constructible<int(int, ...)>();
    test_is_not_trivially_copy_constructible<int(int, ...)&>();
    test_is_not_trivially_copy_constructible<int(int, ...) &&>();
    test_is_not_trivially_copy_constructible<int(int, ...) const>();
    test_is_not_trivially_copy_constructible<int(int, ...) const&>();
    test_is_not_trivially_copy_constructible<int(int, ...) const&&>();
    test_is_not_trivially_copy_constructible<int(int, ...) volatile>();
    test_is_not_trivially_copy_constructible<int(int, ...) volatile&>();
    test_is_not_trivially_copy_constructible<int(int, ...) volatile&&>();
    test_is_not_trivially_copy_constructible<int(int, ...) const volatile>();
    test_is_not_trivially_copy_constructible<int(int, ...) const volatile&>();
    test_is_not_trivially_copy_constructible<int(int, ...) const volatile&&>();
    test_is_not_trivially_copy_constructible<int(int, ...) noexcept>();
    test_is_not_trivially_copy_constructible<int(int, ...)& noexcept>();
    test_is_not_trivially_copy_constructible<int(int, ...)&& noexcept>();
    test_is_not_trivially_copy_constructible<int(int, ...) const noexcept>();
    test_is_not_trivially_copy_constructible<int(int, ...) const& noexcept>();
    test_is_not_trivially_copy_constructible<int(int, ...) const&& noexcept>();
    test_is_not_trivially_copy_constructible<int(int, ...) volatile noexcept>();
    test_is_not_trivially_copy_constructible<int(int, ...) volatile& noexcept>();
    test_is_not_trivially_copy_constructible<int(int, ...) volatile&& noexcept>();
    test_is_not_trivially_copy_constructible<int(int, ...) const volatile noexcept>();
    test_is_not_trivially_copy_constructible<int(int, ...) const volatile& noexcept>();
    test_is_not_trivially_copy_constructible<int(int, ...) const volatile&& noexcept>();

    test_is_trivially_copy_constructible_cv<void (*)()>();
    test_is_trivially_copy_constructible_cv<void (*)() noexcept>();

    test_is_trivially_copy_constructible_cv<void (*)(int)>();
    test_is_trivially_copy_constructible_cv<void (*)(int) noexcept>();

    test_is_trivially_copy_constructible_cv<void (*)(...)>();
    test_is_trivially_copy_constructible_cv<void (*)(...) noexcept>();

    test_is_trivially_copy_constructible_cv<void (*)(int, ...)>();
    test_is_trivially_copy_constructible_cv<void (*)(int, ...) noexcept>();

    test_is_trivially_copy_constructible_cv<int (*)()>();
    test_is_trivially_copy_constructible_cv<int (*)() noexcept>();

    test_is_trivially_copy_constructible_cv<int (*)(int)>();
    test_is_trivially_copy_constructible_cv<int (*)(int) noexcept>();

    test_is_trivially_copy_constructible_cv<int (*)(...)>();
    test_is_trivially_copy_constructible_cv<int (*)(...) noexcept>();

    test_is_trivially_copy_constructible_cv<int (*)(int, ...)>();
    test_is_trivially_copy_constructible_cv<int (*)(int, ...) noexcept>();

    test_is_trivially_copy_constructible_cv<void (&)()>();
    test_is_trivially_copy_constructible_cv<void (&)() noexcept>();

    test_is_trivially_copy_constructible_cv<void (&)(int)>();
    test_is_trivially_copy_constructible_cv<void (&)(int) noexcept>();

    test_is_trivially_copy_constructible_cv<void (&)(...)>();
    test_is_trivially_copy_constructible_cv<void (&)(...) noexcept>();

    test_is_trivially_copy_constructible_cv<void (&)(int, ...)>();
    test_is_trivially_copy_constructible_cv<void (&)(int, ...) noexcept>();

    test_is_trivially_copy_constructible_cv<int (&)()>();
    test_is_trivially_copy_constructible_cv<int (&)() noexcept>();

    test_is_trivially_copy_constructible_cv<int (&)(int)>();
    test_is_trivially_copy_constructible_cv<int (&)(int) noexcept>();

    test_is_trivially_copy_constructible_cv<int (&)(...)>();
    test_is_trivially_copy_constructible_cv<int (&)(...) noexcept>();

    test_is_trivially_copy_constructible_cv<int (&)(int, ...)>();
    test_is_trivially_copy_constructible_cv<int (&)(int, ...) noexcept>();

    test_is_trivially_copy_constructible_cv<void(&&)()>();
    test_is_trivially_copy_constructible_cv<void(&&)() noexcept>();

    test_is_trivially_copy_constructible_cv<void(&&)(int)>();
    test_is_trivially_copy_constructible_cv<void(&&)(int) noexcept>();

    test_is_trivially_copy_constructible_cv<void(&&)(...)>();
    test_is_trivially_copy_constructible_cv<void(&&)(...) noexcept>();

    test_is_trivially_copy_constructible_cv<void(&&)(int, ...)>();
    test_is_trivially_copy_constructible_cv<void(&&)(int, ...) noexcept>();

    test_is_trivially_copy_constructible_cv<int(&&)()>();
    test_is_trivially_copy_constructible_cv<int(&&)() noexcept>();

    test_is_trivially_copy_constructible_cv<int(&&)(int)>();
    test_is_trivially_copy_constructible_cv<int(&&)(int) noexcept>();

    test_is_trivially_copy_constructible_cv<int(&&)(...)>();
    test_is_trivially_copy_constructible_cv<int(&&)(...) noexcept>();

    test_is_trivially_copy_constructible_cv<int(&&)(int, ...)>();
    test_is_trivially_copy_constructible_cv<int(&&)(int, ...) noexcept>();

    test_is_trivially_copy_constructible_cv<void (class_type::*)()>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)()&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)() &&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)() const>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)() const&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)() const&&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)() noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)()& noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)()&& noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)() const noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)() const& noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)() const&& noexcept>();

    test_is_trivially_copy_constructible_cv<void (class_type::*)(int)>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int)&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int) &&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int) const>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int) const&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int) const&&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int) noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int)& noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int)&& noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int) const noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int) const& noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int) const&& noexcept>();

    test_is_trivially_copy_constructible_cv<void (class_type::*)(...)>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(...)&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(...) &&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(...) const>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(...) const&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(...) const&&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(...) noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(...)& noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(...)&& noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(...) const noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(...) const& noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(...) const&& noexcept>();

    test_is_trivially_copy_constructible_cv<void (class_type::*)(int, ...)>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int, ...)&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int, ...) &&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int, ...) const>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int, ...) const&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int, ...) const&&>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int, ...) noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int, ...)& noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int, ...)&& noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int, ...) const noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int, ...) const& noexcept>();
    test_is_trivially_copy_constructible_cv<void (class_type::*)(int, ...) const&& noexcept>();

    test_is_trivially_copy_constructible_cv<int (class_type::*)()>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)()&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)() &&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)() const>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)() const&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)() const&&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)() noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)()& noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)()&& noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)() const noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)() const& noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)() const&& noexcept>();

    test_is_trivially_copy_constructible_cv<int (class_type::*)(int)>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int)&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int) &&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int) const>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int) const&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int) const&&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int) noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int)& noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int)&& noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int) const noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int) const& noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int) const&& noexcept>();

    test_is_trivially_copy_constructible_cv<int (class_type::*)(...)>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(...)&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(...) &&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(...) const>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(...) const&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(...) const&&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(...) noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(...)& noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(...)&& noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(...) const noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(...) const& noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(...) const&& noexcept>();

    test_is_trivially_copy_constructible_cv<int (class_type::*)(int, ...)>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int, ...)&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int, ...) &&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int, ...) const>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int, ...) const&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int, ...) const&&>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int, ...) noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int, ...)& noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int, ...)&& noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int, ...) const noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int, ...) const& noexcept>();
    test_is_trivially_copy_constructible_cv<int (class_type::*)(int, ...) const&& noexcept>();
}
