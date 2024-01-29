cmake_minimum_required(VERSION 3.16)
project(Game)
 
include(CheckCXXCompilerFlag)
include(CheckCXXSourceCompiles)
include(CheckIncludeFile)
include(CheckIncludeFiles)
 
find_package(CopperSpice REQUIRED)

find_package(nlohmann_json QUIET)
if(NOT nlohmann_json_FOUND)
include(FetchContent)
FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG        v3.11.2
)
FetchContent_MakeAvailable(nlohmann_json)
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I/root/CopperspiceSpiel/build/_deps/nlohmann_json-src/include")

# file locations for installing
if (CMAKE_SYSTEM_NAME MATCHES "Darwin")
  include(GNUInstallDirs)
 
  # where libraries are located relative to the executable
  set(CMAKE_INSTALL_RPATH "@executable_path/../Resources")
 
elseif(CMAKE_SYSTEM_NAME MATCHES "(Linux|OpenBSD|FreeBSD)")
  include(GNUInstallDirs)
  set(CMAKE_INSTALL_RPATH "\$ORIGIN")
 
elseif(MSVC)
   # use defaults
 
elseif(CMAKE_SYSTEM_NAME MATCHES "Windows")
   set(CMAKE_INSTALL_SYSTEM_RUNTIME_DESTINATION .)
   include(${CopperSpice_DIR}/InstallMinGW.cmake)
 
endif()
 
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_INCLUDE_DIRECTORIES_BEFORE ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 17)
 
# location for building binary files
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
 
list(APPEND PROJECT_SOURCES
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)
 
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
 
if(CMAKE_SYSTEM_NAME MATCHES "Windows")
  target_link_libraries(${PROJECT_NAME}
    PRIVATE
    netapi32 
    mpr
   -mwindows
   nlohmann_json::nlohmann_json
  )
endif()
 
target_link_libraries(${PROJECT_NAME}
  PRIVATE
  CopperSpice::CsCore
  CopperSpice::CsGui
)
 
install(TARGETS ${PROJECT_NAME} DESTINATION .)
cs_copy_library(CsCore)
cs_copy_library(CsGui)
 
# installs the platform Gui plugin
cs_copy_plugins(CsGui)