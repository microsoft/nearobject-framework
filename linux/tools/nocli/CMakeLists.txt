
add_executable(nearobject-cli-linux "")

target_sources(nearobject-cli-linux
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliDataLinux.hxx
)

target_link_libraries(nearobject-cli-linux
    PRIVATE
        CLI11::CLI11
        logging-utils
        nearobject-cli
        plog::plog
)

set_target_properties(nearobject-cli-linux PROPERTIES OUTPUT_NAME nocli)
set_target_properties(nearobject-cli-linux PROPERTIES FOLDER linux/tools)

install(
    TARGETS nearobject-cli-linux
    EXPORT nearobject-cli-linux
    RUNTIME
)
