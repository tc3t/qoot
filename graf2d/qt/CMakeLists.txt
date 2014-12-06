############################################################################
# CMakeLists.txt file for building ROOT graf2d/qt package
# This is a modified version of a file originally by Pere Mato, CERN
############################################################################

if(qt5)
  project(GQt)
endif()


ROOT_USE_PACKAGE(gui/gui)
if(qt4)
  include(${QT_USE_FILE})
endif()

set(headers TGQt.h TQtTimer.h TQtApplication.h TQtBrush.h
            TQMimeTypes.h TQtClientFilter.h TQtClientWidget.h TQtWidget.h
            TQtMarker.h TQtTimer.h TQtRootSlot.h TQtPadFont.h)

if(qt4)
  QT4_WRAP_CPP(mocfiles inc/TQtWidget.h inc/TQtEmitter.h inc/TQtClientFilter.h
                      inc/TQtClientGuard.h inc/TQtClientWidget.h inc/TQtTimer.h
                      inc/TQtRootSlot.h)
endif()
if(qt5)
  QT5_WRAP_CPP(mocfiles inc/TQtWidget.h inc/TQtEmitter.h inc/TQtClientFilter.h
                      inc/TQtClientGuard.h inc/TQtClientWidget.h inc/TQtTimer.h
                      inc/TQtRootSlot.h)
endif()

ROOT_GENERATE_DICTIONARY(G__GQt ${headers} LINKDEF LinkDef.h)
ROOT_GENERATE_ROOTMAP(GQt LINKDEF LinkDef.h DEPENDENCIES Gui Gpad Graf Rint)

ROOT_LINKER_LIBRARY(GQt *.cxx ${mocfiles} G__GQt.cxx LIBRARIES ${QT_LIBRARIES} DEPENDENCIES Gpad Graf Rint)
#ROOT_LINKER_LIBRARY(GQt *.cxx ${mocfiles} G__GQt.cxx LIBRARIES ${QT_LIBRARIES} DEPENDENCIES Gui Gpad Graf Rint)

if(qt5)
  qt5_use_modules(GQt Widgets)
endif()

ROOT_INSTALL_HEADERS()
