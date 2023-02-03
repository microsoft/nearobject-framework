
add_executable(nocli-win "")

target_sources(nocli-win
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliDataWindows.hxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliHandlerWindows.hxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliHandlerWindows.cxx

)

target_link_libraries(nocli-win
    PRIVATE
        CLI11::CLI11
        LogUtils
        magic_enum::magic_enum
        nocli-core
        notstd-windows
        plog::plog
        windevutil
        windev-uwb
)

set_target_properties(nocli-win PROPERTIES OUTPUT_NAME nocli)
set_target_properties(nocli-win PROPERTIES FOLDER windows/tools)
