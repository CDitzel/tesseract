cmake_minimum_required (VERSION 3.5.1 FATAL_ERROR)

# ;-list of directories specifying a search path for CMake modules to be loaded by the the include() or find_package() commands before checking the default modules that come with CMake. 
# By default it is empty, it is intended to be set by the project
list(INSERT CMAKE_MODULE_PATH 0 ${CMAKE_SOURCE_DIR}/cmake/Modules)
# Enable Colour Support
include(ColourSupport)

# caching compilation outputs (i.e. object files) and reusing them instead of compiling the source again 
# if it gets a compilation request it has seen before and cached
find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    message(STATUS ${ColourYellow} "-> Launching compiler through ccache and caching compilation outputs" ${ColourReset})
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

project(radarProject VERSION 1.0 LANGUAGES CXX)

# Overwrite default system install paths and Set Local Install Path 
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    message(STATUS ${ColourYellow} "-> Setting default CMAKE_INSTALL_PREFIX path: usr/local to ${CMAKE_SOURCE_DIR}/obj" ${ColourReset})
    set(CMAKE_INSTALL_PREFIX "${CMAKE_SOURCE_DIR}/obj" CACHE STRING "The path to use for make install" FORCE)
endif()

# Set Default Build Type
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message(STATUS ${ColourYellow}  "-> Setting build type to 'Debug' as none was specified." ${ColourReset})
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)
    # Set the possible values of build type for cmake-gui
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()

# the toolbox is built standalone
set(CMAKE_INSTALL_DIR ${PROJECT_SOURCE_DIR}/obj)
set(CMAKE_INSTALL_PREFIX ${CMAKE_INSTALL_DIR})
# add our local path to the runtime path
SET(CMAKE_INSTALL_RPATH "$ORIGIN:${CMAKE_INSTALL_PREFIX}/lib")
# also add the link paths to the runtime paths
SET(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

# ;-list of directories specifying installation prefixes to be searched by the find_package(), find_program(), find_library(), find_file(), and find_path() commands. 
# Each command will add appropriate subdirectories (like bin, lib, or include) as specified in its own documentation. By default this is empty. It is intended to be set by the project.
set(CMAKE_PREFIX_PATH "/prefix")
set(as_subproject utilities models backend gui)
macro(find_package)
  if(NOT "${ARGV0}" IN_LIST as_subproject)
    _find_package(${ARGV}) # only now call hidden command
  endif()
endmacro()

# instead of add_definitions which is global!
# TODO: make definitions target specific like target_compile_definitions(....)
#ADD_DEFINITIONS(
#    -std=c++1z
#)

option(printCompileFeatures "Whether the cxx compile features are listed" FALSE)
if (printCompileFeatures)
  message(${ColourBoldBlue}"List of compile features: "${ColourReset})
  foreach(feature ${CMAKE_CXX_COMPILE_FEATURES})
    message(STATUS "${ColourBoldRed}${feature}${ColourReset}")
  endforeach()	
endif()

################################################################################
# check for required modules, aka. precompiled binaries
# A package is basically a CMake file that setups a target that you can use
# just as if it was defined in your build-tree itself
################################################################################
find_package(TIFF MODULE REQUIRED)

#find_package(Boost 1.45.0 REQUIRED COMPONENTS filesystem)
# define imported target
#add_library(boost INTERFACE IMPORTED)
#set_property(TARGET boost PROPERTY INTERFACE_INCLUDE_DIRECTORIES ${Boost_INCLUDE_DIR})

  #set(Qt5_DIR "/usr/lib/x86_64-linux-gnu/cmake/Qt5/")
  #message(STATUS "Searching for Qt5 here: " ${Qt5_DIR})
find_package(Qt5 CONFIG REQUIRED COMPONENTS Core Widgets Xml OpenGL Gui)
find_package(GDAL MODULE REQUIRED)
find_package(QGLViewer MODULE REQUIRED)
find_package(OpenGL MODULE REQUIRED)
find_package(OpenCV CONFIG REQUIRED)

# Export compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

    message("before clone")
execute_process(COMMAND git submodule update --init -- extLibs/json WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
    message("After clone")
    
## --> Build libraries and applications  <--
message(STATUS "${ColourCyan}Entering subdirectory extLibs...${ColourReset}")
add_subdirectory(./extLibs)
message(STATUS "${ColourCyan}Entering subdirectory utilities...${ColourReset}")
add_subdirectory(./utilities)
message(STATUS "${ColourCyan}Entering subdirectory models...${ColourReset}")
add_subdirectory(./models)
message(STATUS "${ColourCyan}Entering subdirectory backend...${ColourReset}")
add_subdirectory(./backend)
message(STATUS "${ColourCyan}Entering subdirectory gui...${ColourReset}")
add_subdirectory(./gui)
message(STATUS "${ColourCyan}Entering subdirectory apps...${ColourReset}")
add_subdirectory(./apps)

