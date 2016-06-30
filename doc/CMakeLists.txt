FIND_PACKAGE(Doxygen)
if (DOXYGEN_FOUND)
    #-- Configure the Template Doxyfile for our specific project
    configure_file(Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)

    #-- Add a custom target to run Doxygen
    add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        DEPENDS Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        SOURCES Doxyfile.in ${PROJECT_SOURCE_DIR}/README.md)
else()
    message(WARNING "Doxygen is needed to build the documentation. Please install it correctly")
endif()
