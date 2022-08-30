
add_library(notstd INTERFACE)

target_sources(notstd
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/include/notstd/memory.hxx
        ${CMAKE_CURRENT_LIST_DIR}/include/notstd/tostring.hxx
        ${CMAKE_CURRENT_LIST_DIR}/include/notstd/task_queue.hxx
)

target_include_directories(notstd
    INTERFACE 
        ${CMAKE_CURRENT_LIST_DIR}/include
)
