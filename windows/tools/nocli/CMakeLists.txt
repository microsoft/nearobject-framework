
add_executable(nearobject-cli-windows "")

target_sources(nearobject-cli-windows
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliDataWindows.hxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliHandlerWindows.hxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliHandlerWindows.cxx

)

target_link_libraries(nearobject-cli-windows
    PRIVATE
        CLI11::CLI11
        logging-utils
        magic_enum::magic_enum
        nearobject-cli
        notstd-windows
        plog::plog
        windows-devices-util
        windows-devices-uwb
)

set_target_properties(nearobject-cli-windows PROPERTIES OUTPUT_NAME nocli)
set_target_properties(nearobject-cli-windows PROPERTIES FOLDER windows/tools)

install(
    TARGETS nearobject-cli-windows
    EXPORT nearobject-cli-windows
    RUNTIME
)
