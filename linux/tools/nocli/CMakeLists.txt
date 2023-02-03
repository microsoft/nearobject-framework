
add_executable(nocli-linux "")

target_sources(nocli-linux
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliDataLinux.hxx
)

target_link_libraries(nocli-linux
    PRIVATE
        CLI11::CLI11
        LogUtils
        nocli-core
        plog::plog
)

set_target_properties(nocli-linux PROPERTIES OUTPUT_NAME nocli)
set_target_properties(nocli-linux PROPERTIES FOLDER linux/tools)
