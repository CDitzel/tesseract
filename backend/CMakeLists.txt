cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
project(backend)
 
add_library(${PROJECT_NAME}
  SHARED
  src/command.cpp
  src/coreCommands.cpp
  src/commandRepository.cpp
  src/commandManager.cpp
  src/commandDispatcher.cpp
  src/observerProxies.cpp
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(Backend::backend ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} 
  PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
    Utilities::utilities
    Qt5::Widgets
    Qt5::Core
    Qt5::Xml
    Qt5::OpenGL
    Qt5::Gui
)
#target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)

