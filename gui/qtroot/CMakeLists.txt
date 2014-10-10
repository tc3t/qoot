############################################################################
# CMakeLists.txt file for building ROOT gui/qtroot package
# @author Pere Mato, CERN
############################################################################

ROOT_USE_PACKAGE(gui/gui)
ROOT_USE_PACKAGE(graf2d/qt)

if(qt4)
  include(${QT_USE_FILE})
endif()

ROOT_GENERATE_DICTIONARY(G__QtRoot *.h LINKDEF LinkDef.h)
ROOT_GENERATE_ROOTMAP(QtRoot LINKDEF LinkDef.h DEPENDENCIES Gui GQt )
ROOT_LINKER_LIBRARY(QtRoot *.cxx G__QtRoot.cxx LIBRARIES Core DEPENDENCIES Gui GQt)

if(qt5)
  qt5_use_modules(QtRoot Widgets)
endif()

ROOT_INSTALL_HEADERS()
