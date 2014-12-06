############################################################################
# CMakeLists.txt file for building ROOT gui/qtroot package
# This modified version of a file originally by Pere Mato, CERN
############################################################################

ROOT_USE_PACKAGE(gui/gui)
ROOT_USE_PACKAGE(graf2d/qt)

if(qt4)
  include(${QT_USE_FILE})
endif()

#if(qt4)
#  QT4_WRAP_CPP(mocfiles inc/TQtContextMenuImp.h inc/TQtObjectDialog.h
#				inc/TQtCanvasImp.h inc/TQtCanvasWidget.h inc/TQtToolBar.h
#				inc/TQtZoomPadWidget.h)
#endif()
#if(qt5)
#  QT5_WRAP_CPP(mocfiles inc/TQtContextMenuImp.h inc/TQtObjectDialog.h
#				inc/TQtCanvasImp.h inc/TQtCanvasWidget.h inc/TQtToolBar.h
#				inc/TQtZoomPadWidget.h)
#endif()

ROOT_GENERATE_DICTIONARY(G__QtRoot TQtRootGuiFactory.h LINKDEF LinkDef.h)
#ROOT_GENERATE_ROOTMAP(QtRoot LINKDEF LinkDef.h DEPENDENCIES Gui GQt )
#ROOT_LINKER_LIBRARY(QtRoot *.cxx ${mocfiles} G__QtRoot.cxx LIBRARIES Core DEPENDENCIES Gui GQt)
ROOT_GENERATE_ROOTMAP(QtRoot LINKDEF LinkDef.h DEPENDENCIES GQt )
ROOT_LINKER_LIBRARY(QtRoot *.cxx ${mocfiles} G__QtRoot.cxx LIBRARIES Core DEPENDENCIES GQt)


if(qt5)
  qt5_use_modules(QtRoot Widgets PrintSupport)
endif()

ROOT_INSTALL_HEADERS()
