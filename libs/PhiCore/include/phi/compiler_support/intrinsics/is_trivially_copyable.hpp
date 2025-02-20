#ifndef INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_TRIVIALLY_COPYABLE_HPP
#define INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_TRIVIALLY_COPYABLE_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/generated/compiler_support/features.hpp"

#if !defined(PHI_IS_TRIVIALLY_COPYABLE) && !defined(PHI_SUPPORTS_IS_TRIVIALLY_COPYABLE)
#    if PHI_HAS_INTRINSIC_IS_TRIVIALLY_COPYABLE()
#        define PHI_IS_TRIVIALLY_COPYABLE(type)      __is_trivially_copyable(type)
#        define PHI_SUPPORTS_IS_TRIVIALLY_COPYABLE() 1
#    else
#        define PHI_IS_TRIVIALLY_COPYABLE(type)      false
#        define PHI_SUPPORTS_IS_TRIVIALLY_COPYABLE() 0
#    endif

#    if defined(PHI_CONFIG_NO_INTRINSICS)
#        undef PHI_SUPPORTS_IS_TRIVIALLY_COPYABLE
#        define PHI_SUPPORTS_IS_TRIVIALLY_COPYABLE() 0
#    endif
#endif

#endif // INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_TRIVIALLY_COPYABLE_HPP
