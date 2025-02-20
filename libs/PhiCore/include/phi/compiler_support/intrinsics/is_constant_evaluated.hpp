#ifndef INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_CONSTANT_EVALUATED_HPP
#define INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_CONSTANT_EVALUATED_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/generated/compiler_support/features.hpp"

#if !defined(PHI_IS_CONSTANT_EVALUATED) && !defined(PHI_SUPPORTS_IS_CONSTANT_EVALUATED)
#    if PHI_HAS_INTRINSIC_BUILTIN_IS_CONSTANT_EVALUATED()
#        define PHI_IS_CONSTANT_EVALUATED()          __builtin_is_constant_evaluated()
#        define PHI_SUPPORTS_IS_CONSTANT_EVALUATED() 1
#    else
#        define PHI_IS_CONSTANT_EVALUATED()          false
#        define PHI_SUPPORTS_IS_CONSTANT_EVALUATED() 0
#    endif

#    if defined(PHI_CONFIG_NO_INTRINSICS)
#        undef PHI_SUPPORTS_IS_CONSTANT_EVALUATED
#        define PHI_SUPPORTS_IS_CONSTANT_EVALUATED() 0
#    endif
#endif

#endif // INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_CONSTANT_EVALUATED_HPP
