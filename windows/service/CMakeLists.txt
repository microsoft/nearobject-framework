
add_executable(winuwbservice "")

target_sources(winuwbservice
    PRIVATE
        Main.cxx
        UwbService.hxx
        UwbService.cxx
)

target_include_directories(winuwbservice
    PRIVATE
        ../lib/se
        ../lib/se/globalplatform
        ../lib/se/iso
)

target_link_libraries(winuwbservice
    PRIVATE
        se
        seiso
        seglobalplatform
)

set_target_properties(winuwbservice PROPERTIES FOLDER windows/service)
