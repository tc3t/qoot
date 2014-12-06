############################################################################
# CMakeLists.txt file for building ROOT gui/qtgui package
# This is a modified version of a file originally by Pere Mato, CERN
############################################################################

ROOT_USE_PACKAGE(graf2d/qt)

if(qt4)
  include(${QT_USE_FILE})
endif()

file(GLOB_RECURSE HEADERS "*.h")

if(qt4)
  QT4_WRAP_CPP(mocfiles inc/TQtContextMenuImp.h inc/TQtObjectDialog.h
				inc/TQtCanvasImp.h inc/TQtCanvasWidget.h inc/TQtToolBar.h
				inc/TQtZoomPadWidget.h inc/TQtRootBrowserImp.h inc/TQtRootBrowserAction.h
				inc/TQtBrowserImp.h inc/TQtIconBrowserImp.h inc/TQtColorSelect.h
				inc/TQtColorSelectButton.h inc/TQtCommandPlugin.h inc/TQtControlBarImp.h
				inc/TQtFloatSlider.h inc/TQtFloatSpinBox.h inc/TQtCustomizeCanvasMenu.h
				inc/TQtInspectImp.h inc/TQtMarkerSelectButton.h inc/TQtPatternSelect.h
				inc/TQtPatternSelectButton.h inc/TQtMarkerSelect.h inc/TQtRootCommandCombo.h
				inc/TQtTextEditor.h inc/TQtStyleComboBox.h)
endif()
if(qt5)
  QT5_WRAP_CPP(mocfiles inc/TQtContextMenuImp.h inc/TQtObjectDialog.h
				inc/TQtCanvasImp.h inc/TQtCanvasWidget.h inc/TQtToolBar.h
				inc/TQtZoomPadWidget.h inc/TQtRootBrowserImp.h inc/TQtRootBrowserAction.h
				inc/TQtBrowserImp.h inc/TQtIconBrowserImp.h inc/TQtColorSelect.h
				inc/TQtColorSelectButton.h inc/TQtCommandPlugin.h inc/TQtControlBarImp.h
				inc/TQtFloatSlider.h inc/TQtFloatSpinBox.h inc/TQtCustomizeCanvasMenu.h
				inc/TQtInspectImp.h inc/TQtMarkerSelectButton.h inc/TQtPatternSelect.h
				inc/TQtPatternSelectButton.h inc/TQtMarkerSelect.h inc/TQtRootCommandCombo.h
				inc/TQtTextEditor.h inc/TQtStyleComboBox.h)
endif()

ROOT_GENERATE_DICTIONARY(G__QtGui TBrowserCustom.h TEmbeddedPad.h
						TQtColorSelect.h TQtMarkerSelect.h TQtPatternSelect.h
						TQtPixmapBox.h TQtGuiFactory.h
						LINKDEF LinkDef.h)
#ROOT_GENERATE_ROOTMAP(QtGui LINKDEF LinkDef.h DEPENDENCIES Gui GQt )
#ROOT_LINKER_LIBRARY(QtGui *.cxx ${HEADERS} ${mocfiles} G__QtGui.cxx LIBRARIES Core DEPENDENCIES Gui GQt)
ROOT_GENERATE_ROOTMAP(QtGui LINKDEF LinkDef.h DEPENDENCIES GQt )
ROOT_LINKER_LIBRARY(QtGui *.cxx ${HEADERS} ${mocfiles} G__QtGui.cxx LIBRARIES Core DEPENDENCIES GQt)


if(qt5)
  qt5_use_modules(QtGui Widgets PrintSupport)
endif()

ROOT_INSTALL_HEADERS()
