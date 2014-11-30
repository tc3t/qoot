############################################################################
# CMakeLists.txt file for building ROOT gui/qtged package
############################################################################

#ROOT_USE_PACKAGE(graf2d/qt)

if(qt4)
  include(${QT_USE_FILE})
endif()

file(GLOB_RECURSE HEADERS "*.h")

if(qt4)
  QT4_WRAP_CPP(mocfiles )
endif()
if(qt5)
  QT5_WRAP_CPP(mocfiles inc/TQtArrowEditor.h inc/TQtAxisEditor.h inc/TQtGedAttFrame.h
               inc/TQtGedEditor.h inc/TQtGedFrames.h inc/TQtGraphEditor.h
               inc/TQtLineEditor.h inc/TQtMarkerEditor.h inc/TQtPadEditor.h)
endif()

ROOT_GENERATE_DICTIONARY(G__QtGed TQtGedAttFrame.h TQtPadEditor.h TQtAxisEditor.h
                        TQtArrowEditor.h TQtLineEditor.h TQtMarkerEditor.h TQtGraphEditor.h
                        TQtGedEditor.h TQtGedFrames.h LINKDEF LinkDef.h)
ROOT_GENERATE_ROOTMAP(QtGed LINKDEF LinkDef.h DEPENDENCIES GQt )
ROOT_LINKER_LIBRARY(QtGed *.cxx ${HEADERS} ${mocfiles} G__QtGed.cxx LIBRARIES Core DEPENDENCIES QtGui GQt)

if(qt5)
  qt5_use_modules(QtGed Widgets PrintSupport)
endif()

ROOT_INSTALL_HEADERS()
