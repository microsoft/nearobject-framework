
vcpkg_from_git(
    OUT_SOURCE_PATH SOURCE_PATH
    URL https://github.com/microsoft/nearobject-framework.git
    REF ecc4db2c1b12f4cfc757edfb819e058a8532bbd1
)

vcpkg_check_features(
    OUT_FEATURE_OPTIONS FEATURE_OPTIONS
    FEATURES
        msvc-link-static-runtime NOF_USE_MSVC_STATIC_RUNTIME
        windows-build NOF_WINDOWS_BUILD
)

vcpkg_cmake_configure(
    SOURCE_PATH 
    OPTIONS
        -DVCPKG_MANIFEST_DIR=/ports/nearobject-framework
        -DNOF_USE_VCPKG=TRUE
        -DNOF_DISABLE_TESTS=TRUE
        ${FEATURE_OPTIONS}
)

vcpkg_cmake_install()
vcpkg_copy_pdbs()

file(REMOVE_RECURSE "/debug/include")
file(INSTALL "/LICENSE" DESTINATION "/share/" RENAME copyright)
