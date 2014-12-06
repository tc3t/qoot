############################################################################
# CMakeLists.txt file for building ROOT montecarlo/vmc package
# This is a modified version of a file originally by Pere Mato, CERN
############################################################################

if(vmc)

ROOT_USE_PACKAGE(montecarlo/eg)
ROOT_USE_PACKAGE(geom/geom)

ROOT_STANDARD_LIBRARY_PACKAGE(VMC DEPENDENCIES EG Geom MathCore)

endif()
