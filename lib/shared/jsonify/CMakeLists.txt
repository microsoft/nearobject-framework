
add_library(jsonify INTERFACE)

target_sources(jsonify
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include/jsonify.hxx
        ${CMAKE_CURRENT_LIST_DIR}/include/nlohmann/std_optional_serializer.hxx
)

target_include_directories(jsonify
    INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries(jsonify
    INTERFACE
        nlohmann_json::nlohmann_json
)
