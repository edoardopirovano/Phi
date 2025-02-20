#ifndef INCG_PHI_CORE_PREPROCESSOR_VERSIONING_HPP
#define INCG_PHI_CORE_PREPROCESSOR_VERSIONING_HPP

#define PHI_VERSION_CREATE(major, minor, patch)                                                    \
    (((major) << 24) + ((minor) << 16) + (patch)) /* NOLINT(hicpp-signed-bitwise) */

#define PHI_VERSION_EXTRACT_MAJOR(version) ((version) >> 24) /* NOLINT(hicpp-signed-bitwise) */

#define PHI_VERSION_EXTRACT_MINOR(version)                                                         \
    (((version) >> 16) % 256) /* NOLINT(hicpp-signed-bitwise) */

#define PHI_VERSION_EXTRACT_PATCH(version) ((version) % 256)

#endif // INCG_PHI_CORE_PREPROCESSOR_VERSIONING_HPP
