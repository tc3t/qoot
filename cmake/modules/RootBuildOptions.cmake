set(root_build_options)

#---------------------------------------------------------------------------------------------------
#---ROOT_BUILD_OPTION( name defvalue [description] )
#---------------------------------------------------------------------------------------------------
function(ROOT_BUILD_OPTION name defvalue)
  if(ARGN)
    set(description ${ARGN})
  else()
    set(description " ")
  endif()    
  option(${name} "${description}" ${defvalue})
  set(root_build_options ${root_build_options} ${name} PARENT_SCOPE )
endfunction()

#---------------------------------------------------------------------------------------------------
#---ROOT_SHOW_OPTIONS([var] )
#---------------------------------------------------------------------------------------------------
function(ROOT_SHOW_OPTIONS)
  set(enabled)
  foreach(opt ${root_build_options})
    if(${opt})
      set(enabled "${enabled} ${opt}")
    endif()
  endforeach()
  if(NOT ARGN)
    message(STATUS "Enabled support for: ${enabled}")
  else()
    set(${ARGN} "${enabled}" PARENT_SCOPE)
  endif()
endfunction()

#---------------------------------------------------------------------------------------------------
#---ROOT_WRITE_OPTIONS(file )
#---------------------------------------------------------------------------------------------------
function(ROOT_WRITE_OPTIONS file)
  file(WRITE ${file} "#---Options enabled for the build of ROOT-----------------------------------------------\n")
  foreach(opt ${root_build_options})
    if(${opt})
      file(APPEND ${file} "set(${opt} ON)\n")
    else()
      file(APPEND ${file} "set(${opt} OFF)\n")
    endif()
  endforeach()
endfunction()

#---Define default values depending on platform before the options are defined----------------------
if(WIN32)
  set(x11_defvalue OFF)
  set(memstat_defvalue OFF)
  set(explicitlink_defvalue ON)
  set(cocoa_defvalue OFF)
  set(davix_defvalue OFF)
elseif(APPLE)
  execute_process(COMMAND sw_vers "-productVersion" COMMAND cut -d . -f 1-2
                  OUTPUT_VARIABLE MACOSX_VERSION OUTPUT_STRIP_TRAILING_WHITESPACE)
  set(memstat_defvalue ON)
  set(explicitlink_defvalue ON)
  if(MACOSX_VERSION VERSION_GREATER 10.7 AND ${CMAKE_CXX_COMPILER_ID} STREQUAL Clang)
    set(x11_defvalue OFF)
    set(cocoa_defvalue ON)
  else()
    set(x11_defvalue ON)
    set(cocoa_defvalue OFF)
  endif()
  set(davix_defvalue OFF)
else()
  set(x11_defvalue ON)
  set(memstat_defvalue ON)
  set(explicitlink_defvalue ON)
  set(cocoa_defvalue OFF)
  set(davix_defvalue OFF)
endif()

if(all)
 set(gdml_defvalue ON)
 set(http_defvalue ON)
 #set(qt4_defvalue ON)
 set(qt5_defvalue ON)
 set(qtged_defvalue ON)
 set(qtgsi_defvalue ON)
 set(qtgui_defvalue ON)
 set(roofit_defvalue ON)
 set(minuit2_defvalue ON)
 set(table_defvalue ON)
 set(unuran_defvalue ON)
 if(WIN32)
   set(vc_defvalue OFF)
 else()
   set(vc_defvalue ON)
 endif()
 set(vdt_defvalue ON)
else()
 set(gdml_defvalue OFF)
 set(http_defvalue OFF)
 set(qt4_defvalue OFF)
 set(qt5_defvalue OFF)
 set(qtged_defvalue OFF)
 set(qtgsi_defvalue ON)
 set(qtgui_defvalue OFF)
 set(roofit_defvalue OFF)
 set(minuit2_defvalue OFF)
 set(table_defvalue OFF)
 set(unuran_defvalue OFF)
 set(vc_defvalue OFF)
 set(vdt_defvalue OFF)
endif()

