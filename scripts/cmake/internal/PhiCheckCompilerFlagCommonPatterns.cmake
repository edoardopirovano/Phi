phi_include_guard()

macro(phi_check_compiler_flag_common_patterns _VAR)
  set(${_VAR}
      FAIL_REGEX
      "[Uu]nrecogni[sz]ed .*option" # GNU, NAG, Fujitsu
      FAIL_REGEX
      "switch .* is no longer supported" # GNU
      FAIL_REGEX
      "unknown .*option" # Clang
      FAIL_REGEX
      "optimization flag .* not supported" # Clang
      FAIL_REGEX
      "unknown argument ignored" # Clang (cl)
      FAIL_REGEX
      "ignoring unknown option" # MSVC, Intel
      FAIL_REGEX
      "warning D9002" # MSVC, any lang
      FAIL_REGEX
      "option.*not supported" # Intel
      FAIL_REGEX
      "invalid argument .*option" # Intel
      FAIL_REGEX
      "ignoring option .*argument required" # Intel
      FAIL_REGEX
      "ignoring option .*argument is of wrong type" # Intel
      FAIL_REGEX
      "[Uu]nknown option" # HP
      FAIL_REGEX
      "[Ww]arning: [Oo]ption" # SunPro
      FAIL_REGEX
      "command option .* is not recognized" # XL
      FAIL_REGEX
      "command option .* contains an incorrect subargument" # XL
      FAIL_REGEX
      "Option .* is not recognized.  Option will be ignored." # XL
      FAIL_REGEX
      "not supported in this configuration. ignored" # AIX
      FAIL_REGEX
      "File with unknown suffix passed to linker" # PGI
      FAIL_REGEX
      "[Uu]nknown switch" # PGI
      FAIL_REGEX
      "WARNING: unknown flag:" # Open64
      FAIL_REGEX
      "Incorrect command line option:" # Borland
      FAIL_REGEX
      "Warning: illegal option" # SunStudio 12
      FAIL_REGEX
      "[Ww]arning: Invalid suboption" # Fujitsu
      FAIL_REGEX
      "An invalid option .* appears on the command line" # Cray
      FAIL_REGEX
      "WARNING: invalid compiler option" # TI armcl
  )
endmacro()
