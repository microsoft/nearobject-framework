
add_executable(nocli "")

target_sources(nocli
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
)

target_link_libraries(nocli
    PRIVATE
        nearobject
        nearobject-service
)

set_target_properties(nocli PROPERTIES FOLDER tools/cli)

add_library(nocli-core STATIC "")

set(NO_CLI_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(NO_CLI_DIR_PUBLIC_INCLUDE_PREFIX ${NO_CLI_DIR_PUBLIC_INCLUDE}/nearobject/cli)

target_sources(nocli-core
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCli.cxx
    PUBLIC
        ${NO_CLI_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectCli.hxx
        ${NO_CLI_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectCliData.hxx
)

target_include_directories(nocli-core
    PUBLIC
        ${NO_CLI_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(nocli-core
    PRIVATE
        CLI11::CLI11
)

set_target_properties(nocli-core PROPERTIES FOLDER lib/nearobject)
