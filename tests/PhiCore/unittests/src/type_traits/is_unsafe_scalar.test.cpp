#include <phi/test/test_macros.hpp>

#include "test_types.hpp"
#include "type_traits_helper.hpp"
#include <phi/compiler_support/char8_t.hpp>
#include <phi/core/boolean.hpp>
#include <phi/core/floating_point.hpp>
#include <phi/core/integer.hpp>
#include <phi/core/nullptr_t.hpp>
#include <phi/core/scope_ptr.hpp>
#include <phi/type_traits/is_unsafe_scalar.hpp>
#include <type_traits>
#include <vector>

template <typename T>
void test_is_unsafe_scalar_impl()
{
#if PHI_HAS_WORKING_IS_UNSAFE_SCALAR()
    STATIC_REQUIRE(phi::is_unsafe_scalar<T>::value);
    STATIC_REQUIRE_FALSE(phi::is_not_unsafe_scalar<T>::value);

#    if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()
    STATIC_REQUIRE(phi::is_unsafe_scalar_v<T>);
    STATIC_REQUIRE_FALSE(phi::is_not_unsafe_scalar_v<T>);
#    endif

    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_unsafe_scalar<T>);
    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_not_unsafe_scalar<T>);

    // Standard compatibility
    STATIC_REQUIRE(std::is_scalar<T>::value);
#endif
}

template <typename T>
void test_is_not_unsafe_scalar_impl()
{
#if PHI_HAS_WORKING_IS_UNSAFE_SCALAR()
    STATIC_REQUIRE_FALSE(phi::is_unsafe_scalar<T>::value);
    STATIC_REQUIRE(phi::is_not_unsafe_scalar<T>::value);

#    if PHI_HAS_FEATURE_VARIABLE_TEMPLATE()
    STATIC_REQUIRE_FALSE(phi::is_unsafe_scalar_v<T>);
    STATIC_REQUIRE(phi::is_not_unsafe_scalar_v<T>);
#    endif

    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_unsafe_scalar<T>);
    TEST_TYPE_TRAITS_TYPE_DEFS(phi::is_not_unsafe_scalar<T>);

    // Standard compatibility
    STATIC_REQUIRE_FALSE(std::is_scalar<T>::value);
#endif
}

template <typename T>
void test_is_unsafe_scalar()
{
    test_is_unsafe_scalar_impl<T>();
    test_is_unsafe_scalar_impl<const T>();
    test_is_unsafe_scalar_impl<volatile T>();
    test_is_unsafe_scalar_impl<const volatile T>();
}

template <typename T>
void test_is_not_unsafe_scalar()
{
    test_is_not_unsafe_scalar_impl<T>();
    test_is_not_unsafe_scalar_impl<const T>();
    test_is_not_unsafe_scalar_impl<volatile T>();
    test_is_not_unsafe_scalar_impl<const volatile T>();
}

