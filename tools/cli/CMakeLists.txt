
add_executable(nocli "")

target_sources(nocli
    PRIVATE
        Main.cxx
)

target_include_directories(nocli
    PRIVATE
        ${UWB_DIR_LIB}
)

target_link_libraries(nocli
    PRIVATE
        nearobject-core
        nearobject-service
        se
        seglobalplatform
        smartcard
        uwb
)

set_target_properties(nocli PROPERTIES FOLDER tools/cli)
