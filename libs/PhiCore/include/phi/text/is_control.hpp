#ifndef INCG_PHI_CORE_TEXT_IS_CONTROL_HPP
#define INCG_PHI_CORE_TEXT_IS_CONTROL_HPP

#include "phi/phi_config.hpp"

#if PHI_HAS_EXTENSION_PRAGMA_ONCE()
#    pragma once
#endif

#include "phi/compiler_support/nodiscard.hpp"
#include "phi/core/boolean.hpp"

DETAIL_PHI_BEGIN_NAMESPACE()

PHI_NODISCARD constexpr phi::boolean is_control(const char c) noexcept
{
    // NOLINTNEXTLINE(hicpp-signed-bitwise,readability-implicit-bool-conversion,readability-magic-numbers)
    return static_cast<bool>((c >= 0 && c <= 31) | (c == 127));
}

DETAIL_PHI_END_NAMESPACE()

#endif // INCG_PHI_CORE_TEXT_IS_CONTROL_HPP