set(alien_defvalue OFF)
set(bonjour_defvalue OFF)
set(cxx11_defvalue OFF)
set(castor_defvalue OFF)
set(chirp_defvalue OFF)
set(cint_exe_defvalue OFF)
set(cintex_defvalue OFF)
set(dcache_defvalue OFF)
set(eve_defvalue OFF)
set(fftw3_defvalue OFF)
set(fitpanel_defvalue OFF)
set(fitsio_defvalue OFF)
set(foam_defvalue OFF)
set(fortran_defvalue OFF)
set(fumili_defvalue ON)
set(genvector_defvalue ON)
set(gfal_defvalue OFF)
set(glite_defvalue OFF)
set(geom_defvalue OFF)
set(geombuilder_defvalue OFF)
set(geompainter_defvalue OFF)
set(graf3d_defvalue ON)
set(gui_defvalue OFF)
set(gui_ged_defvalue OFF)
set(gui_guibuilder_defvalue OFF)
set(gui_guihtml_defvalue OFF)
set(gui_recorder_defvalue OFF)
set(gui_sessionviewer_defvalue OFF)
set(gviz_defvalue OFF)
set(hadd_defvalue OFF)
set(hdfs_defvalue OFF)
set(hsimple_defvalue OFF)
set(html_defvalue OFF)
set(krb5_defvalue OFF)
set(ldap_defvalue OFF)
set(mathmore_defvalue OFF)
set(mathtext_defvalue ON)
set(matrix_defvalue ON)
set(mceg_defvalue OFF)
set(minuit_defvalue OFF)
set(minuit2_defvalue ON)
set(mlp_defvalue OFF)
set(monalisa_defvalue OFF)
set(mysql_defvalue OFF)
set(net_defvalue OFF)
set(new_defvalue OFF)
set(odbc_defvalue OFF)
set(oracle_defvalue OFF)
set(pgsql_defvalue OFF)
set(physics_defvalue ON)
set(proof_defvalue OFF)
set(proofserv_defvalue OFF)
set(pythia6_defvalue OFF)
set(pythia8_defvalue OFF)
set(python_defvalue OFF)
set(qoot_32bit_color_t_defvalue OFF)
set(qoot_examples_defvalue ON)
set(qt4_defvalue OFF)
set(qt5_defvalue ON)
set(qtged_defvalue ON)
set(qtgui_defvalue ON)
set(reflex_defvalue OFF)
set(rfio_defvalue OFF)
set(rgl_defvalue OFF)
set(root_exe_defvalue ON)
set(sapdb_defvalue OFF)
set(sqlite_defvalue OFF)
set(splot_defvalue OFF)
set(quadp_defvalue OFF)
set(shadowpw_defvalue OFF)
set(smatrix_defvalue ON)
set(soversion_defvalue OFF)
set(spectrum_defvalue ON)
set(spectrumpainter_defvalue ON)
set(sqlio_defvalue OFF)
set(srp_defvalue OFF)
set(ssl_defvalue OFF)
set(tmva_defvalue OFF)
set(tree_defvalue OFF)
set(treeplayer_defvalue OFF)
set(treeviewer_defvalue OFF)
set(vmc_defvalue OFF)
set(win32gdk_defvalue OFF)
set(winrtdebug_defvalue ON)
set(xft_defvalue OFF)
set(xml_defvalue OFF)
set(xmlio_defvalue OFF)
set(xrootd_defvalue OFF)


