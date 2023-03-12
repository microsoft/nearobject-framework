
# Metadata related varaibles.
set(CPACK_PACKAGE_VENDOR "Microsoft")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Near Object Framework - A framework for interacting with short-range devices")

# Append the build type to the package file name. Solution taken from Craig
# Scott in https://gitlab.kitware.com/cmake/cmake/-/issues/10940.
if(CMAKE_GENERATOR STREQUAL Xcode OR
   CMAKE_GENERATOR MATCHES "^Visual Studio.*")
    # Multi-config generator, so we don't know the build config right now.
    # Delay evaluation of the build config until CPack runs.
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-\${CPACK_BUILD_CONFIG}")
else()
    # We know the build config now, so use it directly
    set(CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}-${CMAKE_BUILD_TYPE}")
endif()

# Package configuration.
set(CPACK_NSIS OFF)
set(CPACK_BINARY_NSIS OFF)
set(CPACK_ZIP ON)
set(CPACK_BINARY_ZIP ON)
set(CPACK_BINARY_TXZ ON)
set(CPACK_SOURCE_ZIP ON)

include(CPack)
