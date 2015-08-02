############################################################################
# CMakeLists.txt file for building ROOT examples/graphDrawingPerformanceExample
############################################################################

if(gui)
  ROOT_EXECUTABLE(graphDrawingPerformanceExample graphDrawingPerformanceExample.cpp LIBRARIES Core Gui GQt QtGui QtGed QtGSI)
else()
  ROOT_EXECUTABLE(graphDrawingPerformanceExample graphDrawingPerformanceExample.cpp LIBRARIES Core GQt QtGui QtGed)
endif()
set_target_properties(graphDrawingPerformanceExample PROPERTIES FOLDER Examples)

if(qt5)
  qt5_use_modules(graphDrawingPerformanceExample Widgets)
endif()
