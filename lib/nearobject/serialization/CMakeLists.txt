
add_library(nearobject-serialization STATIC "")

set(NO_SERIALIZATION_DIR_PUBLIC_INCLUDE_PREFIX ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/serialization)

target_sources(nearobject-serialization
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectIdentityTokenUwbJsonSerializer.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectProfileJsonSerializer.cxx
    PUBLIC
        ${NO_SERIALIZATION_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectIdentityTokenUwbJsonSerializer.hxx
        ${NO_SERIALIZATION_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectProfileJsonSerializer.hxx
)

target_link_libraries(nearobject-serialization
    PRIVATE
        jsonify
        nearobject
        nearobject-service
        nlohmann_json::nlohmann_json
        uwb
)

list(APPEND NO_SERIALIZATION_PUBLIC_HEADERS
    ${NO_SERIALIZATION_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectIdentityTokenUwbJsonSerializer.hxx
    ${NO_SERIALIZATION_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectProfileJsonSerializer.hxx
)

set_target_properties(nearobject-serialization PROPERTIES FOLDER lib/nearobject)
set_target_properties(nearobject-serialization PROPERTIES PUBLIC_HEADER "${NO_SERIALIZATION_PUBLIC_HEADERS}")

install(
    TARGETS nearobject-serialization
    EXPORT nearobject
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/nearobject/serialization
)
