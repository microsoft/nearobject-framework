
add_executable(uwbsim-win "")

target_sources(uwbsim-win
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
)

target_link_libraries(uwbsim-win
    PRIVATE
        CLI11::CLI11
        logging-utils
        plog::plog
        windev-util
        windev-uwb-simulator
)

set_target_properties(uwbsim-win PROPERTIES OUTPUT_NAME uwbsim)
set_target_properties(uwbsim-win PROPERTIES FOLDER windows/tools)
