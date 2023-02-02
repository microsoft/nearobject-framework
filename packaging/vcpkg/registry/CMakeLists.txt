
set(PORT_VERSION 1.0.0)
set(PORT_FILE_OUT ${CMAKE_CURRENT_LIST_DIR}/ports/nearobject-framework/${PORT_VERSION}/portfile.cmake)
set(PORT_FILE_IN ${PORT_FILE_OUT}.in)

set(NOF_VCPKG_PORT_GIT_REF ecc4db2c1b12f4cfc757edfb819e058a8532bbd1)

configure_file(${PORT_FILE_IN} ${PORT_FILE_OUT})
