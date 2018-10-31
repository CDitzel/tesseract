cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
project(models)

add_library(${PROJECT_NAME}
 SHARED
   src/sensorModels.cpp
   src/radarModel.cpp
   src/imageModel.cpp
   src/admaModel.cpp
   src/lidarModel.cpp
   src/stereoModel.cpp
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(Models::models ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} 
  PUBLIC 
    ${PROJECT_SOURCE_DIR}/include
)


target_link_libraries(${PROJECT_NAME}
  PUBLIC
    Utilities::utilities
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

install(TARGETS ${PROJECT_NAME} LIBRARY DESTINATION lib)
