phi_include_guard()

include(CMakeParseArguments)
include(internal/PhiError)

function(phi_add_library)
  # Command line arguments
  cmake_parse_arguments(
    al
    "STATIC;SHARED;MODULE;EXCLUDE_FROM_ALL"
    "NAME;FOLDER;ALIAS_TARGET;STANDARD"
    "SOURCES;HEADERS;PUBLIC_LINK_LIBRARIES;PRIVATE_LINK_LIBRARIES;INTERFACE_LINK_LIBRARIES;PUBLIC_INCLUDE_DIRS;PRIVATE_INCLUDE_DIRS;INTERFACE_INCLUDE_DIRS;PUBLIC_DEFINITIONS;PRIVATE_DEFINITIONS;INTERFACE_DEFINITIONS;STATIC_ANALYZER"
    ${ARGN})

  # Check required arguments
  if(NOT DEFINED al_NAME)
    phi_error("phi_add_library: You must specify a name using NAME")
  endif()

  if(NOT DEFINED al_STATIC
     AND NOT DEFINED al_SHARED
     AND NOT DEFINED al_MODULE)
    phi_error("phi_add_library: You must specify either STATIC, SHARED or MODULE")
  endif()

  if(NOT DEFINED al_SOURCES)
    phi_error("phi_add_library: No sources specified using SOURCES")
  endif()

  # Add required arguments
  set(command ${al_NAME})

  # Add type
  if(DEFINED al_STATIC)
    set(command ${command} STATIC)
  elseif(DEFINED al_SHARED)
    set(command ${command} SHARED)
  elseif(DEFINED al_MODULE)
    set(command ${command} MODULE)
  endif()

  # Add optional EXCLUDE_FROM_ALL
  if(DEFINED al_EXCLUDE_FROM_ALL)
    set(command ${command} EXCLUDE_FROM_ALL)
  endif()

  # Ensure all files actually exist on disk
  foreach(file IN LISTS al_SOURCES al_HEADERS)
    if(NOT EXISTS ${file})
      message(
        WARNING "File \"${file}\" doesn't seem to exist while configuring library \"${al_NAME}\"")
    endif()
  endforeach()

  # Add sources and headers
  set(command ${command} ${al_SOURCES} ${al_HEADERS})

  # Add library
  add_library(${command})

  # Set standard
  if(DEFINED al_STANDARD)
    phi_target_set_standard(TARGET ${al_NAME} STANDARD ${al_STANDARD})
  endif()

  # Group source files
  if(${CMAKE_VERSION} VERSION_GREATER "3.7")
    # TREE option was introduced with 3.8
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${al_SOURCES} ${al_HEADERS})
  endif()

  # Add optional alias target
  if(DEFINED al_ALIAS_TARGET)
    add_library("${al_ALIAS_TARGET}" ALIAS ${al_NAME})
  endif()

  # Set optional folder
  if(DEFINED al_FOLDER)
    set_target_properties(${al_NAME} PROPERTIES FOLDER "${al_FOLDER}")
  endif()

  # Add optional private link libraries
  if(DEFINED al_PUBLIC_LINK_LIBRARIES)
    target_link_libraries(${al_NAME} PUBLIC "${al_PUBLIC_LINK_LIBRARIES}")
  endif()

  # Add optional private link libraries
  if(DEFINED al_PRIVATE_LINK_LIBRARIES)
    target_link_libraries(${al_NAME} PRIVATE "${al_PRIVATE_LINK_LIBRARIES}")
  endif()

  # Add optional interface link libraries
  if(DEFINED al_INTERFACE_LINK_LIBRARIES)
    target_link_libraries(${al_NAME} INTERFACE "${al_INTERFACE_LINK_LIBRARIES}")
  endif()

  # Add optional public include directories
  if(DEFINED al_PUBLIC_INCLUDE_DIRS)
    target_include_directories(${al_NAME} PUBLIC "${al_PUBLIC_INCLUDE_DIRS}")
  endif()

  # Add optional private include directories
  if(DEFINED al_PRIVATE_INCLUDE_DIRS)
    target_include_directories(${al_NAME} PRIVATE "${al_PRIVATE_INCLUDE_DIRS}")
  endif()

  # Add optional interface include directories
  if(DEFINED al_INTERFACE_INCLUDE_DIRS)
    target_include_directories(${al_NAME} INTERFACE "${al_INTERFACE_INCLUDE_DIRS}")
  endif()

  # Add optional public compile definition
  if(DEFINED al_PUBLIC_COMPILE_DEFINITIONS)
    target_compile_definitions(${al_NAME} PUBLIC "${al_PUBLIC_COMPILE_DEFINITIONS}")
  endif()

  # Add optional private compile definition
  if(DEFINED al_PRIVATE_COMPILE_DEFINITIONS)
    target_compile_definitions(${al_NAME} PRIVATE "${al_PRIVATE_COMPILE_DEFINITIONS}")
  endif()

  # Add optional interface compile definition
  if(DEFINED al_INTERFACE_COMPILE_DEFINITIONS)
    target_compile_definitions(${al_NAME} INTERFACE "${al_INTERFACE_COMPILE_DEFINITIONS}")
  endif()

  # Enable optional static analyzers
  if(DEFINED al_STATIC_ANALYZER)
    phi_target_use_static_analyzers(TARGET ${al_NAME} ${al_STATIC_ANALYZER})
  endif()
endfunction(phi_add_library)
