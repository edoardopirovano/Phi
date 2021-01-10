# Detect information about the build environment

# Detect Operating System (OS)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
  set(PHI_OS_WINDOWS 1)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
  set(PHI_OS_UNIX 1)

  if(ANDROID)
    set(PHI_OS_ANDROID 1)
  else()
    set(PHI_OS_LINUX 1)
  endif()
elseif(CMAKE_SYSTEM_NAME MATCHES "^k?FreeBSD$")
  set(PHI_OS_FREEBSD 1)
elseif(CMAKE_SYSTEM_NAME MATCHES "^OpenBSD$")
  set(PHI_OS_OPENBSD 1)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Darwin")
  if(IOS)
    set(PHI_OS_IOS 1)
  else()
    set(PHI_OS_MACOSX 1)
  endif()
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Android")
  set(PHI_OS_ANDROID 1)
else()
  message(FATAL_ERROR "Unsupported operating system or environment")
  return()
endif()

# Detect the architecture (note: this test won't work for cross-compilation)
include(CheckTypeSize)
check_type_size(void* SIZEOF_VOID_PTR BUILTIN_TYPES_ONLY LANGUAGE CXX)
if(${SIZEOF_VOID_PTR} STREQUAL "4")
  set(PHI_ARCH_32BITS 1)
elseif(${SIZEOF_VOID_PTR} STREQUAL "8")
  set(PHI_ARCH_64BITS 1)
else()
  message(FATAL_ERROR "Unsupported architecture")
  return()
endif()

# Detect compiler
if(CMAKE_CXX_COMPILER MATCHES "clang[+][+]" OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  # CMAKE_CXX_COMPILER_ID is an internal CMake variable subject to change, but there is no other way
  # to detect CLang at the moment
  set(PHI_COMPILER_CLANG 1)
  execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "--version" OUTPUT_VARIABLE CLANG_VERSION_OUTPUT)
  string(REGEX REPLACE ".*clang version ([0-9]+\\.[0-9]+).*" "\\1" PHI_CLANG_VERSION
                       "${CLANG_VERSION_OUTPUT}")

  # Test for AppleClang
  if(CMAKE_CXX_COMPILER_ID MATCHES "AppleClang")
    set(PHI_COMPILER_APPLECLANG 1)
  endif()
elseif(CMAKE_COMPILER_IS_GNUCXX)
  set(PHI_COMPILER_GCC 1)
  execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpversion" OUTPUT_VARIABLE GCC_VERSION_OUTPUT)
  string(REGEX REPLACE "([0-9]+\\.[0-9]+).*" "\\1" PHI_GCC_VERSION "${GCC_VERSION_OUTPUT}")
  execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "--version" OUTPUT_VARIABLE GCC_COMPILER_VERSION)
  string(REGEX MATCHALL ".*(tdm[64]*-[1-9]).*" PHI_COMPILER_GCC_TDM "${GCC_COMPILER_VERSION}")
  execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpmachine" OUTPUT_VARIABLE GCC_MACHINE)
  string(STRIP "${GCC_MACHINE}" GCC_MACHINE)
  if(GCC_MACHINE MATCHES ".*w64.*")
    set(PHI_COMPILER_GCC_W64 1)
  endif()
elseif(MSVC)
  set(PHI_COMPILER_MSVC 1)
  if(MSVC_VERSION EQUAL 1400)
    set(PHI_MSVC_VERSION 8)
  elseif(MSVC_VERSION EQUAL 1500)
    set(PHI_MSVC_VERSION 9)
  elseif(MSVC_VERSION EQUAL 1600)
    set(PHI_MSVC_VERSION 10)
  elseif(MSVC_VERSION EQUAL 1700)
    set(PHI_MSVC_VERSION 11)
  elseif(MSVC_VERSION EQUAL 1800)
    set(PHI_MSVC_VERSION 12)
  elseif(MSVC_VERSION EQUAL 1900)
    set(PHI_MSVC_VERSION 14)
  elseif(MSVC_VERSION EQUAL 1910)
    set(PHI_MSVC_VERSION 14.1)
  elseif(MSVC_VERSION EQUAL 1911)
    set(PHI_MSVC_VERSION 14.11)
  elseif(MSVC_VERSION EQUAL 1912)
    set(PHI_MSVC_VERSION 14.12)
  elseif(MSVC_VERSION EQUAL 1913)
    set(PHI_MSVC_VERSION 14.13)
  elseif(MSVC_VERSION EQUAL 1914)
    set(PHI_MSVC_VERSION 14.14)
  elseif(MSVC_VERSION EQUAL 1915)
    set(PHI_MSVC_VERSION 14.15)
  elseif(MSVC_VERSION EQUAL 1916)
    set(PHI_MSVC_VERSION 14.16)
  elseif(MSVC_VERSION EQUAL 1920)
    set(PHI_MSVC_VERSION 14.2)
  elseif(MSVC_VERSION EQUAL 1921)
    set(PHI_MSVC_VERSION 14.21)
  elseif(MSVC_VERSION EQUAL 1922)
    set(PHI_MSVC_VERSION 14.22)
  elseif(MSVC_VERSION EQUAL 1923)
    set(PHI_MSVC_VERSION 14.23)
  elseif(MSVC_VERSION EQUAL 1924)
    set(PHI_MSVC_VERSION 14.24)
  elseif(MSVC_VERSION EQUAL 1925)
    set(PHI_MSVC_VERSION 14.25)
  elseif(MSVC_VERSION EQUAL 1926)
    set(PHI_MSVC_VERSION 14.26)
  elseif(MSVC_VERSION EQUAL 1927)
    set(PHI_MSVC_VERSION 14.27)
  elseif(MSVC_VERSION EQUAL 1928)
    set(PHI_MSVC_VERSION 14.28)
  else()
    message(WARNING "Unknown version of MSVC used. MSVC_VERSION=${MSVC_VERSION}")
  endif()
else()
  message(FATAL_ERROR "Unsupported compiler")
  return()
endif()
