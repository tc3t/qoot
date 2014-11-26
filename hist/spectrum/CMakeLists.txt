############################################################################
# CMakeLists.txt file for building ROOT hist/spectrum package
############################################################################

if(spectrum)

ROOT_USE_PACKAGE(hist/hist)
ROOT_STANDARD_LIBRARY_PACKAGE(Spectrum DEPENDENCIES Hist Matrix)

endif()
