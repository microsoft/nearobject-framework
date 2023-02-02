
vcpkg_from_github(
    OUT_SOURCE_PATH SOURCE_PATH
    REPO microsoft/nearobject-framework
    REF v0.2.4
    SHA512 f1e262c329ada13fe289ae35e4061a9730c98aeff8b10ca07962c7879ac3048a8d2c0a5901e2f1c477838d8eb27d5d2bccf82cc68851f39765d33249d60677cd
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
        -DVCPKG_MANIFEST_DIR=${SOURCE_PATH}/ports/nearobject-framework
        -DNOF_USE_VCPKG=TRUE
        -DNOF_DISABLE_TESTS=TRUE
        ${FEATURE_OPTIONS}
)

vcpkg_cmake_install()
vcpkg_copy_pdbs()

file(REMOVE_RECURSE "${CURRENT_PACKAGES_DIR}/debug/include")
file(INSTALL "${SOURCE_PATH}/LICENSE" DESTINATION "${CURRENT_PACKAGES_DIR}/share/${PORT}" RENAME copyright)
