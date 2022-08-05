
add_library(jsonify STATIC "")

include_directories("rapidjson")
# add_executable(jsontest test.cxx)

target_sources(jsonify
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/test.cxx
)

target_link_libraries(jsonify
    PUBLIC 
        GSL
)

set_target_properties(jsonify PROPERTIES FOLDER lib/shared/jsonify)
