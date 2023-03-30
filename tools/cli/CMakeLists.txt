
add_library(nearobject-cli STATIC "")

set(NO_CLI_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(NO_CLI_DIR_PUBLIC_INCLUDE_PREFIX ${NO_CLI_DIR_PUBLIC_INCLUDE}/nearobject/cli)

target_sources(nearobject-cli
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCli.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliControlFlowContext.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliData.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliHandler.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliUwbSessionEventCallbacks.cxx
    PUBLIC
        ${NO_CLI_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectCli.hxx
        ${NO_CLI_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectCliControlFlowContext.hxx
        ${NO_CLI_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectCliHandler.hxx
        ${NO_CLI_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectCliData.hxx
        ${NO_CLI_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectCliUwbSessionEventCallbacks.hxx
)

target_include_directories(nearobject-cli
    PUBLIC
        ${NO_CLI_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(nearobject-cli
    PRIVATE
        CLI11::CLI11
        magic_enum::magic_enum
        plog::plog
    PUBLIC
        uwb-proto-fira
        uwb
)

set_target_properties(nearobject-cli PROPERTIES OUTPUT_NAME nearobject-cli)
set_target_properties(nearobject-cli PROPERTIES FOLDER lib/nearobject)
