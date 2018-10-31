cmake_minimum_required (VERSION 3.5.1 FATAL_ERROR)
project (app)

#find_package(GDAL REQUIRED)
#if(GDAL_FOUND)
#  message(STATUS ${ColourBoldCyan} "Using GDAL (" ${GDAL_VERSION_MAJOR} "." ${GDAL_VERSION_MINOR} "." ${GDAL_VERSION_PATCH} ") at " ${GDAL_INCLUDE_DIR} " :: " ${GDAL_LIBRARIES} ${ColourReset})
#else()
#message(STATUS "${ColourYellow}Install the gdal stuff via \"sudo apt-get install libgdal-dev\" from the linux packages, otherwise the gdalViewer ist not build my honey.${ColourReset}")
#endif()

QT5_WRAP_CPP(MOC_files
   	../gui/include/gui/imageWidget.hpp
   	../gui/include/gui/mainWindow.hpp
   	../gui/include/gui/zoomWidget.hpp
        ../gui/include/gui/currentState.hpp
        ../gui/include/gui/controlWidget.hpp
        ../gui/include/gui/commandButton.hpp
        ../gui/include/gui/application.hpp
)

add_executable(${PROJECT_NAME}
    main.cpp
    ${MOC_files}
)

target_include_directories(${PROJECT_NAME}
    PUBLIC ${PROJECT_BINARY_DIR}
)

set_target_properties(${PROJECT_NAME} PROPERTIES AUTOMOC ON)

target_link_libraries(${PROJECT_NAME}
     PRIVATE
     #libQGLViewer-2.7.0
       QGLViewer::QGLViewer
	Gui::gui
	Backend::backend
	Qt5::Widgets
	Qt5::Core
	Qt5::Xml
	Qt5::OpenGL
	Qt5::Gui
        ${OpenCV_LIBS}
	#${GDAL_LIBRARY}
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

#target_compile_options(${PROJECT_NAME} PRIVATE $<$<CXX_COMPILER_ID:GNU>:-Wall>)

set_target_properties(${PROJECT_NAME} PROPERTIES 
	INSTALL_RPATH "$ORIGIN:${CMAKE_INSTALL_PREFIX}/lib:$ORIGIN/")

install(TARGETS ${PROJECT_NAME}
        DESTINATION bin)
# relative to CMAKE_INSTALL_PREFIX, i.e. ${CMAKE_INSTALL_PREFIX}/bin
