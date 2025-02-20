#ifndef INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_OBJECT_HPP
#define INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_OBJECT_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/generated/compiler_support/features.hpp"

#if !defined(PHI_IS_OBJECT) && !defined(PHI_SUPPORTS_IS_OBJECT)
#    if PHI_HAS_INTRINSIC_IS_OBJECT()
#        define PHI_IS_OBJECT(type)      __is_object(type)
#        define PHI_SUPPORTS_IS_OBJECT() 1
#    else
#        define PHI_IS_OBJECT(type)      false
#        define PHI_SUPPORTS_IS_OBJECT() 0
#    endif

#    if defined(PHI_CONFIG_NO_INTRINSICS)
#        undef PHI_SUPPORTS_IS_OBJECT
#        define PHI_SUPPORTS_IS_OBJECT() 0
#    endif
#endif

#endif // INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_OBJECT_HPP
