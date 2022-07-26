
add_executable(winuwbservice "")

target_sources(winuwbservice
    PRIVATE
        Main.cxx
)

target_include_directories(winuwbservice
    PRIVATE
        ${NOF_DIR_LIB}
)

target_link_libraries(winuwbservice
    PRIVATE
        nearobject-service
)

set_target_properties(winuwbservice PROPERTIES FOLDER windows/service)
