
add_executable(nocli-win "")

target_sources(nocli-win
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
)

target_link_libraries(nocli-win
    PRIVATE
        CLI11::CLI11
        magic_enum::magic_enum
        notstd-windows
        windevutil
        windevuwb
)

set_target_properties(nocli-win PROPERTIES OUTPUT_NAME nocli)
set_target_properties(nocli-win PROPERTIES FOLDER windows/tools)
