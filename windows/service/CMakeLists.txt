
add_executable(winuwbservice "")

target_sources(winuwbservice
    PRIVATE
        Main.cxx
)

target_include_directories(winuwbservice
    PRIVATE
        ${UWB_DIR_LIB}
)

target_link_libraries(winuwbservice
    PRIVATE
        nearobject-service
        se
        seglobalplatform
        smartcard
        uwb
)

set_target_properties(winuwbservice PROPERTIES FOLDER windows/service)
