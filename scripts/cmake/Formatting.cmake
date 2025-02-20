# Copied from https://github.com/BlueBrain/git-cmake-format/blob/master/FindClangFormat.cmake

# TODO: Rework to allow working on a single target

# TODO: Add function which takes a list of files and adds a global format target which formats these
# files

# TODO: Add a function to add a global cmake-format target

phi_include_guard()

if(NOT PHI_AUTO_FORMATTING)
  phi_trace("Auto formatting disabled")
  return()
endif()

# Clang format
find_program(
  CLANG_FORMAT_EXECUTABLE
  NAMES clang-format
        clang-format-15
        clang-format-14
        clang-format-13
        clang-format-12
        clang-format-11
        clang-format-10
        clang-format-9
        clang-format-8
        clang-format-7
        clang-format-6.0
        clang-format-5.0
        clang-format-4.0
        clang-format-3.9
        clang-format-3.8
        clang-format-3.7
        clang-format-3.6
        clang-format-3.5
        clang-format-3.4
        clang-format-3.3
  DOC "clang-format executable")
mark_as_advanced(CLANG_FORMAT_EXECUTABLE)

# Extract version from command "clang-format -version"
if(CLANG_FORMAT_EXECUTABLE)
  execute_process(
    COMMAND ${CLANG_FORMAT_EXECUTABLE} -version
    OUTPUT_VARIABLE clang_format_version
    ERROR_QUIET OUTPUT_STRIP_TRAILING_WHITESPACE)

  if(clang_format_version MATCHES "^clang-format version .*")
    # clang_format_version sample: "clang-format version 3.9.1-4ubuntu3~16.04.1
    # (tags/RELEASE_391/rc2)"
    string(REGEX REPLACE "clang-format version ([.0-9]+).*" "\\1" CLANG_FORMAT_VERSION
                         "${clang_format_version}")
    # CLANG_FORMAT_VERSION sample: "3.9.1"

    # Extract version components
    string(REPLACE "." ";" clang_format_version "${CLANG_FORMAT_VERSION}")
    list(LENGTH clang_format_version CLANG_FORMAT_VERSION_COUNT)
    if(CLANG_FORMAT_VERSION_COUNT GREATER 0)
      list(GET clang_format_version 0 CLANG_FORMAT_VERSION_MAJOR)
    else()
      set(CLANG_FORMAT_VERSION_MAJOR 0)
    endif()
    if(CLANG_FORMAT_VERSION_COUNT GREATER 1)
      list(GET clang_format_version 1 CLANG_FORMAT_VERSION_MINOR)
    else()
      set(CLANG_FORMAT_VERSION_MINOR 0)
    endif()
    if(CLANG_FORMAT_VERSION_COUNT GREATER 2)
      list(GET clang_format_version 2 CLANG_FORMAT_VERSION_PATCH)
    else()
      set(CLANG_FORMAT_VERSION_PATCH 0)
    endif()
  endif()
  unset(clang_format_version)
endif()

if(CLANG_FORMAT_EXECUTABLE)
  set(CLANG_FORMAT_FOUND TRUE)

  phi_log(
    "Found clang-format version ${CLANG_FORMAT_VERSION_MAJOR}.${CLANG_FORMAT_VERSION_MINOR}.${CLANG_FORMAT_VERSION_PATCH} at ${CLANG_FORMAT_EXECUTABLE}"
  )
else()
  set(CLANG_FORMAT_FOUND FALSE)
endif()

# Setup targets
file(GLOB_RECURSE ALL_SOURCE_FILES "${CMAKE_SOURCE_DIR}/examples/*.[ch]pp"
     "${CMAKE_SOURCE_DIR}/libs/*.[ch]pp" "${CMAKE_SOURCE_DIR}/tests/*.[ch]pp"
     "${CMAKE_SOURCE_DIR}/tools/*.[ch]pp")
list(LENGTH ALL_SOURCE_FILES NUMBER_OF_SOURCES)

add_custom_target(
  clang-format
  COMMENT "Running clang-format on ${NUMBER_OF_SOURCES} source files"
  COMMAND ${CLANG_FORMAT_EXECUTABLE} -style=file -i ${ALL_SOURCE_FILES})

# CMake format
find_program(
  CMAKE_FORMAT_EXECUTABLE
  NAMES cmake-format
  DOC "cmake-format executable")
mark_as_advanced(CMAKE_FORMAT_EXECUTABLE)

if(CMAKE_FORMAT_EXECUTABLE)
  set(CMAKE_FORMAT_FOUND TRUE)

  phi_log("Found cmake-format at ${CMAKE_FORMAT_EXECUTABLE}")
else()
  set(CMAKE_FORMAT_FOUND FALSE)
endif()

# Setup targets
file(
  GLOB_RECURSE
  ALL_CMAKEFILES
  "${CMAKE_SOURCE_DIR}/docs/CMakeLists.txt"
  "${CMAKE_SOURCE_DIR}/examples/CMakeLists.txt"
  "${CMAKE_SOURCE_DIR}/examples/*/CMakeLists.txt"
  "${CMAKE_SOURCE_DIR}/libs/CMakeLists.txt"
  "${CMAKE_SOURCE_DIR}/libs/*/CMakeLists.txt"
  "${CMAKE_SOURCE_DIR}/scripts/cmake/*.cmake"
  "${CMAKE_SOURCE_DIR}/tests/CMakeLists.txt"
  "${CMAKE_SOURCE_DIR}/test/*/CMakeLists.txt"
  "${CMAKE_SOURCE_DIR}/CMakeLists.txt")
list(LENGTH ALL_CMAKEFILES NUMBER_OF_CMAKEFILES)

add_custom_target(
  cmake-format
  COMMENT "Running cmake-format on ${NUMBER_OF_CMAKEFILES} CMake files"
  COMMAND ${CMAKE_FORMAT_EXECUTABLE} -i ${ALL_CMAKEFILES})

# Format target
if(CLANG_FORMAT_FOUND OR CMAKE_FORMAT_FOUND)
  add_custom_target(format ALL)
  phi_log("Auto formatting enabled")
else()
  add_custom_target(format)
  phi_log("Auto formatting disabled")
endif()

# Add dependencies
if(CLANG_FORMAT_FOUND)
  add_dependencies(format clang-format)
endif()
if(CMAKE_FORMAT_FOUND)
  add_dependencies(format cmake-format)
endif()

set_target_properties(clang-format PROPERTIES FOLDER "Tools/Formatting")
set_target_properties(cmake-format PROPERTIES FOLDER "Tools/Formatting")
set_target_properties(format PROPERTIES FOLDER "Tools/Formatting")
