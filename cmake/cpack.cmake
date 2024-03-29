
# Metadata related varaibles.
set(CPACK_PACKAGE_NAME ${CMAKE_PROJECT_NAME})
set(CPACK_PACKAGE_VENDOR "Microsoft")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Near Object Framework - A framework for interacting with short-range devices")

# Package configuration.
set(CPACK_NSIS OFF)
set(CPACK_BINARY_NSIS OFF)
set(CPACK_ZIP ON)
set(CPACK_BINARY_ZIP ON)
set(CPACK_BINARY_TXZ ON)
set(CPACK_SOURCE_ZIP ON)

if (NOT CPack_CMake_INCLUDED)
  include(CPack)
endif()
