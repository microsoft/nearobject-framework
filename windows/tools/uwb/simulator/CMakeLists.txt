
add_executable(uwbsim-windows "")

target_sources(uwbsim-windows
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
)

target_link_libraries(uwbsim-windows
    PRIVATE
        CLI11::CLI11
        logging-utils
        plog::plog
        windows-devices-util
        windows-devices-uwb-simulator
)

set_target_properties(uwbsim-windows PROPERTIES OUTPUT_NAME uwbsim)
set_target_properties(uwbsim-windows PROPERTIES FOLDER windows/tools)
