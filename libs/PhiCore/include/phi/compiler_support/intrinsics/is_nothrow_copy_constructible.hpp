#ifndef INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_NOTHROW_COPY_CONSTRUCTIBLE_HPP
#define INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_NOTHROW_COPY_CONSTRUCTIBLE_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/compiler_support/compiler.hpp"
#include "phi/generated/compiler_support/features.hpp"

#if !defined(PHI_IS_NOTHROW_COPY_CONSTRUCTIBLE) &&                                                 \
        !defined(PHI_SUPPORTS_IS_NOTHROW_COPY_CONSTRUCTIBLE)
#    if PHI_HAS_INTRINSIC_HAS_NOTHROW_COPY() && PHI_COMPILER_IS(MSVC)
#        define PHI_IS_NOTHROW_COPY_CONSTRUCTIBLE(type)      __has_nothrow_copy(type)
#        define PHI_SUPPORTS_IS_NOTHROW_COPY_CONSTRUCTIBLE() 1
#    else
#        define PHI_IS_NOTHROW_COPY_CONSTRUCTIBLE(type)      false
#        define PHI_SUPPORTS_IS_NOTHROW_COPY_CONSTRUCTIBLE() 0
#    endif

#    if defined(PHI_CONFIG_NO_INTRINSICS)
#        undef PHI_SUPPORTS_IS_NOTHROW_COPY_CONSTRUCTIBLE
#        define PHI_SUPPORTS_IS_NOTHROW_COPY_CONSTRUCTIBLE() 0
#    endif
#endif

#endif // INCH_PHI_CORE_COMPILER_SUPPORT_INTRINSICS_IS_NOTHROW_COPY_CONSTRUCTIBLE_HPP
