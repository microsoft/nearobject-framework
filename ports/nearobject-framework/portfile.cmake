
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO microsoft/nearobject-framework
    REF v0.2.0
    SHA512 8e18b09c9f8b0594d299d70bfbbe29c44de5959dd4b10bbc07dccbcfbe6a1629d5db1d18200442735da4d4a399b4ffa4ee13e4bf96ecf5117b485a3f679f502e
    HEAD_REF develop
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
