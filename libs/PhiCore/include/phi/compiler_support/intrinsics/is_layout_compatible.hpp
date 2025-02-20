#ifndef INCG_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_LAYOUT_COMPATIBLE_HPP
#define INCG_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_LAYOUT_COMPATIBLE_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/generated/compiler_support/features.hpp"

#if !defined(PHI_IS_LAYOUT_COMPATIBLE) && !defined(PHI_SUPPORTS_IS_COMPATIBLE_COMPAT)
#    if PHI_HAS_INTRINSIC_IS_LAYOUT_COMPATIBLE()
#        define PHI_IS_LAYOUT_COMPATIBLE(type, with) __is_layout_compatible(type, with)
#        define PHI_SUPPORTS_IS_LAYOUT_COMPATIBLE()  1
#    else
#        define PHI_IS_LAYOUT_COMPATIBLE(type, with) false
#        define PHI_SUPPORTS_IS_LAYOUT_COMPATIBLE()  0
#    endif

#    if defined(PHI_CONFIG_NO_INTRINSICS)
#        undef PHI_SUPPORTS_IS_LAYOUT_COMPATIBLE
#        define PHI_SUPPORTS_IS_LAYOUT_COMPATIBLE() 0
#    endif
#endif

#endif // INCG_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_LAYOUT_COMPATIBLE_HPP
