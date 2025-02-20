phi_include_guard()

include(CMakeParseArguments)
include(internal/PhiCheckCXXCompilerFlag)

# Flags
set(phi_warning_flags
    Waligned-new=all
    Wall
    Walloc-zero
    Walloca
    Warith-conversion
    Warray-bounds
    Wattribute-alias=2
    Wbidi-chars=any
    Wcast-align # warn for potential performance problem casts
    Wcast-qual
    Wcatch-value=3
    Wcomma-subscript
    Wconversion # warn on type conversions that may lose data
    Wdate-time
    Wdouble-promotion # warn if float is implicit promoted to double
    Wduplicated-branches # warn if if / else branches have duplicated code
    Wduplicated-cond # warn if if / else chain has duplicated conditions
    Wenum-conversion
    Weverything
    Wextra # reasonable and standard
    Wextra-semi
    Wfloat-equal
    Wformat-overflow=2
    Wformat-signedness
    Wformat-truncation=2
    Wformat=2 # warn on security issues around functions that format output (ie printf)
    Winvalid-pch
    Wlifetime
    Wlogical-op # warn about logical operations being used where bitwise were probably wanted
    Wmisleading-indentation # warn if identation implies blocks where blocks do not exist
    Wmismatched-tags
    Wmissing-field-initializers
    Wmissing-format-attribute
    Wmissing-include-dirs
    Wmissing-noreturn
    Wnoexcept
    Wnon-virtual-dtor # warn the user if a class with virtual functions has a non-virtual
                      # destructor. This helps catch hard to track down memory errors
    Wnormalized
    Wnull-dereference # warn if a null dereference is detected
    Wold-style-cast # warn for c-style casts
    Woverloaded-virtual # warn if you overload (not override) a virtual function
    Wpedantic # warn if non-standard C++ is used
    Wplacement-new=2
    Wpointer-arith
    Wredundant-decls
    Wredundant-tags
    Wregister
    Wrestrict
    Wshadow # warn the user if a variable declaration shadows one from a parent context
    Wshift-overflow=2
    Wsign-conversion # warn on sign conversions
    Wstack-protector
    Wstrict-aliasing=2
    Wstrict-null-sentinel
    Wstrict-overflow=5
    Wsuggest-attribute=cold
    Wsuggest-attribute=const
    Wsuggest-attribute=format
    Wsuggest-attribute=malloc
    Wsuggest-attribute=noreturn
    Wsuggest-attribute=pure
    Wsuggest-final-methods
    Wsuggest-final-types
    Wsuggest-override
    Wsync-nand
    Wtrampolines
    Wundef
    Wuninitialized
    Wunreachable-code
    Wunsafe-loop-optimizations
    Wunused # warn on anything being unused
    Wunused-const-variable=2
    Wunused-parameter
    Wuse-after-free=3
    Wuseless-cast # warn if you perform a cast to the same type
    Wvector-operation-performance
    Wvla
    Wvolatile
    Wzero-as-null-pointer-constant
    # MSVC
    Wall # Baseline reasonable warnings
    w14242 # 'identifier': conversion from 'type1' to 'type1', possible loss of data
    w14254 # 'operator': conversion from 'type1:field_bits' to 'type2:field_bits', possible loss
    # of data
    w14263 # 'function': member function does not override any base class virtual member function
    w14265 # 'classname': class has virtual functions, but destructor is not virtual instances of
    # this class may not be destructed correctly
    w14287 # 'operator': unsigned/negative constant mismatch
    we4289 # nonstandard extension used: 'variable': loop control variable declared in the
    # for-loop is used outside the for-loop scope
    w14296 # 'operator': expression is always 'boolean_value'
    w14311 # 'variable': pointer truncation from 'type1' to 'type2'
    w14545 # expression before comma evaluates to a function which is missing an argument list
    w14546 # function call before comma missing argument list
    w14547 # 'operator': operator before comma has no effect; expected operator with side-effect
    w14549 # 'operator': operator before comma has no effect; did you intend 'operator'?
    w14555 # expression has no effect; expected expression with side- effect
    w14619 # pragma warning: there is no warning number 'number'
    w14640 # Enable warning on thread un-safe static member initialization
    w14826 # Conversion from 'type1' to 'type_2' is sign-extended. This may cause unexpected
    # runtime behavior.
    w14905 # wide string literal cast to 'LPSTR'
    w14906 # string literal cast to 'LPWSTR'
    w14928 # illegal copy-initialization; more than one user-defined conversion has been
    # implicitly applied
)

set(phi_pedantic_flags "pedantic" "pedantic-errors" "permissive-")

set(phi_warnings_as_errors_flags "Werror" "WX")

