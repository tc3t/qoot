############################################################################
# CMakeLists.txt file for building ROOT cint/cint package
############################################################################

#---Defining the source files to be compiled---------------------------------------------------------
file(GLOB CINT_C_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.c)
set(CINT_C_SRCS ${CINT_C_SRCS} 
  ${CMAKE_CURRENT_SOURCE_DIR}/main/G__setup.c
  ${CMAKE_CURRENT_SOURCE_DIR}/src/config/strlcpy.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/config/strlcpy.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/config/strlcat.c 
  ${CMAKE_CURRENT_SOURCE_DIR}/src/config/snprintf.c )

file(GLOB CINT_CXX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cxx)
list(APPEND CINT_CXX_SRCS  
            ${CMAKE_CURRENT_SOURCE_DIR}/src/dict/Apiif.cxx
            ${CMAKE_CURRENT_SOURCE_DIR}/src/dict/stdstrct.cxx)
            
list(REMOVE_ITEM CINT_CXX_SRCS
            ${CMAKE_CURRENT_SOURCE_DIR}/src/config/sunos.cxx 
            ${CMAKE_CURRENT_SOURCE_DIR}/src/config/macos.cxx 
            ${CMAKE_CURRENT_SOURCE_DIR}/src/config/winnt.cxx 
            ${CMAKE_CURRENT_SOURCE_DIR}/src/config/newsos.cxx )

if(ROOT_PLATFORM STREQUAL linux)  
  list(APPEND CINT_CXX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/dict/libstrm.cxx )
elseif(ROOT_PLATFORM STREQUAL macosx)
  list(APPEND CINT_CXX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/dict/libstrm.cxx )
elseif(ROOT_PLATFORM STREQUAL win32)
  list(APPEND CINT_CXX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/config/winnt.cxx)
  if(VC_MAJOR EQUAL 19)
    list(APPEND CINT_CXX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/dict/vc14strm.cxx)
  elseif(VC_MAJOR EQUAL 18)
    list(APPEND CINT_CXX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/dict/vc12strm.cxx)
  elseif(VC_MAJOR EQUAL 17)
    list(APPEND CINT_CXX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/dict/vc11strm.cxx)
  elseif(VC_MAJOR EQUAL 16)
    list(APPEND CINT_CXX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/dict/vc10strm.cxx)
  elseif(VC_MAJOR EQUAL 13 AND VC_MINOR EQUAL 10)
    list(APPEND CINT_CXX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/dict/vc7strm.cxx)
  elseif(VC_MAJOR LESS 14)	
    list(APPEND CINT_CXX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/dict/vc7strm.cxx)
  else()
    list(APPEND CINT_CXX_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/dict/iccstrm.cxx)
  endif()
endif()  

#---Add/remove sourcefiles depending on the platform---------------------------------------------------
include(${CMAKE_CURRENT_SOURCE_DIR}/PlatformDependentSettings.cmake)
set(CINT_SRCS ${CINT_C_SRCS} ${CINT_CXX_SRCS})
list(REMOVE_ITEM CINT_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/src/loadfile.cxx)

#---Change compile settings depending on the platform--------------------------------------------------
set(CINT_CXX_DEFINITIONS "${CINT_CXX_DEFINITIONS} -DG__HAVE_CONFIG -DG__NOMAKEINFO -DG__CINTBODY")
set(CINT_C_DEFINITIONS "${CINT_C_DEFINITIONS} -DG__HAVE_CONFIG -DG__NOMAKEINFO -DG__CINTBODY")
add_definitions(${CINT_CXX_DEFINITIONS})                
                
#---Version of loadfile.cxx with different compiler settings needed for the cint_tmp executable--------
add_custom_command(OUTPUT loadfile_tmp.cxx
                   COMMAND cmake -E copy_if_different ${CMAKE_CURRENT_SOURCE_DIR}/src/loadfile.cxx loadfile_tmp.cxx )
set_source_files_properties(loadfile_tmp.cxx  COMPILE_FLAGS "-UR__HAVE_CONFIG -DROOTBUILD ${CINT_TMP_INC}")

#---Generate and/or copy the iosenum headerfile to the include directory-------------------------------
if(${CLANG_MAJOR} GREATER 2)
  set(IOSENUM_H ${PROJECT_SOURCE_DIR}/cint/iosenum/iosenum.${ROOT_ARCHITECTURE}3)
elseif(${GCC_MAJOR} GREATER 3)
  set(IOSENUM_H ${PROJECT_SOURCE_DIR}/cint/iosenum/iosenum.${ROOT_ARCHITECTURE}3)
else()
  set(IOSENUM_H ${PROJECT_SOURCE_DIR}/cint/iosenum/iosenum.${ROOT_ARCHITECTURE})
endif()

if(EXISTS ${IOSENUM_H})
  #---copy headerfile from source directory to build directory
  add_custom_command(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/iosenum.h
    COMMAND cmake -E copy ${IOSENUM_H} ${CMAKE_CURRENT_BINARY_DIR}/iosenum.h)
else()
  #---create header file using cint
  add_custom_command(
     OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/iosenum.h
     COMMAND echo 
     COMMAND cint_tmp -I${CMAKE_CURRENT_SOURCE_DIR}/include 
       -I${CMAKE_CURRENT_SOURCE_DIR}/stl -I${CMAKE_CURRENT_SOURCE_DIR}/lib 
       -I${HEADER_OUTPUT_PATH} ${PROJECT_SOURCE_DIR}/cint/iosenum/iosenum.cxx
     DEPENDS  ${CINTDEP}
  )
endif()

