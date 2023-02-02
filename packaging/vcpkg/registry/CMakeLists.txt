
set(PORT_VERSION 1.0.0)
set(PORT_FILE_OUT ${CMAKE_CURRENT_LIST_DIR}/ports/nearobject-framework/${PORT_VERSION}/portfile.cmake)
set(PORT_FILE_IN ${PORT_FILE_OUT}.in)

execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE NOF_VCPKG_PORT_GIT_REF
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

MESSAGE(STATUS "vcpkg development port using commit ${NOF_VCPKG_PORT_GIT_REF}")

configure_file(${PORT_FILE_IN} ${PORT_FILE_OUT})
