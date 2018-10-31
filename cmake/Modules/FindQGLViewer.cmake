# FindQGLViewer.cmake
#
# Finds the QGLViewer library
#
# This will define the following variables
#
#    QGLViewer_FOUND
#    QGLViewer_INCLUDE_DIR
#    QGLViewer_LIBRARY_DIR
#
# and the following imported targets
#
#     QGLViewer::QGLViewer
#
# Author: Carsten Ditzel - c.ditzel@gmail.com

find_path(QGLViewer_INCLUDE_DIR
    NAMES 
      qglviewer.h
    PATH
      /usr/include
      /usr/local/include
    PATH_SUFFIXES 
      QGLViewer
)

find_library(QGLViewer_LIBRARY_DIR 
    NAMES 
      qglviewer 
      QGLViewer 
      qglviewer-qt5 
      QGLViewer-qt5
    PATHS 
      /usr/lib
      /usr/local/lib
    PATH_SUFFIXES 
      QGLViewer
)

mark_as_advanced(QGLViewer_FOUND QGLViewer_INCLUDE_DIR QGLViewer_VERSION)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(QGLViewer
  REQUIRED_VARS 
    QGLViewer_INCLUDE_DIR 
    QGLViewer_LIBRARY_DIR
)


if(QGLViewer_FOUND)
    set(QGLViewer_INCLUDE_DIRS ${QGLViewer_INCLUDE_DIR})
    #get_filename_component(QGViewer_INCLUDE_DIRS ${QGLViewer_INCLUDE_DIR} DIRECTORY
endif()

if(QGLViewer_FOUND AND NOT TARGET QGLViewer::QGLViewer)
	add_library(QGLViewer::QGLViewer UNKNOWN IMPORTED)
	set_target_properties(QGLViewer::QGLViewer PROPERTIES
	    IMPORTED_LINK_INTERFACE_LANGUAGES "CXX"
	    IMPORTED_LOCATION "${QGLViewer_LIBRARY_DIR}"
	    INCLUDE_DIRECTORIES "${QGLViewer_INCLUDE_DIR}"
        )  
endif()
