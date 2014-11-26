############################################################################
# CMakeLists.txt file for building ROOT core/newdelete package
############################################################################

if(new)

ROOT_USE_PACKAGE(io/io)
ROOT_LINKER_LIBRARY(New *.cxx DEPENDENCIES Core Cint)
ROOT_INSTALL_HEADERS()

endif()

