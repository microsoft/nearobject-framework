
# Obtain current git branch.
execute_process(COMMAND ${GIT_EXECUTABLE} branch --show
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE NOF_GIT_BRANCH_CURRENT
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Obtain git HEAD commit sha from this branch.
execute_process(COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE NOF_GIT_REF_HEAD
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# Obtain git origin url.
execute_process(COMMAND ${GIT_EXECUTABLE} remote get-url origin
    WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    OUTPUT_VARIABLE NOF_GIT_URL_ORIGIN
    OUTPUT_STRIP_TRAILING_WHITESPACE
)

# TODO: replace 'DRIVER" with 'DEVELOPMENT'

# Current driver versioning information.
set(NOF_VCPKG_DRIVER_URL ${CMAKE_SOURCE_DIR})
set(NOF_VCPKG_DRIVER_GIT_REF ${NOF_GIT_REF_HEAD})
set(NOF_VCPKG_DRIVER_GIT_REF_FETCH ${NOF_GIT_BRANCH_CURRENT})

# Current release versioning information.
set(NOF_VCPKG_RELEASE_TAG v0.2.4)
set(NOF_VCPKG_RELEASE_URL ${NOF_GIT_URL_ORIGIN})
set(NOF_VCPKG_RELEASE_GIT_REF ${NOF_VCPKG_RELEASE_TAG})
set(NOF_VCPKG_RELEASE_GIT_REF_FETCH ${NOF_GIT_BRANCH_CURRENT})

# Determine the vcpkg target the codebase is being built for.
if (NOF_VCPKG_FEATURE_ENABLE_DEVELOPMENT)
    set(NOF_VCPKG_TARGET DRIVER)
else()
    set(NOF_VCPKG_TARGET RELEASE)
endif()

# Select configuration based on build target.
set(NOF_VCPKG_GIT_URL ${NOF_VCPKG_${NOF_VCPKG_TARGET}_URL})
set(NOF_VCPKG_GIT_REF ${NOF_VCPKG_${NOF_VCPKG_TARGET}_GIT_REF})
set(NOF_VCPKG_GIT_REF_FETCH ${NOF_VCPKG_${NOF_VCPKG_TARGET}_GIT_REF_FETCH})

MESSAGE(STATUS "vcpkg target ${NOF_VCPKG_TARGET} with ${NOF_VCPKG_GIT_URL}|${NOF_VCPKG_GIT_REF_FETCH}@${NOF_VCPKG_GIT_REF}")

set(NOF_VCPKG_PORT_FILE_OUT ${CMAKE_CURRENT_LIST_DIR}/ports/nearobject-framework/portfile.cmake)
set(NOF_VCPKG_PORT_FILE_IN ${NOF_VCPKG_PORT_FILE_OUT}.in)

# Generate portfile.cmake with selected versioning and configuration information.
configure_file(${NOF_VCPKG_PORT_FILE_IN} ${NOF_VCPKG_PORT_FILE_OUT} @ONLY)
