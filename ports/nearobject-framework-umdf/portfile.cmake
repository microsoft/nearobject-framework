
vcpkg_from_git(
    OUT_SOURCE_PATH SOURCE_PATH
    URL https://github.com/microsoft/nearobject-framework.git
    REF c7dddb18316c37fc89659e63bc18271ac2609250
)
vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        -DNOF_DISABLE_TESTS=TRUE
        -DNOF_USE_VCPKG=TRUE
        -DNOF_BUILD_FOR_UMDF=TRUE
)
vcpkg_cmake_install()
vcpkg_copy_pdbs()
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