TEST_CASE("is_unsafe_scalar")
{
    test_is_not_unsafe_scalar<void>();
    test_is_unsafe_scalar<phi::nullptr_t>();
    test_is_unsafe_scalar<bool>();
    test_is_unsafe_scalar<char>();
    test_is_unsafe_scalar<signed char>();
    test_is_unsafe_scalar<unsigned char>();
    test_is_unsafe_scalar<short>();
    test_is_unsafe_scalar<unsigned short>();
    test_is_unsafe_scalar<int>();
    test_is_unsafe_scalar<unsigned int>();
    test_is_unsafe_scalar<long>();
    test_is_unsafe_scalar<unsigned long>();
    test_is_unsafe_scalar<long long>();
    test_is_unsafe_scalar<unsigned long long>();
    test_is_unsafe_scalar<float>();
    test_is_unsafe_scalar<double>();
    test_is_unsafe_scalar<long double>();
    test_is_unsafe_scalar<char8_t>();
    test_is_unsafe_scalar<char16_t>();
    test_is_unsafe_scalar<char32_t>();
    test_is_unsafe_scalar<wchar_t>();

    test_is_not_unsafe_scalar<phi::boolean>();
    test_is_not_unsafe_scalar<phi::integer<signed char>>();
    test_is_not_unsafe_scalar<phi::integer<unsigned char>>();
    test_is_not_unsafe_scalar<phi::integer<short>>();
    test_is_not_unsafe_scalar<phi::integer<unsigned short>>();
    test_is_not_unsafe_scalar<phi::integer<int>>();
    test_is_not_unsafe_scalar<phi::integer<unsigned int>>();
    test_is_not_unsafe_scalar<phi::integer<long>>();
    test_is_not_unsafe_scalar<phi::integer<unsigned long>>();
    test_is_not_unsafe_scalar<phi::integer<long long>>();
    test_is_not_unsafe_scalar<phi::integer<unsigned long long>>();
    test_is_not_unsafe_scalar<phi::floating_point<float>>();
    test_is_not_unsafe_scalar<phi::floating_point<double>>();
    test_is_not_unsafe_scalar<phi::floating_point<long double>>();

    test_is_not_unsafe_scalar<std::vector<int>>();
    test_is_not_unsafe_scalar<phi::scope_ptr<int>>();

    test_is_not_unsafe_scalar<int&>();
    test_is_not_unsafe_scalar<const int&>();
    test_is_not_unsafe_scalar<volatile int&>();
    test_is_not_unsafe_scalar<const volatile int&>();
    test_is_not_unsafe_scalar<int&&>();
    test_is_not_unsafe_scalar<const int&&>();
    test_is_not_unsafe_scalar<volatile int&&>();
    test_is_not_unsafe_scalar<const volatile int&&>();
    test_is_unsafe_scalar<int*>();
    test_is_unsafe_scalar<const int*>();
    test_is_unsafe_scalar<volatile int*>();
    test_is_unsafe_scalar<const volatile int*>();
    test_is_unsafe_scalar<int**>();
    test_is_unsafe_scalar<const int**>();
    test_is_unsafe_scalar<volatile int**>();
    test_is_unsafe_scalar<const volatile int**>();
    test_is_not_unsafe_scalar<int*&>();
    test_is_not_unsafe_scalar<const int*&>();
    test_is_not_unsafe_scalar<volatile int*&>();
    test_is_not_unsafe_scalar<const volatile int*&>();
    test_is_not_unsafe_scalar<int*&&>();
    test_is_not_unsafe_scalar<const int*&&>();
    test_is_not_unsafe_scalar<volatile int*&&>();
    test_is_not_unsafe_scalar<const volatile int*&&>();
    test_is_unsafe_scalar<void*>();
    test_is_not_unsafe_scalar<char[3]>();
    test_is_not_unsafe_scalar<char[]>();
    test_is_not_unsafe_scalar<char* [3]>();
    test_is_not_unsafe_scalar<char*[]>();
    test_is_unsafe_scalar<int(*)[3]>();
    test_is_unsafe_scalar<int(*)[]>();
    test_is_not_unsafe_scalar<int(&)[3]>();
    test_is_not_unsafe_scalar<int(&)[]>();
    test_is_not_unsafe_scalar<int(&&)[3]>();
    test_is_not_unsafe_scalar<int(&&)[]>();
    test_is_not_unsafe_scalar<char[3][2]>();
    test_is_not_unsafe_scalar<char[][2]>();
    test_is_not_unsafe_scalar<char* [3][2]>();
    test_is_not_unsafe_scalar<char*[][2]>();
    test_is_unsafe_scalar<int(*)[3][2]>();
    test_is_unsafe_scalar<int(*)[][2]>();
    test_is_not_unsafe_scalar<int(&)[3][2]>();
    test_is_not_unsafe_scalar<int(&)[][2]>();
    test_is_not_unsafe_scalar<int(&&)[3][2]>();
    test_is_not_unsafe_scalar<int(&&)[][2]>();
    test_is_not_unsafe_scalar<class_type>();
    test_is_not_unsafe_scalar<class_type[]>();
    test_is_not_unsafe_scalar<class_type[2]>();
    test_is_not_unsafe_scalar<template_type<void>>();
    test_is_not_unsafe_scalar<template_type<int>>();
    test_is_not_unsafe_scalar<template_type<class_type>>();
    test_is_not_unsafe_scalar<template_type<incomplete_type>>();
    test_is_not_unsafe_scalar<variadic_template<>>();
    test_is_not_unsafe_scalar<variadic_template<void>>();
    test_is_not_unsafe_scalar<variadic_template<int>>();
    test_is_not_unsafe_scalar<variadic_template<class_type>>();
    test_is_not_unsafe_scalar<variadic_template<incomplete_type>>();
    test_is_not_unsafe_scalar<variadic_template<int, void, class_type, volatile char[]>>();
    test_is_not_unsafe_scalar<public_derived_from<base>>();
    test_is_not_unsafe_scalar<public_derived_from<derived>>();
    test_is_not_unsafe_scalar<public_derived_from<class_type>>();
    test_is_not_unsafe_scalar<private_derived_from<base>>();
    test_is_not_unsafe_scalar<private_derived_from<derived>>();
    test_is_not_unsafe_scalar<private_derived_from<class_type>>();
    test_is_not_unsafe_scalar<protected_derived_from<base>>();
    test_is_not_unsafe_scalar<protected_derived_from<derived>>();
    test_is_not_unsafe_scalar<protected_derived_from<class_type>>();
    test_is_not_unsafe_scalar<virtual_derived_from<base>>();
    test_is_not_unsafe_scalar<virtual_derived_from<derived>>();
    test_is_not_unsafe_scalar<virtual_derived_from<class_type>>();
    test_is_not_unsafe_scalar<union_type>();
    test_is_not_unsafe_scalar<non_empty_union>();
    test_is_not_unsafe_scalar<non_trivial_union>();
    test_is_not_unsafe_scalar<empty>();
    test_is_not_unsafe_scalar<not_empty>();
    test_is_not_unsafe_scalar<non_trivial>();
    test_is_not_unsafe_scalar<bit_zero>();
    test_is_not_unsafe_scalar<bit_one>();
    test_is_not_unsafe_scalar<base>();
    test_is_not_unsafe_scalar<derived>();
    test_is_not_unsafe_scalar<polymorphic>();
    test_is_not_unsafe_scalar<derived_polymorphic>();
    test_is_not_unsafe_scalar<abstract>();
    test_is_not_unsafe_scalar<public_abstract>();
    test_is_not_unsafe_scalar<private_abstract>();
    test_is_not_unsafe_scalar<protected_abstract>();
    test_is_not_unsafe_scalar<abstract_template<int>>();
    test_is_not_unsafe_scalar<abstract_template<double>>();
    test_is_not_unsafe_scalar<abstract_template<class_type>>();
    test_is_not_unsafe_scalar<abstract_template<incomplete_type>>();
    test_is_not_unsafe_scalar<public_abstract_deleted_destructor>();
    test_is_not_unsafe_scalar<protected_abstract_deleted_destructor>();
    test_is_not_unsafe_scalar<private_abstract_deleted_destructor>();
    test_is_not_unsafe_scalar<final_type>();
    test_is_not_unsafe_scalar<final_derived>();
    test_is_not_unsafe_scalar<public_destructor>();
    test_is_not_unsafe_scalar<protected_destructor>();
    test_is_not_unsafe_scalar<private_destructor>();
    test_is_not_unsafe_scalar<virtual_public_destructor>();
    test_is_not_unsafe_scalar<virtual_protected_destructor>();
    test_is_not_unsafe_scalar<virtual_private_destructor>();
    test_is_not_unsafe_scalar<pure_public_destructor>();
    test_is_not_unsafe_scalar<pure_protected_destructor>();
    test_is_not_unsafe_scalar<pure_private_destructor>();
    test_is_not_unsafe_scalar<deleted_public_destructor>();
    test_is_not_unsafe_scalar<deleted_protected_destructor>();
    test_is_not_unsafe_scalar<deleted_private_destructor>();
    test_is_not_unsafe_scalar<deleted_virtual_public_destructor>();
    test_is_not_unsafe_scalar<deleted_virtual_protected_destructor>();
    test_is_not_unsafe_scalar<deleted_virtual_private_destructor>();
    test_is_not_unsafe_scalar<explicit_class>();
    test_is_not_unsafe_scalar<nothrow_explicit_class>();
    test_is_not_unsafe_scalar<throw_explicit_class>();
    test_is_not_unsafe_scalar<throw_default_class>();
    test_is_not_unsafe_scalar<throw_copy_constructible_class>();
    test_is_not_unsafe_scalar<throw_move_constructible_class>();
    test_is_not_unsafe_scalar<noexcept_explicit_class>();
    test_is_not_unsafe_scalar<except_explicit_class>();
    test_is_not_unsafe_scalar<noexcept_default_class>();
    test_is_not_unsafe_scalar<except_default_class>();
    test_is_not_unsafe_scalar<noexcept_copy_constructible_class>();
    test_is_not_unsafe_scalar<except_copy_constructible_class>();
    test_is_not_unsafe_scalar<noexcept_move_constructible_class>();
    test_is_not_unsafe_scalar<except_move_constructible_class>();
    test_is_not_unsafe_scalar<noexcept_copy_assign_class>();
    test_is_not_unsafe_scalar<except_copy_assign_class>();
    test_is_not_unsafe_scalar<noexcept_move_assign_class>();
    test_is_not_unsafe_scalar<except_move_assign_class>();
    test_is_not_unsafe_scalar<deleted_copy_assign_class>();
    test_is_not_unsafe_scalar<deleted_move_assign_class>();
    test_is_not_unsafe_scalar<noexcept_move_constructible_and_assignable_class>();
    test_is_not_unsafe_scalar<except_move_constructible_noexcept_move_assign_class>();
    test_is_not_unsafe_scalar<noexcept_move_constructible_except_move_assign_class>();
    test_is_not_unsafe_scalar<except_move_constructible_and_assign_class>();
    test_is_not_unsafe_scalar<implicit_to<int>>();
    test_is_not_unsafe_scalar<implicit_to<float>>();
    test_is_not_unsafe_scalar<implicit_to<class_type>>();
    test_is_not_unsafe_scalar<deleted_implicit_to<int>>();
    test_is_not_unsafe_scalar<deleted_implicit_to<float>>();
    test_is_not_unsafe_scalar<deleted_implicit_to<class_type>>();
    test_is_not_unsafe_scalar<explicit_to<int>>();
    test_is_not_unsafe_scalar<explicit_to<float>>();
    test_is_not_unsafe_scalar<explicit_to<class_type>>();
    test_is_not_unsafe_scalar<deleted_explicit_to<int>>();
    test_is_not_unsafe_scalar<deleted_explicit_to<float>>();
    test_is_not_unsafe_scalar<deleted_explicit_to<class_type>>();
    test_is_not_unsafe_scalar<ellipsis>();
    test_is_not_unsafe_scalar<deleted_ellipsis>();
    test_is_not_unsafe_scalar<copy_constructible_only_type>();
    test_is_not_unsafe_scalar<move_constructible_only_type>();
    test_is_not_unsafe_scalar<overloaded_operators>();
    test_is_not_unsafe_scalar<public_int_member>();
    test_is_not_unsafe_scalar<protected_int_member>();
    test_is_not_unsafe_scalar<private_int_member>();
    test_is_not_unsafe_scalar<public_static_int_member>();
    test_is_not_unsafe_scalar<protected_static_int_member>();
    test_is_not_unsafe_scalar<private_static_int_member>();
    test_is_not_unsafe_scalar<public_template_member<int>>();
    test_is_not_unsafe_scalar<public_template_member<float>>();
    test_is_not_unsafe_scalar<public_template_member<class_type>>();
    test_is_not_unsafe_scalar<protected_template_member<int>>();
    test_is_not_unsafe_scalar<protected_template_member<float>>();
    test_is_not_unsafe_scalar<protected_template_member<class_type>>();
    test_is_not_unsafe_scalar<private_template_member<int>>();
    test_is_not_unsafe_scalar<private_template_member<float>>();
    test_is_not_unsafe_scalar<private_template_member<class_type>>();
    test_is_not_unsafe_scalar<public_static_tempalte_member<int>>();
    test_is_not_unsafe_scalar<public_static_tempalte_member<float>>();
    test_is_not_unsafe_scalar<public_static_tempalte_member<class_type>>();
    test_is_not_unsafe_scalar<protected_static_template_member<int>>();
    test_is_not_unsafe_scalar<protected_static_template_member<float>>();
    test_is_not_unsafe_scalar<protected_static_template_member<class_type>>();
    test_is_not_unsafe_scalar<private_static_template_member<int>>();
    test_is_not_unsafe_scalar<private_static_template_member<float>>();
    test_is_not_unsafe_scalar<private_static_template_member<class_type>>();
    test_is_unsafe_scalar<Enum>();
    test_is_unsafe_scalar<EnumSigned>();
    test_is_unsafe_scalar<EnumUnsigned>();
    test_is_unsafe_scalar<EnumClass>();
    test_is_unsafe_scalar<EnumStruct>();
    test_is_not_unsafe_scalar<function_type>();
    test_is_unsafe_scalar<function_ptr>();
    test_is_unsafe_scalar<member_object_ptr>();
    test_is_unsafe_scalar<member_function_ptr>();
    test_is_not_unsafe_scalar<lambda_type>();
    test_is_not_unsafe_scalar<lambda_noexcept_type>();
    test_is_not_unsafe_scalar<lambda_throws_type>();
    test_is_not_unsafe_scalar<incomplete_type>();
    test_is_not_unsafe_scalar<incomplete_template<void>>();
    test_is_not_unsafe_scalar<incomplete_template<int>>();
    test_is_not_unsafe_scalar<incomplete_template<class_type>>();
    test_is_not_unsafe_scalar<incomplete_template<incomplete_type>>();
    test_is_not_unsafe_scalar<incomplete_variadic_template<>>();
    test_is_not_unsafe_scalar<incomplete_variadic_template<void>>();
    test_is_not_unsafe_scalar<incomplete_variadic_template<int>>();
    test_is_not_unsafe_scalar<incomplete_variadic_template<class_type>>();
    test_is_not_unsafe_scalar<incomplete_variadic_template<incomplete_type>>();
    test_is_not_unsafe_scalar<
            incomplete_variadic_template<int, void, class_type, volatile char[]>>();
    test_is_not_unsafe_scalar<incomplete_union>();
    test_is_unsafe_scalar<IncompleteEnumSigned>();
    test_is_unsafe_scalar<IncompleteEnumUnsigned>();
    test_is_unsafe_scalar<IncompleteEnumClass>();
    test_is_unsafe_scalar<IncompleteEnumStruct>();
    test_is_unsafe_scalar<int class_type::*>();
    test_is_unsafe_scalar<float class_type::*>();
    test_is_unsafe_scalar<void * class_type::*>();
    test_is_unsafe_scalar<int * class_type::*>();
    test_is_not_unsafe_scalar<int class_type::*&>();
    test_is_not_unsafe_scalar<float class_type::*&>();
    test_is_not_unsafe_scalar<void * class_type::*&>();
    test_is_not_unsafe_scalar<int * class_type::*&>();
    test_is_not_unsafe_scalar<int class_type::*&&>();
    test_is_not_unsafe_scalar<float class_type::*&&>();
    test_is_not_unsafe_scalar<void * class_type::*&&>();
    test_is_not_unsafe_scalar<int * class_type::*&&>();
    test_is_unsafe_scalar<int class_type::*const>();
    test_is_unsafe_scalar<float class_type::*const>();
    test_is_unsafe_scalar<void * class_type::*const>();
    test_is_not_unsafe_scalar<int class_type::*const&>();
    test_is_not_unsafe_scalar<float class_type::*const&>();
    test_is_not_unsafe_scalar<void * class_type::*const&>();
    test_is_not_unsafe_scalar<int class_type::*const&&>();
    test_is_not_unsafe_scalar<float class_type::*const&&>();
    test_is_not_unsafe_scalar<void * class_type::*const&&>();
    test_is_unsafe_scalar<int class_type::*volatile>();
    test_is_unsafe_scalar<float class_type::*volatile>();
    test_is_unsafe_scalar<void * class_type::*volatile>();
    test_is_not_unsafe_scalar<int class_type::*volatile&>();
    test_is_not_unsafe_scalar<float class_type::*volatile&>();
    test_is_not_unsafe_scalar<void * class_type::*volatile&>();
    test_is_not_unsafe_scalar<int class_type::*volatile&&>();
    test_is_not_unsafe_scalar<float class_type::*volatile&&>();
    test_is_not_unsafe_scalar<void * class_type::*volatile&&>();
    test_is_unsafe_scalar<int class_type::*const volatile>();
    test_is_unsafe_scalar<float class_type::*const volatile>();
    test_is_unsafe_scalar<void * class_type::*const volatile>();
    test_is_not_unsafe_scalar<int class_type::*const volatile&>();
    test_is_not_unsafe_scalar<float class_type::*const volatile&>();
    test_is_not_unsafe_scalar<void * class_type::*const volatile&>();
    test_is_not_unsafe_scalar<int class_type::*const volatile&&>();
    test_is_not_unsafe_scalar<float class_type::*const volatile&&>();
    test_is_not_unsafe_scalar<void * class_type::*const volatile&&>();
    test_is_not_unsafe_scalar<non_copyable>();
    test_is_not_unsafe_scalar<non_moveable>();
    test_is_not_unsafe_scalar<non_constructible>();
    test_is_not_unsafe_scalar<non_destructible>();
    test_is_not_unsafe_scalar<tracked>();
    test_is_not_unsafe_scalar<trap_constructible>();
    test_is_not_unsafe_scalar<trap_implicit_conversion>();
    test_is_not_unsafe_scalar<trap_comma>();
    test_is_not_unsafe_scalar<trap_call>();
    test_is_not_unsafe_scalar<trap_self_assign>();
    test_is_not_unsafe_scalar<trap_deref>();
    test_is_not_unsafe_scalar<trap_array_subscript>();

    test_is_not_unsafe_scalar<void()>();
    test_is_not_unsafe_scalar<void()&>();
    test_is_not_unsafe_scalar<void() &&>();
    test_is_not_unsafe_scalar<void() const>();
    test_is_not_unsafe_scalar<void() const&>();
    test_is_not_unsafe_scalar<void() const&&>();
    test_is_not_unsafe_scalar<void() volatile>();
    test_is_not_unsafe_scalar<void() volatile&>();
    test_is_not_unsafe_scalar<void() volatile&&>();
    test_is_not_unsafe_scalar<void() const volatile>();
    test_is_not_unsafe_scalar<void() const volatile&>();
    test_is_not_unsafe_scalar<void() const volatile&&>();
    test_is_not_unsafe_scalar<void() noexcept>();
    test_is_not_unsafe_scalar<void()& noexcept>();
    test_is_not_unsafe_scalar<void()&& noexcept>();
    test_is_not_unsafe_scalar<void() const noexcept>();
    test_is_not_unsafe_scalar<void() const& noexcept>();
    test_is_not_unsafe_scalar<void() const&& noexcept>();
    test_is_not_unsafe_scalar<void() volatile noexcept>();
    test_is_not_unsafe_scalar<void() volatile& noexcept>();
    test_is_not_unsafe_scalar<void() volatile&& noexcept>();
    test_is_not_unsafe_scalar<void() const volatile noexcept>();
    test_is_not_unsafe_scalar<void() const volatile& noexcept>();
    test_is_not_unsafe_scalar<void() const volatile&& noexcept>();

    test_is_not_unsafe_scalar<void(int)>();
    test_is_not_unsafe_scalar<void(int)&>();
    test_is_not_unsafe_scalar<void(int) &&>();
    test_is_not_unsafe_scalar<void(int) const>();
    test_is_not_unsafe_scalar<void(int) const&>();
    test_is_not_unsafe_scalar<void(int) const&&>();
    test_is_not_unsafe_scalar<void(int) volatile>();
    test_is_not_unsafe_scalar<void(int) volatile&>();
    test_is_not_unsafe_scalar<void(int) volatile&&>();
    test_is_not_unsafe_scalar<void(int) const volatile>();
    test_is_not_unsafe_scalar<void(int) const volatile&>();
    test_is_not_unsafe_scalar<void(int) const volatile&&>();
    test_is_not_unsafe_scalar<void(int) noexcept>();
    test_is_not_unsafe_scalar<void(int)& noexcept>();
    test_is_not_unsafe_scalar<void(int)&& noexcept>();
    test_is_not_unsafe_scalar<void(int) const noexcept>();
    test_is_not_unsafe_scalar<void(int) const& noexcept>();
    test_is_not_unsafe_scalar<void(int) const&& noexcept>();
    test_is_not_unsafe_scalar<void(int) volatile noexcept>();
    test_is_not_unsafe_scalar<void(int) volatile& noexcept>();
    test_is_not_unsafe_scalar<void(int) volatile&& noexcept>();
    test_is_not_unsafe_scalar<void(int) const volatile noexcept>();
    test_is_not_unsafe_scalar<void(int) const volatile& noexcept>();
    test_is_not_unsafe_scalar<void(int) const volatile&& noexcept>();

    test_is_not_unsafe_scalar<void(...)>();
    test_is_not_unsafe_scalar<void(...)&>();
    test_is_not_unsafe_scalar<void(...) &&>();
    test_is_not_unsafe_scalar<void(...) const>();
    test_is_not_unsafe_scalar<void(...) const&>();
    test_is_not_unsafe_scalar<void(...) const&&>();
    test_is_not_unsafe_scalar<void(...) volatile>();
    test_is_not_unsafe_scalar<void(...) volatile&>();
    test_is_not_unsafe_scalar<void(...) volatile&&>();
    test_is_not_unsafe_scalar<void(...) const volatile>();
    test_is_not_unsafe_scalar<void(...) const volatile&>();
    test_is_not_unsafe_scalar<void(...) const volatile&&>();
    test_is_not_unsafe_scalar<void(...) noexcept>();
    test_is_not_unsafe_scalar<void(...)& noexcept>();
    test_is_not_unsafe_scalar<void(...)&& noexcept>();
    test_is_not_unsafe_scalar<void(...) const noexcept>();
    test_is_not_unsafe_scalar<void(...) const& noexcept>();
    test_is_not_unsafe_scalar<void(...) const&& noexcept>();
    test_is_not_unsafe_scalar<void(...) volatile noexcept>();
    test_is_not_unsafe_scalar<void(...) volatile& noexcept>();
    test_is_not_unsafe_scalar<void(...) volatile&& noexcept>();
    test_is_not_unsafe_scalar<void(...) const volatile noexcept>();
    test_is_not_unsafe_scalar<void(...) const volatile& noexcept>();
    test_is_not_unsafe_scalar<void(...) const volatile&& noexcept>();

    test_is_not_unsafe_scalar<void(int, ...)>();
    test_is_not_unsafe_scalar<void(int, ...)&>();
    test_is_not_unsafe_scalar<void(int, ...) &&>();
    test_is_not_unsafe_scalar<void(int, ...) const>();
    test_is_not_unsafe_scalar<void(int, ...) const&>();
    test_is_not_unsafe_scalar<void(int, ...) const&&>();
    test_is_not_unsafe_scalar<void(int, ...) volatile>();
    test_is_not_unsafe_scalar<void(int, ...) volatile&>();
    test_is_not_unsafe_scalar<void(int, ...) volatile&&>();
    test_is_not_unsafe_scalar<void(int, ...) const volatile>();
    test_is_not_unsafe_scalar<void(int, ...) const volatile&>();
    test_is_not_unsafe_scalar<void(int, ...) const volatile&&>();
    test_is_not_unsafe_scalar<void(int, ...) noexcept>();
    test_is_not_unsafe_scalar<void(int, ...)& noexcept>();
    test_is_not_unsafe_scalar<void(int, ...)&& noexcept>();
    test_is_not_unsafe_scalar<void(int, ...) const noexcept>();
    test_is_not_unsafe_scalar<void(int, ...) const& noexcept>();
    test_is_not_unsafe_scalar<void(int, ...) const&& noexcept>();
    test_is_not_unsafe_scalar<void(int, ...) volatile noexcept>();
    test_is_not_unsafe_scalar<void(int, ...) volatile& noexcept>();
    test_is_not_unsafe_scalar<void(int, ...) volatile&& noexcept>();
    test_is_not_unsafe_scalar<void(int, ...) const volatile noexcept>();
    test_is_not_unsafe_scalar<void(int, ...) const volatile& noexcept>();
    test_is_not_unsafe_scalar<void(int, ...) const volatile&& noexcept>();

    test_is_not_unsafe_scalar<int()>();
    test_is_not_unsafe_scalar<int()&>();
    test_is_not_unsafe_scalar<int() &&>();
    test_is_not_unsafe_scalar<int() const>();
    test_is_not_unsafe_scalar<int() const&>();
    test_is_not_unsafe_scalar<int() const&&>();
    test_is_not_unsafe_scalar<int() volatile>();
    test_is_not_unsafe_scalar<int() volatile&>();
    test_is_not_unsafe_scalar<int() volatile&&>();
    test_is_not_unsafe_scalar<int() const volatile>();
    test_is_not_unsafe_scalar<int() const volatile&>();
    test_is_not_unsafe_scalar<int() const volatile&&>();
    test_is_not_unsafe_scalar<int() noexcept>();
    test_is_not_unsafe_scalar<int()& noexcept>();
    test_is_not_unsafe_scalar<int()&& noexcept>();
    test_is_not_unsafe_scalar<int() const noexcept>();
    test_is_not_unsafe_scalar<int() const& noexcept>();
    test_is_not_unsafe_scalar<int() const&& noexcept>();
    test_is_not_unsafe_scalar<int() volatile noexcept>();
    test_is_not_unsafe_scalar<int() volatile& noexcept>();
    test_is_not_unsafe_scalar<int() volatile&& noexcept>();
    test_is_not_unsafe_scalar<int() const volatile noexcept>();
    test_is_not_unsafe_scalar<int() const volatile& noexcept>();
    test_is_not_unsafe_scalar<int() const volatile&& noexcept>();

    test_is_not_unsafe_scalar<int(int)>();
    test_is_not_unsafe_scalar<int(int)&>();
    test_is_not_unsafe_scalar<int(int) &&>();
    test_is_not_unsafe_scalar<int(int) const>();
    test_is_not_unsafe_scalar<int(int) const&>();
    test_is_not_unsafe_scalar<int(int) const&&>();
    test_is_not_unsafe_scalar<int(int) volatile>();
    test_is_not_unsafe_scalar<int(int) volatile&>();
    test_is_not_unsafe_scalar<int(int) volatile&&>();
    test_is_not_unsafe_scalar<int(int) const volatile>();
    test_is_not_unsafe_scalar<int(int) const volatile&>();
    test_is_not_unsafe_scalar<int(int) const volatile&&>();
    test_is_not_unsafe_scalar<int(int) noexcept>();
    test_is_not_unsafe_scalar<int(int)& noexcept>();
    test_is_not_unsafe_scalar<int(int)&& noexcept>();
    test_is_not_unsafe_scalar<int(int) const noexcept>();
    test_is_not_unsafe_scalar<int(int) const& noexcept>();
    test_is_not_unsafe_scalar<int(int) const&& noexcept>();
    test_is_not_unsafe_scalar<int(int) volatile noexcept>();
    test_is_not_unsafe_scalar<int(int) volatile& noexcept>();
    test_is_not_unsafe_scalar<int(int) volatile&& noexcept>();
    test_is_not_unsafe_scalar<int(int) const volatile noexcept>();
    test_is_not_unsafe_scalar<int(int) const volatile& noexcept>();
    test_is_not_unsafe_scalar<int(int) const volatile&& noexcept>();

    test_is_not_unsafe_scalar<int(...)>();
    test_is_not_unsafe_scalar<int(...)&>();
    test_is_not_unsafe_scalar<int(...) &&>();
    test_is_not_unsafe_scalar<int(...) const>();
    test_is_not_unsafe_scalar<int(...) const&>();
    test_is_not_unsafe_scalar<int(...) const&&>();
    test_is_not_unsafe_scalar<int(...) volatile>();
    test_is_not_unsafe_scalar<int(...) volatile&>();
    test_is_not_unsafe_scalar<int(...) volatile&&>();
    test_is_not_unsafe_scalar<int(...) const volatile>();
    test_is_not_unsafe_scalar<int(...) const volatile&>();
    test_is_not_unsafe_scalar<int(...) const volatile&&>();
    test_is_not_unsafe_scalar<int(...) noexcept>();
    test_is_not_unsafe_scalar<int(...)& noexcept>();
    test_is_not_unsafe_scalar<int(...)&& noexcept>();
    test_is_not_unsafe_scalar<int(...) const noexcept>();
    test_is_not_unsafe_scalar<int(...) const& noexcept>();
    test_is_not_unsafe_scalar<int(...) const&& noexcept>();
    test_is_not_unsafe_scalar<int(...) volatile noexcept>();
    test_is_not_unsafe_scalar<int(...) volatile& noexcept>();
    test_is_not_unsafe_scalar<int(...) volatile&& noexcept>();
    test_is_not_unsafe_scalar<int(...) const volatile noexcept>();
    test_is_not_unsafe_scalar<int(...) const volatile& noexcept>();
    test_is_not_unsafe_scalar<int(...) const volatile&& noexcept>();

    test_is_not_unsafe_scalar<int(int, ...)>();
    test_is_not_unsafe_scalar<int(int, ...)&>();
    test_is_not_unsafe_scalar<int(int, ...) &&>();
    test_is_not_unsafe_scalar<int(int, ...) const>();
    test_is_not_unsafe_scalar<int(int, ...) const&>();
    test_is_not_unsafe_scalar<int(int, ...) const&&>();
    test_is_not_unsafe_scalar<int(int, ...) volatile>();
    test_is_not_unsafe_scalar<int(int, ...) volatile&>();
    test_is_not_unsafe_scalar<int(int, ...) volatile&&>();
    test_is_not_unsafe_scalar<int(int, ...) const volatile>();
    test_is_not_unsafe_scalar<int(int, ...) const volatile&>();
    test_is_not_unsafe_scalar<int(int, ...) const volatile&&>();
    test_is_not_unsafe_scalar<int(int, ...) noexcept>();
    test_is_not_unsafe_scalar<int(int, ...)& noexcept>();
    test_is_not_unsafe_scalar<int(int, ...)&& noexcept>();
    test_is_not_unsafe_scalar<int(int, ...) const noexcept>();
    test_is_not_unsafe_scalar<int(int, ...) const& noexcept>();
    test_is_not_unsafe_scalar<int(int, ...) const&& noexcept>();
    test_is_not_unsafe_scalar<int(int, ...) volatile noexcept>();
    test_is_not_unsafe_scalar<int(int, ...) volatile& noexcept>();
    test_is_not_unsafe_scalar<int(int, ...) volatile&& noexcept>();
    test_is_not_unsafe_scalar<int(int, ...) const volatile noexcept>();
    test_is_not_unsafe_scalar<int(int, ...) const volatile& noexcept>();
    test_is_not_unsafe_scalar<int(int, ...) const volatile&& noexcept>();

    test_is_unsafe_scalar<void (*)()>();
    test_is_unsafe_scalar<void (*)() noexcept>();

    test_is_unsafe_scalar<void (*)(int)>();
    test_is_unsafe_scalar<void (*)(int) noexcept>();

    test_is_unsafe_scalar<void (*)(...)>();
    test_is_unsafe_scalar<void (*)(...) noexcept>();

    test_is_unsafe_scalar<void (*)(int, ...)>();
    test_is_unsafe_scalar<void (*)(int, ...) noexcept>();

    test_is_unsafe_scalar<int (*)()>();
    test_is_unsafe_scalar<int (*)() noexcept>();

    test_is_unsafe_scalar<int (*)(int)>();
    test_is_unsafe_scalar<int (*)(int) noexcept>();

    test_is_unsafe_scalar<int (*)(...)>();
    test_is_unsafe_scalar<int (*)(...) noexcept>();

    test_is_unsafe_scalar<int (*)(int, ...)>();
    test_is_unsafe_scalar<int (*)(int, ...) noexcept>();

    test_is_not_unsafe_scalar<void (&)()>();
    test_is_not_unsafe_scalar<void (&)() noexcept>();

    test_is_not_unsafe_scalar<void (&)(int)>();
    test_is_not_unsafe_scalar<void (&)(int) noexcept>();

    test_is_not_unsafe_scalar<void (&)(...)>();
    test_is_not_unsafe_scalar<void (&)(...) noexcept>();

    test_is_not_unsafe_scalar<void (&)(int, ...)>();
    test_is_not_unsafe_scalar<void (&)(int, ...) noexcept>();

    test_is_not_unsafe_scalar<int (&)()>();
    test_is_not_unsafe_scalar<int (&)() noexcept>();

    test_is_not_unsafe_scalar<int (&)(int)>();
    test_is_not_unsafe_scalar<int (&)(int) noexcept>();

    test_is_not_unsafe_scalar<int (&)(...)>();
    test_is_not_unsafe_scalar<int (&)(...) noexcept>();

    test_is_not_unsafe_scalar<int (&)(int, ...)>();
    test_is_not_unsafe_scalar<int (&)(int, ...) noexcept>();

    test_is_not_unsafe_scalar<void(&&)()>();
    test_is_not_unsafe_scalar<void(&&)() noexcept>();

    test_is_not_unsafe_scalar<void(&&)(int)>();
    test_is_not_unsafe_scalar<void(&&)(int) noexcept>();

    test_is_not_unsafe_scalar<void(&&)(...)>();
    test_is_not_unsafe_scalar<void(&&)(...) noexcept>();

    test_is_not_unsafe_scalar<void(&&)(int, ...)>();
    test_is_not_unsafe_scalar<void(&&)(int, ...) noexcept>();

    test_is_not_unsafe_scalar<int(&&)()>();
    test_is_not_unsafe_scalar<int(&&)() noexcept>();

    test_is_not_unsafe_scalar<int(&&)(int)>();
    test_is_not_unsafe_scalar<int(&&)(int) noexcept>();

    test_is_not_unsafe_scalar<int(&&)(...)>();
    test_is_not_unsafe_scalar<int(&&)(...) noexcept>();

    test_is_not_unsafe_scalar<int(&&)(int, ...)>();
    test_is_not_unsafe_scalar<int(&&)(int, ...) noexcept>();

    test_is_unsafe_scalar<void (class_type::*)()>();
    test_is_unsafe_scalar<void (class_type::*)()&>();
    test_is_unsafe_scalar<void (class_type::*)() &&>();
    test_is_unsafe_scalar<void (class_type::*)() const>();
    test_is_unsafe_scalar<void (class_type::*)() const&>();
    test_is_unsafe_scalar<void (class_type::*)() const&&>();
    test_is_unsafe_scalar<void (class_type::*)() noexcept>();
    test_is_unsafe_scalar<void (class_type::*)()& noexcept>();
    test_is_unsafe_scalar<void (class_type::*)()&& noexcept>();
    test_is_unsafe_scalar<void (class_type::*)() const noexcept>();
    test_is_unsafe_scalar<void (class_type::*)() const& noexcept>();
    test_is_unsafe_scalar<void (class_type::*)() const&& noexcept>();

    test_is_unsafe_scalar<void (class_type::*)(int)>();
    test_is_unsafe_scalar<void (class_type::*)(int)&>();
    test_is_unsafe_scalar<void (class_type::*)(int) &&>();
    test_is_unsafe_scalar<void (class_type::*)(int) const>();
    test_is_unsafe_scalar<void (class_type::*)(int) const&>();
    test_is_unsafe_scalar<void (class_type::*)(int) const&&>();
    test_is_unsafe_scalar<void (class_type::*)(int) noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(int)& noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(int)&& noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(int) const noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(int) const& noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(int) const&& noexcept>();

    test_is_unsafe_scalar<void (class_type::*)(...)>();
    test_is_unsafe_scalar<void (class_type::*)(...)&>();
    test_is_unsafe_scalar<void (class_type::*)(...) &&>();
    test_is_unsafe_scalar<void (class_type::*)(...) const>();
    test_is_unsafe_scalar<void (class_type::*)(...) const&>();
    test_is_unsafe_scalar<void (class_type::*)(...) const&&>();
    test_is_unsafe_scalar<void (class_type::*)(...) noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(...)& noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(...)&& noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(...) const noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(...) const& noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(...) const&& noexcept>();

    test_is_unsafe_scalar<void (class_type::*)(int, ...)>();
    test_is_unsafe_scalar<void (class_type::*)(int, ...)&>();
    test_is_unsafe_scalar<void (class_type::*)(int, ...) &&>();
    test_is_unsafe_scalar<void (class_type::*)(int, ...) const>();
    test_is_unsafe_scalar<void (class_type::*)(int, ...) const&>();
    test_is_unsafe_scalar<void (class_type::*)(int, ...) const&&>();
    test_is_unsafe_scalar<void (class_type::*)(int, ...) noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(int, ...)& noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(int, ...)&& noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(int, ...) const noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(int, ...) const& noexcept>();
    test_is_unsafe_scalar<void (class_type::*)(int, ...) const&& noexcept>();

    test_is_unsafe_scalar<int (class_type::*)()>();
    test_is_unsafe_scalar<int (class_type::*)()&>();
    test_is_unsafe_scalar<int (class_type::*)() &&>();
    test_is_unsafe_scalar<int (class_type::*)() const>();
    test_is_unsafe_scalar<int (class_type::*)() const&>();
    test_is_unsafe_scalar<int (class_type::*)() const&&>();
    test_is_unsafe_scalar<int (class_type::*)() noexcept>();
    test_is_unsafe_scalar<int (class_type::*)()& noexcept>();
    test_is_unsafe_scalar<int (class_type::*)()&& noexcept>();
    test_is_unsafe_scalar<int (class_type::*)() const noexcept>();
    test_is_unsafe_scalar<int (class_type::*)() const& noexcept>();
    test_is_unsafe_scalar<int (class_type::*)() const&& noexcept>();

    test_is_unsafe_scalar<int (class_type::*)(int)>();
    test_is_unsafe_scalar<int (class_type::*)(int)&>();
    test_is_unsafe_scalar<int (class_type::*)(int) &&>();
    test_is_unsafe_scalar<int (class_type::*)(int) const>();
    test_is_unsafe_scalar<int (class_type::*)(int) const&>();
    test_is_unsafe_scalar<int (class_type::*)(int) const&&>();
    test_is_unsafe_scalar<int (class_type::*)(int) noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(int)& noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(int)&& noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(int) const noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(int) const& noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(int) const&& noexcept>();

    test_is_unsafe_scalar<int (class_type::*)(...)>();
    test_is_unsafe_scalar<int (class_type::*)(...)&>();
    test_is_unsafe_scalar<int (class_type::*)(...) &&>();
    test_is_unsafe_scalar<int (class_type::*)(...) const>();
    test_is_unsafe_scalar<int (class_type::*)(...) const&>();
    test_is_unsafe_scalar<int (class_type::*)(...) const&&>();
    test_is_unsafe_scalar<int (class_type::*)(...) noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(...)& noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(...)&& noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(...) const noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(...) const& noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(...) const&& noexcept>();

    test_is_unsafe_scalar<int (class_type::*)(int, ...)>();
    test_is_unsafe_scalar<int (class_type::*)(int, ...)&>();
    test_is_unsafe_scalar<int (class_type::*)(int, ...) &&>();
    test_is_unsafe_scalar<int (class_type::*)(int, ...) const>();
    test_is_unsafe_scalar<int (class_type::*)(int, ...) const&>();
    test_is_unsafe_scalar<int (class_type::*)(int, ...) const&&>();
    test_is_unsafe_scalar<int (class_type::*)(int, ...) noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(int, ...)& noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(int, ...)&& noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(int, ...) const noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(int, ...) const& noexcept>();
    test_is_unsafe_scalar<int (class_type::*)(int, ...) const&& noexcept>();
}
