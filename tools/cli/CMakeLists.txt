
add_library(nocli-core STATIC "")

set(NO_CLI_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(NO_CLI_DIR_PUBLIC_INCLUDE_PREFIX ${NO_CLI_DIR_PUBLIC_INCLUDE}/nearobject/cli)

target_sources(nocli-core
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCli.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliData.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliHandler.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCliUwbSessionEventCallbacks.cxx
    PUBLIC
        ${NO_CLI_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectCli.hxx
        ${NO_CLI_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectCliHandler.hxx
        ${NO_CLI_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectCliData.hxx
        ${NO_CLI_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectCliUwbSessionEventCallbacks.hxx
)

target_include_directories(nocli-core
    PUBLIC
        ${NO_CLI_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(nocli-core
    PRIVATE
        CLI11::CLI11
        magic_enum::magic_enum
    PUBLIC
        uwb-proto-fira
        uwb
)

set_target_properties(nocli-core PROPERTIES FOLDER lib/nearobject)
