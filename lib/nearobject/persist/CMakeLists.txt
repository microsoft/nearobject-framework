
add_library(nearobject-persist STATIC "")

set(NO_PERSIST_DIR_PUBLIC_INCLUDE_PREFIX ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/persist)

target_sources(nearobject-persist
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectProfileJsonSerializer.cxx
    PUBLIC
        ${NO_PERSIST_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectProfileJsonSerializer.hxx
)

target_link_libraries(nearobject-persist
    PRIVATE
        nearobject-service
        jsonify
        nlohmann_json::nlohmann_json
)

set_target_properties(nearobject-persist PROPERTIES FOLDER lib/nearobject/service)

