cmake_minimum_required(VERSION 3.5.1 FATAL_ERROR)
project(gui)

QT5_WRAP_CPP(MOC_files
 	include/gui/mainWindow.hpp
        include/gui/imageWidget.hpp
        include/gui/controlWidget.hpp
	include/gui/currentState.hpp
        include/gui/commandButton.hpp
        include/gui/canvas.hpp
        include/gui/radarVoxel.hpp
        include/gui/lidarPoint.hpp
        include/gui/admaDrawer.hpp
        include/gui/splitter.hpp
        include/gui/application.hpp
)

message(STATUS "List of Qt's MOC FILES:")
foreach(file ${MOC_files})
	  message(STATUS "${ColourBoldRed}${file}${ColourReset}")
endforeach()

add_library(${PROJECT_NAME}
  SHARED
    src/mainWindow.cpp
    src/imageWidget.cpp
    src/zoomWidget.cpp
    src/currentState.cpp
    src/controlWidget.cpp
    src/commandButton.cpp
    src/canvas.cpp
    src/radarVoxel.cpp
    src/lidarPoint.cpp
    src/admaDrawer.cpp
    src/originAxisDrawer.cpp
    src/splitter.cpp
    src/application.cpp
    ${MOC_files}
)

#Add an alias so that library can be used inside the build tree, e.g. when testing
add_library(Gui::gui ALIAS ${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)

target_include_directories(${PROJECT_NAME} 
  PUBLIC
    ${PROJECT_SOURCE_DIR}/include
)

target_compile_options(${PROJECT_NAME} 
  PRIVATE 
  #$<$<CXX_COMPILER_ID:GNU>:-Wall>
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

target_link_libraries(${PROJECT_NAME}
  PUBLIC
  ##libQGLViewer-2.7.0
  QGLViewer::QGLViewer
    Utilities::utilities
    Models::models
    Qt5::Widgets
    Qt5::Core
    Qt5::Xml
    Qt5::OpenGL
    Qt5::Gui
    OpenGL::OpenGL
)

install(TARGETS ${PROJECT_NAME} 
	EXPORT guiTargets
	LIBRARY DESTINATION lib
	ARCHIVE DESTINATION lib
	RUNTIME DESTINATION bin
	INCLUDES DESTINATION include
	)

#install(EXPORT guiTargets
#	FILE guiTargets.cmake
#	NAMESPACE Gui::
#	DESTINATION lib/cmake/gui
#	)
