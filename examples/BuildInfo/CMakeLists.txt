# Phi_BuildInfo

if(${CMAKE_VERSION} VERSION_LESS 3.0)
  project(Phi_BuildInfo CXX)
else()
  project(Phi_BuildInfo LANGUAGES CXX)
endif()

include(CompilerOptimizations)

phi_add_executable(
  NAME
  ${PROJECT_NAME}
  SOURCES
  "src/main.cpp"
  PRIVATE_LINK_LIBRARIES
  Phi::Core
  FOLDER
  "Examples")
