############################################################################
# CMakeLists.txt file for building ROOT math/physics package
############################################################################

if(quadp)

ROOT_USE_PACKAGE(math/matrix)

ROOT_STANDARD_LIBRARY_PACKAGE(Quadp DEPENDENCIES Matrix)

endif()
