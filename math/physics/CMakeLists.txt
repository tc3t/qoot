############################################################################
# CMakeLists.txt file for building ROOT math/physics package
############################################################################

if(physics)

ROOT_USE_PACKAGE(hist/hist)
ROOT_USE_PACKAGE(math/matrix)
ROOT_USE_PACKAGE(math/mathcore)

ROOT_STANDARD_LIBRARY_PACKAGE(Physics DEPENDENCIES Matrix MathCore)

endif()
