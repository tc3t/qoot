############################################################################
# CMakeLists.txt file for building ROOT gui/guihtml package
############################################################################

if(gui_guihtml)

ROOT_USE_PACKAGE(gui/gui)
ROOT_USE_PACKAGE(net/net)

ROOT_STANDARD_LIBRARY_PACKAGE(GuiHtml DEPENDENCIES Gui Graf Net)

endif()