ROOT_BUILD_OPTION(afdsmgrd OFF "Dataset manager for PROOF-based analysis facilities")
ROOT_BUILD_OPTION(afs OFF "AFS support, requires AFS libs and objects")
ROOT_BUILD_OPTION(alien ${alien_defvalue} "AliEn support, requires libgapiUI from ALICE")
ROOT_BUILD_OPTION(asimage ON "Image processing support, requires libAfterImage")
ROOT_BUILD_OPTION(astiff ON "Include tiff support in image processing")
ROOT_BUILD_OPTION(bonjour ${bonjour_defvalue} "Bonjour support, requires libdns_sd and/or Avahi")
ROOT_BUILD_OPTION(builtin_afterimage ON "Built included libAfterImage, or use system libAfterImage")
ROOT_BUILD_OPTION(builtin_ftgl ON "Built included libFTGL, or use system libftgl")
ROOT_BUILD_OPTION(builtin_freetype OFF "Built included libfreetype, or use system libfreetype")
ROOT_BUILD_OPTION(builtin_glew ON "Built included libGLEW, or use system libGLEW")
ROOT_BUILD_OPTION(builtin_openssl OFF "Build OpenSSL internally, or use system OpenSSL")
ROOT_BUILD_OPTION(builtin_pcre OFF "Built included libpcre, or use system libpcre")
ROOT_BUILD_OPTION(builtin_zlib OFF "Built included libz, or use system libz")
ROOT_BUILD_OPTION(builtin_lzma OFF "Built included liblzma, or use system liblzma")
ROOT_BUILD_OPTION(builtin_davix OFF "Built the Davix library internally (downloading tarfile from the Web)")
ROOT_BUILD_OPTION(builtin_gsl OFF "Built the GSL library internally (downloading tarfile from the Web)")
ROOT_BUILD_OPTION(builtin_cfitsio OFF "Built the FITSIO library internally (downloading tarfile from the Web)")
ROOT_BUILD_OPTION(builtin_xrootd OFF "Built the XROOTD internally (downloading tarfile from the Web)")
ROOT_BUILD_OPTION(cint_exe ${cint_exe_defvalue} "cint.exe")
ROOT_BUILD_OPTION(cxx11 ${cxx11_defvalue} "Build using C++11 compatible mode, requires gcc > 4.7.x or clang")
ROOT_BUILD_OPTION(cxx14 OFF "Build using C++14 compatible mode, requires gcc > 4.9.x or clang")
ROOT_BUILD_OPTION(libcxx OFF "Build using libc++, requires cxx11 option (MacOS X only, for the time being)")
ROOT_BUILD_OPTION(castor ${castor_defvalue} "CASTOR support, requires libshift from CASTOR >= 1.5.2")
ROOT_BUILD_OPTION(chirp ${chirp_defvalue} "Chirp support (Condor remote I/O), requires libchirp_client")
ROOT_BUILD_OPTION(cintex ${cintex_defvalue} "Build the libCintex Reflex interface library")
ROOT_BUILD_OPTION(cling OFF "Enable new CLING C++ interpreter")
ROOT_BUILD_OPTION(cocoa ${cocoa_defvalue} "Use native Cocoa/Quartz graphics backend (MacOS X only)")
ROOT_BUILD_OPTION(davix ${davix_defvalue} "DavIx library for HTTP/WEBDAV access")
ROOT_BUILD_OPTION(dcache ${dcache_defvalue} "dCache support, requires libdcap from DESY")
ROOT_BUILD_OPTION(eve ${eve_defvalue} "eve (TODO: description)")
ROOT_BUILD_OPTION(exceptions ON "Turn on compiler exception handling capability")
ROOT_BUILD_OPTION(explicitlink ${explicitlink_defvalue} "Explicitly link with all dependent libraries")
ROOT_BUILD_OPTION(fftw3 ${fftw3_defvalue} "Fast Fourier Transform support, requires libfftw3")
ROOT_BUILD_OPTION(fitpanel ${fitpanel_defvalue} "gui/FitPanel")
ROOT_BUILD_OPTION(fitsio ${fitsio_defvalue} "Read images and data from FITS files, requires cfitsio")
ROOT_BUILD_OPTION(foam ${foam_defvalue} "math/foam")
ROOT_BUILD_OPTION(fortran ${fortran_defvalue} "Enable the Fortran components of ROOT")
ROOT_BUILD_OPTION(fumili ${fumili_defvalue} "Fumili package")
ROOT_BUILD_OPTION(gviz ${gviz_defvalue} "Graphs visualization support, requires graphviz")
ROOT_BUILD_OPTION(gdml ${gdml_defvalue} "GDML writer and reader")
ROOT_BUILD_OPTION(genvector ${genvector_defvalue} "Build the new libGenVector library")
ROOT_BUILD_OPTION(gfal ${gfal_defvalue} "GFAL support, requires libgfal")
ROOT_BUILD_OPTION(glite ${glite_defvalue} "gLite support, requires libglite-api-wrapper v.3 from GSI (https://subversion.gsi.de/trac/dgrid/wiki)")
ROOT_BUILD_OPTION(geom ${geom_defvalue} "geom")
ROOT_BUILD_OPTION(geombuilder ${geombuilder_defvalue} "geombuilder")
ROOT_BUILD_OPTION(geompainter ${geompainter_defvalue} "geompainter")
ROOT_BUILD_OPTION(globus OFF "Globus authentication support, requires Globus toolkit")
ROOT_BUILD_OPTION(graf3d ${graf3d_defvalue} "graf3d")
ROOT_BUILD_OPTION(gsl_shared OFF "Enable linking against shared libraries for GSL (default no)")
ROOT_BUILD_OPTION(gui ${gui_defvalue} "ROOT gui")
ROOT_BUILD_OPTION(gui_ged ${gui_ged_defvalue} "gui/GED")
ROOT_BUILD_OPTION(gui_guibuilder ${gui_guibuilder_defvalue} "gui/guibuilder")
ROOT_BUILD_OPTION(gui_guihtml ${gui_guihtml_defvalue} "gui/guihtml")
ROOT_BUILD_OPTION(gui_recorder ${gui_recorder_defvalue} "gui/recorder")
ROOT_BUILD_OPTION(gui_sessionviewer ${gui_sessionviewer_defvalue} "gui/sessionviewer")
ROOT_BUILD_OPTION(gviz3d ${gviz3d_defvalue} "Gviz3d")
ROOT_BUILD_OPTION(hadd ${hadd_defvalue} "hadd")
ROOT_BUILD_OPTION(hdfs ${hdfs_defvalue} "HDFS support; requires libhdfs from HDFS >= 0.19.1")
ROOT_BUILD_OPTION(hsimple ${hsimple_defvalue} "Histogram example project")
ROOT_BUILD_OPTION(html ${html_defvalue} "THtml")
ROOT_BUILD_OPTION(http ${http_defvalue} "HTTP Server support")
ROOT_BUILD_OPTION(krb5 ${krb5_defvalue} "Kerberos5 support, requires Kerberos libs")
ROOT_BUILD_OPTION(ldap ${ldap_defvalue} "LDAP support, requires (Open)LDAP libs")
ROOT_BUILD_OPTION(mathmore ${mathmore_defvalue} "Build the new libMathMore extended math library, requires GSL (vers. >= 1.8)")
ROOT_BUILD_OPTION(mathtext ${mathtext_defvalue} "mathtext")
ROOT_BUILD_OPTION(matrix ${matrix_defvalue} "matrix")
ROOT_BUILD_OPTION(mceg ${mceg_defvalue} "EG (TODO: description)")
ROOT_BUILD_OPTION(memstat ${memstat_defvalue} "A memory statistics utility, helps to detect memory leaks")
ROOT_BUILD_OPTION(minuit ${minuit_defvalue} "minuit")
ROOT_BUILD_OPTION(minuit2 ${minuit2_defvalue} "Build the new libMinuit2 minimizer library")
ROOT_BUILD_OPTION(mlp ${mlp_defvalue} "MLP")
ROOT_BUILD_OPTION(monalisa ${monalisa_defvalue} "Monalisa monitoring support, requires libapmoncpp")
ROOT_BUILD_OPTION(mysql ${mysql_defvalue} "MySQL support, requires libmysqlclient")
ROOT_BUILD_OPTION(net ${net_defvalue} "Net")
ROOT_BUILD_OPTION(new ${new_defvalue} "new")
ROOT_BUILD_OPTION(odbc ${odbc_defvalue} "ODBC support, requires libiodbc or libodbc")
ROOT_BUILD_OPTION(opengl ON "OpenGL support, requires libGL and libGLU")
ROOT_BUILD_OPTION(oracle ${oracle_defvalue} "Oracle support, requires libocci")
ROOT_BUILD_OPTION(pch ON)
ROOT_BUILD_OPTION(pgsql ${pgsql_defvalue} "PostgreSQL support, requires libpq")
ROOT_BUILD_OPTION(physics ${physics_defvalue} "physics")
ROOT_BUILD_OPTION(proof ${proof_defvalue} "proof")
ROOT_BUILD_OPTION(proofserv ${proofserv_defvalue} "proofserver")
ROOT_BUILD_OPTION(pythia6 ${pythia6_defvalue} "Pythia6 EG support, requires libPythia6")
ROOT_BUILD_OPTION(pythia6_nolink OFF "Delayed linking of Pythia6 library")
ROOT_BUILD_OPTION(pythia8 ${pythia8_defvalue} "Pythia8 EG support, requires libPythia8")
ROOT_BUILD_OPTION(python ${python_defvalue} "Python ROOT bindings, requires python >= 2.2")
ROOT_BUILD_OPTION(qoot_32bit_color_t ${qoot_32bit_color_t_defvalue} "Use 32-bit color_t. NOTE: Experimental. NOTE: causes root-files to be incompatible with ROOT.")
ROOT_BUILD_OPTION(qoot_examples ${qoot_examples_defvalue} "Example programs.")
ROOT_BUILD_OPTION(qt4 ${qt4_defvalue} "Qt4 graphics backend, requires libqt >= 4.8")
ROOT_BUILD_OPTION(qt5 ${qt5_defvalue} "Qt5 graphics backend, requires libqt >= 5.0")
ROOT_BUILD_OPTION(qtged ${qtged_defvalue} "Qt ged")
ROOT_BUILD_OPTION(qtgsi ${qtgsi_defvalue} "GSI's Qt integration, requires libqt >= 4.8")
ROOT_BUILD_OPTION(qtgui ${qtgui_defvalue} "Qt GUI")
ROOT_BUILD_OPTION(quadp ${quadp_defvalue} "math/quadp")
ROOT_BUILD_OPTION(reflex ${reflex_defvalue} "Build the libReflex dictionary library")
ROOT_BUILD_OPTION(rgl ${rgl_defvalue} "RGL")
ROOT_BUILD_OPTION(roofit ${roofit_defvalue} "Build the libRooFit advanced fitting package")
ROOT_BUILD_OPTION(ruby OFF "Ruby ROOT bindings, requires ruby >= 1.8")
ROOT_BUILD_OPTION(rfio ${rfio_defvalue} "RFIO support, requires libshift from CASTOR >= 1.5.2")
ROOT_BUILD_OPTION(root_exe ${root_exe_defvalue} "root.exe")
ROOT_BUILD_OPTION(rpath OFF "Set run-time library load path on executables and shared libraries (at installation area)")
ROOT_BUILD_OPTION(sapdb ${sapdb_defvalue} "MaxDB/SapDB support, requires libsqlod and libsqlrte")
ROOT_BUILD_OPTION(shadowpw ${shadowpw_defvalue} "Shadow password support")
ROOT_BUILD_OPTION(shared ON "Use shared 3rd party libraries if possible")
ROOT_BUILD_OPTION(smatrix ${smatrix_defvalue} "smatrix")
ROOT_BUILD_OPTION(soversion ${soversion_defvalue} "Set version number in sonames (recommended)")
ROOT_BUILD_OPTION(sqlio ${sqlio_defvalue} "io/sql")
ROOT_BUILD_OPTION(sqlite ${sqlite_defvalue} "SQLite support, requires libsqlite3")
ROOT_BUILD_OPTION(spectrum ${spectrum_defvalue} "spectrum")
ROOT_BUILD_OPTION(spectrumpainter ${spectrumpainter_defvalue} "spectrumpainter")
ROOT_BUILD_OPTION(splot ${splot_defvalue} "splot")
ROOT_BUILD_OPTION(srp ${srp_defvalue} "SRP support, requires SRP source tree")
ROOT_BUILD_OPTION(ssl ${ssl_defvalue} "SSL encryption support, requires openssl")
ROOT_BUILD_OPTION(gnuinstall OFF "Perform installation following the GNU guidelines")
ROOT_BUILD_OPTION(table ${table_defvalue} "Build libTable contrib library")
ROOT_BUILD_OPTION(thread ON "Using thread library (cannot be disabled)")
ROOT_BUILD_OPTION(tmva ${tmva_defvalue} "Build TMVA multi variate analysis library")
ROOT_BUILD_OPTION(tree ${tree_defvalue} "tree")
ROOT_BUILD_OPTION(treeplayer ${treeplayer_defvalue} "Tree player")
ROOT_BUILD_OPTION(treeviewer ${treeviewer_defvalue} "Tree viewer")
ROOT_BUILD_OPTION(unuran ${unuran_defvalue} "UNURAN - package for generating non-uniform random numbers")
ROOT_BUILD_OPTION(vc ${vc_defvalue} "Vc adds a few new types for portable and intuitive SIMD programming")
ROOT_BUILD_OPTION(vdt ${vdt_defvalue} "VDT adds a set of fast and vectorisable mathematical functions")
ROOT_BUILD_OPTION(win32gdk ${win32gdk_defvalue} "(Windows only): build win32gdk")
ROOT_BUILD_OPTION(winrtdebug ${winrtdebug_defvalue} "Link against the Windows debug runtime library")
ROOT_BUILD_OPTION(vmc ${vmc_defvalue} "vmc")
ROOT_BUILD_OPTION(xft ${xft_defvalue} "Xft support (X11 antialiased fonts)")
ROOT_BUILD_OPTION(xml ${xml_defvalue} "XML parser interface")
ROOT_BUILD_OPTION(xmlio ${xmlio_defvalue} "io/xml")
ROOT_BUILD_OPTION(x11 ${x11_defvalue} "X11 support")
ROOT_BUILD_OPTION(xrootd ${xrootd_defvalue} "Build xrootd file server and its client (if supported)")
  
