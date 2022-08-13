
add_library(nearobject STATIC "")

target_sources(nearobject
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObject.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectManager.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectProfile.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectProfileSecurity.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectSession.cxx
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/NearObject.hxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectCapabilities.hxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectManager.hxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectProfile.hxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectProfileSecurity.hxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectSession.hxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectSessionEventCallbacks.hxx
)

target_include_directories(nearobject
    PRIVATE
        ${NOF_DIR_LIB}
        ${NOF_DIR_LIB}/shared
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(nearobject
    PRIVATE
        nearobject-service
)

set_target_properties(nearobject PROPERTIES FOLDER lib/nearobject)

add_subdirectory(discovery)
add_subdirectory(persist)
add_subdirectory(protocols)
add_subdirectory(service)
