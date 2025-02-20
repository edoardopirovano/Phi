#ifndef INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_HAS_TRIVIAL_DESTRUCTOR_HPP
#define INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_HAS_TRIVIAL_DESTRUCTOR_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/generated/compiler_support/features.hpp"

#if !defined(PHI_HAS_TRIVIAL_DESTRUCTOR) && !defined(PHI_SUPPORTS_HAS_TRIVIAL_DESTRUCTOR)
#    if PHI_HAS_INTRINSIC_HAS_TRIVIAL_DESTRUCTOR()
#        define PHI_HAS_TRIVIAL_DESTRUCTOR(type)      __has_trivial_destructor(type)
#        define PHI_SUPPORTS_HAS_TRIVIAL_DESTRUCTOR() 1
#    else
#        define PHI_HAS_TRIVIAL_DESTRUCTOR(type)      false
#        define PHI_SUPPORTS_HAS_TRIVIAL_DESTRUCTOR() 0
#    endif

#    if defined(PHI_CONFIG_NO_INTRINSICS)
#        undef PHI_SUPPORTS_HAS_TRIVIAL_DESTRUCTOR
#        define PHI_SUPPORTS_HAS_TRIVIAL_DESTRUCTOR() 0
#    endif
#endif

#endif // INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_HAS_TRIVIAL_DESTRUCTOR_HPP
