
add_executable(devicemon "")

target_sources(devicemon
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
)

target_link_libraries(devicemon
    PRIVATE
        CLI11::CLI11
        magic_enum::magic_enum
        notstd-windows
        windows-devices-util
        plog::plog
        logging-utils
)

set_target_properties(devicemon PROPERTIES FOLDER windows/tools)
