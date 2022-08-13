
add_executable(nocli "")

target_sources(nocli
    PRIVATE
        Main.cxx
)

target_link_libraries(nocli
    PRIVATE
        nearobject
        nearobject-service
)

set_target_properties(nocli PROPERTIES FOLDER tools/cli)
