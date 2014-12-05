############################################################################
# CMakeLists.txt file for building ROOT examples/basicQootExample
############################################################################

ROOT_EXECUTABLE(basicQootExample basicQootExample.cpp LIBRARIES Core GQt QtGui QtGed)
set_target_properties(basicQootExample PROPERTIES FOLDER Examples)

if(qt5)
  qt5_use_modules(basicQootExample Widgets)
endif()
