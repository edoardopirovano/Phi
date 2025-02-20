#ifndef INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_MEMBER_OBJECT_POINTER_HPP
#define INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_MEMBER_OBJECT_POINTER_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/generated/compiler_support/features.hpp"

#if !defined(PHI_IS_MEMBER_OBJECT_POINTER) && !defined(PHI_SUPPORTS_IS_MEMBER_OBJECT_POINTER)
#    if PHI_HAS_INTRINSIC_IS_MEMBER_OBJECT_POINTER()
#        define PHI_IS_MEMBER_OBJECT_POINTER(type)      __is_member_object_pointer(type)
#        define PHI_SUPPORTS_IS_MEMBER_OBJECT_POINTER() 1
#    else
#        define PHI_IS_MEMBER_OBJECT_POINTER(type)      false
#        define PHI_SUPPORTS_IS_MEMBER_OBJECT_POINTER() 0
#    endif

#    if defined(PHI_CONFIG_NO_INTRINSICS)
#        undef PHI_SUPPORTS_IS_MEMBER_OBJECT_POINTER
#        define PHI_SUPPORTS_IS_MEMBER_OBJECT_POINTER() 0
#    endif
#endif

#endif // INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_MEMBER_OBJECT_POINTER_HPP
