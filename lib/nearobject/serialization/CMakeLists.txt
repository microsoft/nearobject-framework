
add_library(nearobject-serialization STATIC "")

set(NO_SERIALIZATION_DIR_PUBLIC_INCLUDE_PREFIX ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/serialization)

target_sources(nearobject-serialization
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectIdentityTokenUwbJsonSerializer.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectProfileJsonSerializer.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbMacAddressJsonSerializer.cxx
    PUBLIC
        ${NO_SERIALIZATION_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectIdentityTokenUwbJsonSerializer.hxx
        ${NO_SERIALIZATION_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectProfileJsonSerializer.hxx
        ${NO_SERIALIZATION_DIR_PUBLIC_INCLUDE_PREFIX}/UwbMacAddressJsonSerializer.hxx
)

target_link_libraries(nearobject-serialization
    PRIVATE
        jsonify
        nearobject
        nearobject-service
        nlohmann_json::nlohmann_json
        uwb
)

set_target_properties(nearobject-serialization PROPERTIES FOLDER lib/nearobject)
