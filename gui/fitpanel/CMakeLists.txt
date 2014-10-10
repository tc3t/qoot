############################################################################
# CMakeLists.txt file for building ROOT gui/fitpanel package
############################################################################

if(fitpanel)

ROOT_USE_PACKAGE(gui/gui)

if(tree)
  ROOT_USE_PACKAGE(tree/treeplayer)
  ROOT_STANDARD_LIBRARY_PACKAGE(FitPanel DEPENDENCIES Gui Gpad Hist Graf Tree MathCore)
else()
  ROOT_STANDARD_LIBRARY_PACKAGE(FitPanel DEPENDENCIES Gui Gpad Hist Graf MathCore)
  add_definitions(-DFITPANEL_NO_TREE_DEPENDENCY)
endif()

endif()
