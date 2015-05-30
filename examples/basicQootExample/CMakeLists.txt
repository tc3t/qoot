############################################################################
# CMakeLists.txt file for building ROOT examples/basicQootExample
############################################################################

if(gui)
  ROOT_EXECUTABLE(basicQootExample basicQootExample.cpp LIBRARIES Core Gui GQt QtGui QtGed QtGSI)
else()
  ROOT_EXECUTABLE(basicQootExample basicQootExample.cpp LIBRARIES Core GQt QtGui QtGed)
endif()
set_target_properties(basicQootExample PROPERTIES FOLDER Examples)

if(qt5)
  qt5_use_modules(basicQootExample Widgets)
endif()
