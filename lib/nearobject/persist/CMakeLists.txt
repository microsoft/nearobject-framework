
add_library(nearobject-persist STATIC "")

set(NO_PERSIST_DIR_PUBLIC_INCLUDE_PREFIX ${NO_DIR_PUBLIC_INCLUDE_PREFIX}/persist)

target_sources(nearobject-persist
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectProfilePersisterFilesystem.cxx
    PUBLIC
        ${NO_PERSIST_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectProfilePersister.hxx
        ${NO_PERSIST_DIR_PUBLIC_INCLUDE_PREFIX}/NearObjectProfilePersisterFilesystem.hxx
)

target_link_libraries(nearobject-persist
    PRIVATE
        jsonify
        nearobject-serialization
        nearobject-service
        nlohmann_json::nlohmann_json
)

set_target_properties(nearobject-persist PROPERTIES FOLDER lib/nearobject)
