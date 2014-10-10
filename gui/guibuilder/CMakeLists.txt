############################################################################
# CMakeLists.txt file for building ROOT gui/guibuilder package
############################################################################

if(gui_guibuilder)

ROOT_USE_PACKAGE(gui/gui)
ROOT_USE_PACKAGE(tree/treeplayer)

ROOT_STANDARD_LIBRARY_PACKAGE(GuiBld DEPENDENCIES Gui Graf MathCore)

endif()