set(phi_disabled_warnings_flags
    # MSVC
    wd4101 # 'x': unreferenced local variable
    wd4464 # relative include path contains '..'
    wd4481 # nonstandard extension used: override specifier 'override'
    wd4514 # 'x': unreferenced inline function has been removed
    wd4548 # expression before comma has no effect; expected expression with side-effect
    wd4571 # SEH related
    wd4577 # 'noexcept' used with no exception handling mode specified; termination on exception
    # is not guaranteed. Specify /EHsc
    wd4616 # invalid compiler warnings - https://msdn.microsoft.com/en-us/library/t7ab6xtd.aspx
    wd4619 # invalid compiler warnings - https://msdn.microsoft.com/en-us/library/tacee08d.aspx
    wd4623 # 'x': default constructor was implicitly defined as deleted
    wd4625 # 'x': copy constructor was implicitly defined as deleted
    wd4626 # 'x': assignment operator was implicitly defined as deleted
    wd4643 # Forward declaring 'x' in namespace std is not permitted by the C++ Standard.
    wd4710 # 'x': function not inlined
    wd4711 # function 'x' selected for automatic inline expansion
    wd4814 # 'x in C++14 'constexpr' will not imply 'const'; consider explicitly specifying
    # 'const'
    wd4820 # 'bytes' bytes padding added after construct 'member_name' -
    # https://msdn.microsoft.com/en-us/library/t7khkyth.aspx
    wd5026 # 'x': move constructor was implicitly defined as deleted
    wd5027 # 'x': move assignment operator was implicitly defined as deleted
    wd5045 # Compiler will insert Spectre mitigation for memory load if /Qspectre switch
    # specified
    wd4505 # 'x': unreferenced function with internal linkage has been removed
    wd4180 # qualifier applied to function type has no meaning; ignored
    wd5052 # Keyword 'char8_t' was introduced in C++20 and requires use of the '/std:c++latest'
    # command-line option
    wd4866 # compilerr may not enforce left-to-right evaluation order for call to 'x'
    wd4324 # 'struct_name' : structure was padded due to __declspec(align())
    wd5220 # 'name': a non-static data member with a volatile qualified type no longer implies that
    # compiler generated copy/move constructors and copy/move assignment operators are not trivial
    Wno-constexpr-not-const
    Wno-c++1y-extensions
    Wno-c++1z-extensions
    Wno-c++98-compat
    Wno-c++98-compat-pedantic
    Wno-c++98-c++11-compat-pedantic
    Wno-documentation-unknown-command
    Wno-gnu-zero-variadic-macro-arguments
    Wno-language-extension-token
    Wno-missing-prototypes
    Wno-missing-variable-declarations
    Wno-padded
    Wno-unused-macros
    Wno-unused-template
    Wno-variadic-macros
    Wno-weak-vtables
    # GCC
    Wno-unused-function)

set(phi_check_required_flags "Werror=unknown-attributes")

# Checks

# Check all normal warnings
set(_WarningsAvailible)
foreach(_test ${phi_warning_flags})
  string(REPLACE "-" "_" _testName ${_test})
  string(REPLACE "=" "_" _testName ${_testName})
  string(TOUPPER ${_testName} _testName)

  phi_check_cxx_compiler_flag(${PHI_FLAG_PREFIX_CHAR}${_test} "PHI_HAS_FLAG_${_testName}")

  if(PHI_HAS_FLAG_${_testName})
    list(APPEND _WarningsAvailible ${PHI_FLAG_PREFIX_CHAR}${_test})
  endif()
endforeach(_test)

# Check pedantic
set(_PedanticAvailible)
foreach(_test ${phi_pedantic_flags})
  string(REPLACE "-" "_" _testName ${_test})
  string(REPLACE "=" "_" _testName ${_testName})
  string(TOUPPER ${_testName} _testName)

  phi_check_cxx_compiler_flag(${PHI_FLAG_PREFIX_CHAR}${_test} "PHI_HAS_FLAG_${_testName}")

  if(PHI_HAS_FLAG_${_testName})
    list(APPEND _PedanticAvailible ${PHI_FLAG_PREFIX_CHAR}${_test})
  endif()
endforeach(_test)

# Check warning as error
set(_WarningAsErrorAvailible)
foreach(_test ${phi_warnings_as_errors_flags})
  string(REPLACE "-" "_" _testName ${_test})
  string(REPLACE "=" "_" _testName ${_testName})
  string(TOUPPER ${_testName} _testName)

  phi_check_cxx_compiler_flag(${PHI_FLAG_PREFIX_CHAR}${_test} "PHI_HAS_FLAG_${_testName}")

  if(PHI_HAS_FLAG_${_testName})
    list(APPEND _WarningAsErrorAvailible ${PHI_FLAG_PREFIX_CHAR}${_test})
  endif()
endforeach(_test)

# Disabled warnings
set(_DisableWarningAvailible)
foreach(_test ${phi_disabled_warnings_flags})
  string(REPLACE "-" "_" _testName ${_test})
  string(REPLACE "=" "_" _testName ${_testName})
  string(TOUPPER ${_testName} _testName)

  phi_check_cxx_compiler_flag(${PHI_FLAG_PREFIX_CHAR}${_test} "PHI_HAS_FLAG_${_testName}")

  if(PHI_HAS_FLAG_${_testName})
    list(APPEND _DisableWarningAvailible ${PHI_FLAG_PREFIX_CHAR}${_test})
  endif()
