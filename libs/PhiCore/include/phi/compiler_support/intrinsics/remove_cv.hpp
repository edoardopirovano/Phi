#ifndef INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_REMOVE_CV_HPP
#define INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_REMOVE_CV_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/generated/compiler_support/features.hpp"

#if !defined(PHI_REMOVE_CV) && !defined(PHI_SUPPORTS_REMOVE_CV)
#    if PHI_HAS_INTRINSIC_REMOVE_CV()
#        define PHI_REMOVE_CV(type)      __remove_cv(type)
#        define PHI_SUPPORTS_REMOVE_CV() 1
#    else
#        define PHI_REMOVE_CV(type)      false
#        define PHI_SUPPORTS_REMOVE_CV() 0
#    endif

#    if defined(PHI_CONFIG_NO_INTRINSICS)
#        undef PHI_SUPPORTS_REMOVE_CV
#        define PHI_SUPPORTS_REMOVE_CV() 0
#    endif
#endif

#endif // INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_REMOVE_CV_HPP
