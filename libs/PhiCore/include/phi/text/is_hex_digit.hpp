#ifndef INCG_PHI_CORE_TEXT_IS_HEX_DIGIT_HPP
#define INCG_PHI_CORE_TEXT_IS_HEX_DIGIT_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/compiler_support/nodiscard.hpp"
#include "phi/core/boolean.hpp"

DETAIL_PHI_BEGIN_NAMESPACE()

// Implementation comparison: https://godbolt.org/z/3Yqsd8oTd

PHI_NODISCARD constexpr phi::boolean is_hex_digit(const char c) noexcept
{
    // NOLINTBEGIN(hicpp-signed-bitwise,readability-implicit-bool-conversion)
    return static_cast<bool>((c >= '0' && c <= '9') | (c >= 'a' && c <= 'f') |
                             (c >= 'A' && c <= 'F'));
    // NOLINTEND(hicpp-signed-bitwise,readability-implicit-bool-conversion)
}

DETAIL_PHI_END_NAMESPACE()

#endif // INCG_PHI_CORE_TEXT_IS_HEX_DIGIT_HPP
