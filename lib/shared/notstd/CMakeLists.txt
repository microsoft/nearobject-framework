
add_library(notstd STATIC "")

set(NOTSTD_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX ${NOTSTD_DIR_PUBLIC_INCLUDE}/notstd)

target_sources(notstd
    PUBLIC
        ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/memory.hxx
        ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/tostring.hxx
        ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/task_queue.hxx
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/src/task_queue/task_queue.cxx
)

target_include_directories(notstd
    PUBLIC 
        ${NOTSTD_DIR_PUBLIC_INCLUDE}
)
