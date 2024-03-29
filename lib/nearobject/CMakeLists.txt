
add_library(nearobject STATIC "")

set(NO_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(NO_DIR_PUBLIC_INCLUDE_PREFIX ${NO_DIR_PUBLIC_INCLUDE}/nearobject)

target_sources(nearobject
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObject.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectIdentityToken.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectIdentityTokenUwb.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectManager.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectProfile.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectProfileSecurity.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectSession.cxx
    PUBLIC
        ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObject.hxx
        ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectIdentityToken.hxx
        ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectIdentityTokenUwb.hxx
        ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectCapabilities.hxx
        ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectManager.hxx
        ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectProfile.hxx
        ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectProfileSecurity.hxx
        ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectSession.hxx
        ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectSessionEventCallbacks.hxx
)

target_include_directories(nearobject
    PUBLIC
        ${NO_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(nearobject
    PRIVATE
        magic_enum::magic_enum
        nearobject-service
        notstd
    PUBLIC
        jsonify
        uwb
)

list(APPEND NO_PUBLIC_HEADERS
    ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObject.hxx
    ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectIdentityToken.hxx
    ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectIdentityTokenUwb.hxx
    ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectCapabilities.hxx
    ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectManager.hxx
    ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectProfile.hxx
    ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectProfileSecurity.hxx
    ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectSession.hxx
    ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectSessionEventCallbacks.hxx
)

set_target_properties(nearobject PROPERTIES FOLDER lib/nearobject)
set_target_properties(nearobject PROPERTIES PUBLIC_HEADER "${NO_PUBLIC_HEADERS}")

install(
    TARGETS nearobject
    EXPORT nearobject
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nearobject
)

add_subdirectory(discovery)
add_subdirectory(persist)
add_subdirectory(serialization)
add_subdirectory(service)
