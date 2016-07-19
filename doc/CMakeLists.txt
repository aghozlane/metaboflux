# doc CmakeLists.txt
cmake_minimum_required(VERSION 2.6)
INCLUDE (FindDoxygen.cmake)
if (DOXYGEN_FOUND)
    configure_file (${MetaBoFlux_SOURCE_DIR}/doc/Doxyfile-html.cmake ${MetaBoFlux_BINARY_DIR}/doc/Doxyfile-html @ONLY)
    add_custom_target (html ${DOXYGEN_EXECUTABLE} ${MetaBoFlux_BINARY_DIR}/doc/Doxyfile-html)
endif (DOXYGEN_FOUND)