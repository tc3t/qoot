############################################################################
# CMakeLists.txt file for building ROOT math/matrix package
############################################################################

if(matrix)

ROOT_USE_PACKAGE(math/mathcore)

ROOT_STANDARD_LIBRARY_PACKAGE(Matrix DEPENDENCIES MathCore)

endif()
