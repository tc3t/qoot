############################################################################
# CMakeLists.txt file for building ROOT gui/ged package
############################################################################

if(gui_ged)

ROOT_USE_PACKAGE(gui/gui)

if(tree)
  ROOT_USE_PACKAGE(tree/treeplayer)
  ROOT_STANDARD_LIBRARY_PACKAGE(Ged DEPENDENCIES Hist Gpad Graf Gui Tree TreePlayer)
else()
  ROOT_STANDARD_LIBRARY_PACKAGE(Ged DEPENDENCIES Hist Gpad Graf Gui)
  add_definitions(-DGED_NO_TREE_DEPENDENCY)
endif()

endif()
