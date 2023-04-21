
add_library(jsonify INTERFACE)

set(JSONIFY_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include/jsonify)
set(JSONIFY_PUBLIC_INCLUDE_PREFIX ${JSONIFY_PUBLIC_INCLUDE})

target_sources(jsonify
    INTERFACE
        ${JSONIFY_PUBLIC_INCLUDE_PREFIX}/jsonify.hxx
        ${JSONIFY_PUBLIC_INCLUDE_PREFIX}/nlohmann/std_optional_serializer.hxx
)

target_include_directories(jsonify
    INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(jsonify
    INTERFACE
        nlohmann_json::nlohmann_json
)

list(APPEND JSONIFY_PUBLIC_HEADERS
    ${JSONIFY_PUBLIC_INCLUDE_PREFIX}/jsonify.hxx
    ${JSONIFY_PUBLIC_INCLUDE_PREFIX}/nlohmann/std_optional_serializer.hxx
)

set_target_properties(jsonify PROPERTIES FOLDER shared/jsonify)
set_target_properties(jsonify PROPERTIES PUBLIC_HEADER "${JSONIFY_PUBLIC_HEADERS}")

install(
    TARGETS jsonify
    EXPORT jsonify
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/jsonify
)
