cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
project(utilities)
 
add_library(${PROJECT_NAME} SHARED
    src/dataHandler.cpp
    src/subdirectoryHandler.cpp
    #src/auxiliary.inl
    src/publisher.cpp
    src/userInterface.cpp
    src/observer.cpp
    src/typeErasure.cpp
    src/task.cpp
    src/annotations.cpp
    src/archiver.cpp
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(Utilities::utilities ALIAS ${PROJECT_NAME})

# the location of this directory needs to be different depending on whether we are building or installing the library
target_include_directories(${PROJECT_NAME} 
  PUBLIC 
    $<INSTALL_INTERFACE:include>    
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

target_link_libraries(${PROJECT_NAME} 
  PUBLIC
    Qt5::Widgets
    Qt5::Core
    Qt5::Xml
    Qt5::OpenGL
    Qt5::Gui
    nlohmann_json
  PRIVATE
   stdc++fs #Linking to libstdc++fs is required for use of the Filesystem library extensions in <experimental/filesystem>
    TIFF::TIFF
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)

