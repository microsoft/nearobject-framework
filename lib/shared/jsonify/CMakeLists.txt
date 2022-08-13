
add_library(jsonify INTERFACE)

target_sources(jsonify
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include/jsonify.hxx
        ${CMAKE_CURRENT_LIST_DIR}/include/JsonSerializer.hxx
)

target_include_directories(jsonify
    INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}/include
)