add_custom_target(IOSENUM_H
   COMMAND cmake -E copy ${CMAKE_CURRENT_BINARY_DIR}/iosenum.h  ${HEADER_OUTPUT_PATH}/iosenum.h
   DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/iosenum.h 
)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES ${HEADER_OUTPUT_PATH}/iosenum.h)
install(FILES ${HEADER_OUTPUT_PATH}/iosenum.h DESTINATION ${CMAKE_INSTALL_CINTINCDIR}/cint/include)

set_target_properties(IOSENUM_H PROPERTIES FOLDER Builtins)
#---special compile settings------------------------------------------------------------------------------
foreach(_value ${CINT_SRCS})
  get_filename_component(_name ${_value} NAME_WE) 
  string(REGEX MATCH "^.*strm" _bla ${_name})
  set(comp_flags_tmp)
  if(_bla)
    set(comp_flags_tmp "-I${CMAKE_CURRENT_SOURCE_DIR}/lib/${_bla}")
    set_source_files_properties(${_value} COMPILE_FLAGS "-I${CMAKE_CURRENT_SOURCE_DIR}/lib/${_bla}")
  endif()
  if(${_name} MATCHES libstrm)
    set_source_files_properties(${_value} COMPILE_FLAGS "${comp_flags_tmp} -I${CMAKE_CURRENT_SOURCE_DIR}/lib/stream")    
  elseif(${_name} MATCHES sun5strm)
    set_source_files_properties(${_value} COMPILE_FLAGS "${comp_flags_tmp} -I${CMAKE_CURRENT_SOURCE_DIR}/lib/sunstrm")    
  elseif(${_name} MATCHES vcstrm)
    set_source_files_properties(${_value} COMPILE_FLAGS "${comp_flags_tmp} -I${CMAKE_CURRENT_SOURCE_DIR}/lib/vcstream")    
  elseif(${_name} MATCHES gcc4strm)
    set_source_files_properties(${_value} COMPILE_FLAGS "${comp_flags_tmp} -Wno-strict-aliasing")
  elseif(${_name} MATCHES stdstrct)
    set_source_files_properties(${_value} COMPILE_FLAGS "-I${CMAKE_CURRENT_SOURCE_DIR}/lib/stdstrct")
  endif()
endforeach()

if(ROOT_PLATFORM STREQUAL win32 AND NOT (MSVC_VERSION LESS 1800))
  # turn optimizations off for this file, otherwise we get a compilation error with
  # Visual C++ 12 (2013) in Release mode (due to the /O2 flag):
  # g__cfunc.c(835) : error C2099: initializer is not a constant
  set_source_files_properties(src/g__cfunc.c COMPILE_FLAGS "-Od")
endif()

include_directories(src)
if(MACOSX_TMPNAM_DEPRECATED)
  add_definitions(-Wno-deprecated-declarations)
  add_definitions(-Wno-constant-logical-operand)
endif()

ROOT_ADD_CXX_FLAG(CMAKE_CXX_FLAGS -Wno-strict-aliasing)

#---Define libraries and executables to be build------------------------------------------------------------

add_definitions(-DG__HAVE_CONFIG -DG__NOMAKEINFO -DG__CINTBODY)
# This static library is needed due to some cmake internal feature/bug
# If a source file is added to several programs the file is compiled
# more than once. In case of cint, cint_tmp, rootcint and rootcint_tmp
# this would need 4 compilations of many files.
# Creating a static library with all the files which are equal in all
# executables/libraries overcomes the problem.
# TODO: Check if this works on all platforms 
add_library(Cint_static STATIC ${CINT_SRCS})
add_dependencies(Cint_static IOSENUM_H move_headers)
if(WIN32)
  set_target_properties(Cint_static PROPERTIES STATIC_LIBRARY_FLAGS -ignore:4221)
endif()

#---Create the shared Cint library--------------------------------------------------------------------------
ROOT_LINKER_LIBRARY(Cint loadfile.cxx LIBRARIES Cint_static ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS})

#---Create the cint cint executable-------------------------------------------------------------------------
if(cint_exe)
    ROOT_EXECUTABLE(cint.exe ${CMAKE_CURRENT_SOURCE_DIR}/main/cppmain.cxx LIBRARIES Cint NOINSTALL)
endif()

#---Create the temporary cint_tmp executable----------------------------------------------------------------
ROOT_EXECUTABLE(cint_tmp ${CMAKE_CURRENT_SOURCE_DIR}/main/cppmain.cxx 
                         ${CMAKE_CURRENT_BINARY_DIR}/loadfile_tmp.cxx LIBRARIES Cint_static ${CMAKE_THREAD_LIBS_INIT} ${CMAKE_DL_LIBS} NOINSTALL)

#---Trick to avoid building all dictionaties when CINT is changed-------------------------------------------
add_custom_target(CINTTARGET DEPENDS cint_tmp)

#---Create the makecint executable--------------------------------------------------------------------------
ROOT_EXECUTABLE(makecint ${CMAKE_SOURCE_DIR}/cint/tool/makecint.cxx LIBRARIES ${CMAKE_THREAD_LIBS_INIT} NOINSTALL)

#---Installation--------------------------------------------------------------------------------------------
ROOT_INSTALL_HEADERS()
if(NOT CMAKE_SOURCE_DIR STREQUAL CMAKE_INSTALL_PREFIX)
  install(DIRECTORY lib     DESTINATION ${CMAKE_INSTALL_CINTINCDIR}/cint PATTERN ".dll" EXCLUDE PATTERN ".svn" EXCLUDE)
  install(DIRECTORY include DESTINATION ${CMAKE_INSTALL_CINTINCDIR}/cint PATTERN ".dll" EXCLUDE PATTERN ".svn" EXCLUDE)
  install(DIRECTORY stl     DESTINATION ${CMAKE_INSTALL_CINTINCDIR}/cint PATTERN ".dll" EXCLUDE PATTERN ".svn" EXCLUDE)
endif()