endforeach(_test)

# Check required flags
set(_CheckRequiredFlags)
foreach(_test ${phi_check_required_flags})
  string(REPLACE "-" "_" _testName ${_test})
  string(REPLACE "=" "_" _testName ${_testName})
  string(TOUPPER ${_testName} _testName)

  phi_check_cxx_compiler_flag(${PHI_FLAG_PREFIX_CHAR}${_test} "PHI_HAS_FLAG_${_testName}")

  if(PHI_HAS_FLAG_${_testName})
    list(APPEND _CheckRequiredFlagsAvailible ${PHI_FLAG_PREFIX_CHAR}${_test})
  endif()
endforeach(_test)

# GCC does except all -Wno-xxxx flags even if it can't handle them
if(PHI_COMPILER_GCC)
  set(_DisableWarningAvailible "-Wno-unused-function")

  if(GCC_COMPILER_VERSION VERSION_LESS "9.0.0")
    list(REMOVE_ITEM _WarningsAvailible "-Wuseless-cast")
  endif()
endif()

# Clang only requires -Weverything
if(PHI_COMPILER_CLANG)
  set(_WarningsAvailible "-Weverything")
endif()

# Emscriptens warning suppression for some reason doesn't work correctly so we disable all
if(PHI_PLATFORM_EMSCRIPTEN)
  set(_WarningsAvailible "-Wundef")
  set(_DisableWarningAvailible
      "${_DisableWarningAvailible};-Wno-assume;-Wno-unused-result;-Wno-deprecated-volatile;-Wno-deprecated-declarations"
  )
endif()

# Disable -Wstrict-overflow on gcc with asan, ubsan or tsan
if(PHI_COMPILER_GCC
   AND (ENABLE_SANITIZER_ADDRESS
        OR ENABLE_SANITIZER_UNDEFINED_BEHAVIOR
        OR ENABLE_SANITIZER_THREAD))
  list(REMOVE_ITEM _WarningsAvailible "-Wstrict-overflow=5")
endif()

# from here:
#
# https://github.com/lefticus/cppbestpractices/blob/master/02-Use_the_Tools_Available.md

# Enable warnings for project
function(phi_target_set_warnings)
  # Command line arguments
  cmake_parse_arguments(warn "WARNINGS_AS_ERRORS;PEDANTIC" "TARGET" "" ${ARGN})

  # Check required arguments
  if(NOT DEFINED warn_TARGET)
    phi_error("phi_target_set_warnings: You must specify a target with TARGET")
  endif()

  # Check target actually exists
  if(NOT TARGET ${warn_TARGET})
    phi_error(
      "phi_target_set_warnings: The specified target \"${warn_TARGET}\" doesn't seem to exist")
  endif()

  # Get target type
  get_property(
    target_type
    TARGET ${warn_TARGET}
    PROPERTY TYPE)

  if("${target_type}" STREQUAL "INTERFACE_LIBRARY")
    set(visibility_scope INTERFACE)
  else()
    set(visibility_scope PRIVATE)
  endif()

  # Set warnings
  if(PHI_COMPILER_APPLECLANG)
    # TODO: Since warning suppression for AppleClang seems to not work correctly we disable all
    # warnings
    target_compile_options(${warn_TARGET} ${visibility_scope} "-w")
  else()
    target_compile_options(${warn_TARGET} ${visibility_scope} ${_WarningsAvailible})
    target_compile_options(${warn_TARGET} ${visibility_scope} ${_DisableWarningAvailible})
  endif()

  # Add optional warnings as errors
  if(warn_WARNINGS_AS_ERRORS
     AND NOT PHI_COMPILER_APPLECLANG
     AND NOT PHI_COMPILER_EMCC)
    # TODO: Warning suppression for AppleClang and Emscripten seems to not work correctly so we
    # disable warnings as errors to be able to build at all

    # Give a warnings if no warnigns as error flag is available
    if(NOT _WarningAsErrorAvailible)
      phi_warn(
        "phi_target_set_warnings: Trying to enable warnings as errors but no supported flags we're found while processing target ${warn_TARGET}"
      )
    endif()

    target_compile_options(${warn_TARGET} ${visibility_scope} ${_WarningAsErrorAvailible})
    target_compile_definitions(${warn_TARGET} ${visibility_scope} "PHI_CONFIG_WARNINGS_AS_ERRORS")
  endif()

  # Add optional pedantic
  if(warn_PEDANTIC)
    target_compile_options(${warn_TARGET} ${visibility_scope} ${_PedanticAvailible})
    target_compile_definitions(${warn_TARGET} ${visibility_scope} "PHI_CONFIG_PEDANTIC_WARNINGS")
  endif()
endfunction()
