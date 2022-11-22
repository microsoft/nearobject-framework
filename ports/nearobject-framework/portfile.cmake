
vcpkg_from_git(
    OUT_SOURCE_PATH SOURCE_PATH
    URL "https://github.com/aep-microsoft/nearobject-framework.git"
    REF 6235d1afc5e31b0a63c5591d5548f7cc17850971
)

vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        -DNOF_DISABLE_TESTS=TRUE
        -DNOF_USE_VCPKG=TRUE
        -DNOF_OFFICIAL_BUILD=TRUE
)

vcpkg_cmake_install()
vcpkg_copy_pdbs()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