option(fail-on-missing "Fail the configure step if a required external package is missing" OFF)
option(minimal "Do not automatically search for support libraries" OFF)
option(gminimal "Do not automatically search for support libraries, but include X11" OFF)
option(all "Enable all optional components" OFF)
option(testing "Enable testing with CTest" OFF)


if(DEFINED c++11)   # For backward compatibility
  set(cxx11 ${c++11} CACHE BOOL "" FORCE)
endif()

#---Apply minimal or gminimal------------------------------------------------------------------
foreach(opt ${root_build_options})
  if(NOT opt MATCHES "thread|cxx11|cling|builtin_llvm|builtin_ftgl|explicitlink")
    if(minimal)
      set(${opt} OFF CACHE BOOL "" FORCE)
    elseif(gminimal AND NOT opt MATCHES "x11|cocoa")
      set(${opt} OFF CACHE BOOL "" FORCE)
    endif()
  endif()
endforeach()

#---Avoid creating dependencies to 'non-standard' header files -------------------------------
include_regular_expression("^[^.]+$|[.]h$|[.]icc$|[.]hxx$|[.]hpp$")

#---Add Installation Variables------------------------------------------------------------------
include(RootInstallDirs)

#---Add defines for CINT limits-----------------------------------------------------------------
if(DEFINED CINTMAXSTRUCT)
  add_definitions(-DG__MAXSTRUCT=${CINTMAXSTRUCT})
