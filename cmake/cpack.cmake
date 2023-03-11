
# Metadata related varaibles.
set(CPACK_PACKAGE_NAME "nearobjectframework")
set(CPACK_PACKAGE_VENDOR "Microsoft")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Near Object Framework - A framework for interacting with short-range devices")

# Package configuration.
set(CPACK_NSIS OFF)
set(CPACK_BINARY_NSIS OFF)
set(CPACK_ZIP ON)
set(CPACK_BINARY_ZIP ON)
set(CPACK_BINARY_TXZ ON)
set(CPACK_SOURCE_ZIP ON)  

include(CPack)
