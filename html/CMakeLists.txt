############################################################################
# CMakeLists.txt file for building ROOT html package
############################################################################

if(html)

ROOT_USE_PACKAGE(graf2d/gpad)
ROOT_USE_PACKAGE(core/thread)

ROOT_STANDARD_LIBRARY_PACKAGE(Html DEPENDENCIES Graf Thread)

endif()