endif()
if(DEFINED CINTMAXTYPEDEF)
  add_definitions(-DG__MAXTYPEDEF=${CINTMAXTYPEDEF})
endif()
if(DEFINED CINTLONGLINE)
  add_definitions(-DG__LONGLINE=${CINTLONGLINE})
endif()

#---RPATH options-------------------------------------------------------------------------------
#  When building, don't use the install RPATH already (but later on when installing)
set(CMAKE_SKIP_BUILD_RPATH FALSE)         # don't skip the full RPATH for the build tree
set(CMAKE_BUILD_WITH_INSTALL_RPATH FALSE) # use always the build RPATH for the build tree
set(CMAKE_MACOSX_RPATH TRUE)              # use RPATH for MacOSX
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE) # point to directories outside the build tree to the install RPATH

# Check whether to add RPATH to the installation (the build tree always has the RPATH enabled)
if(rpath OR gnuinstall)
  set(CMAKE_INSTALL_RPATH ${CMAKE_INSTALL_FULL_LIBDIR}) # install LIBDIR
  set(CMAKE_SKIP_INSTALL_RPATH FALSE)          # don't skip the full RPATH for the install tree
elseif(APPLE)
  set(CMAKE_INSTALL_NAME_DIR "@rpath")
  set(CMAKE_INSTALL_RPATH "@loader_path/../lib")    # self relative LIBDIR
  set(CMAKE_SKIP_INSTALL_RPATH FALSE)          # don't skip the full RPATH for the install tree
else()
  set(CMAKE_SKIP_INSTALL_RPATH TRUE)           # skip the full RPATH for the install tree
endif()


