
# Current release versioning information.
set(NOF_VCPKG_PORT_RELEASE_REF v0.2.4)
set(NOF_VCPKG_PORT_RELEASE_SHA512 f1e262c329ada13fe289ae35e4061a9730c98aeff8b10ca07962c7879ac3048a8d2c0a5901e2f1c477838d8eb27d5d2bccf82cc68851f39765d33249d60677cd)

set(NOF_VCPKG_PORT_FILE_OUT ${CMAKE_CURRENT_LIST_DIR}/ports/nearobject-framework/portfile.cmake)
set(NOF_VCPKG_PORT_FILE_IN ${NOF_VCPKG_PORT_FILE_OUT}.in)

# Obtain current git commit sha from this branch.
execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE NOF_VCPKG_PORT_GIT_REF
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

MESSAGE(STATUS "vcpkg development port using commit ${NOF_VCPKG_PORT_GIT_REF}")

# Generate portfile.cmake with current versioning information.
configure_file(${NOF_VCPKG_PORT_FILE_IN} ${NOF_VCPKG_PORT_FILE_OUT} @ONLY)
