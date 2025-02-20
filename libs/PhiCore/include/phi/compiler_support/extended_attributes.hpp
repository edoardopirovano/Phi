#ifndef INCG_PHI_CORE_COMPILER_SUPPORT_EXTENDED_ATTRIBUTES_HPP
#define INCG_PHI_CORE_COMPILER_SUPPORT_EXTENDED_ATTRIBUTES_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/generated/compiler_support/features.hpp"

#if PHI_HAS_EXTENSION_ATTRIBUTE_CONST()
#    define PHI_ATTRIBUTE_CONST __attribute__((const))
#else
#    define PHI_ATTRIBUTE_CONST /* Nothing */
#endif

#if PHI_HAS_EXTENSION_ATTRIBUTE_PURE()
#    define PHI_ATTRIBUTE_PURE __attribute__((pure))
#else
#    define PHI_ATTRIBUTE_PURE /* Nothing */
#endif

#endif // INCG_PHI_CORE_COMPILER_SUPPORT_EXTENDED_ATTRIBUTES_HPP
