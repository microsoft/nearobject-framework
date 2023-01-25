
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO microsoft/nearobject-framework
    REF v0.2.2
    SHA512 34013159a09cfd52e297529cff115b4392dec536c02b2260548b25bb1abf4702a0641e61b3c83401c4002df7bb456175b7e5277cd862d6e7972fc12312c03ae4
    HEAD_REF develop
)
vcpkg_check_features(
    OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    FEATURES
        msvc-link-static-runtime NOF_USE_MSVC_STATIC_RUNTIME
        windows-build NOF_WINDOWS_BUILD
)
vcpkg_cmake_configure(
    SOURCE_PATH ${SOURCE_PATH}
    OPTIONS
        -DNOF_DISABLE_TESTS=TRUE
        ${FEATURE_OPTIONS}
)
vcpkg_cmake_install()
vcpkg_copy_pdbs()
file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
