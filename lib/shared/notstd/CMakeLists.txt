
add_library(notstd STATIC "")

set(NOTSTD_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX ${NOTSTD_DIR_PUBLIC_INCLUDE}/notstd)

target_sources(notstd
    PUBLIC
        ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/flextype_wrapper.hxx
        ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/hash.hxx
        ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/memory.hxx
        ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/range.hxx
        ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/tostring.hxx
        ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/task_queue.hxx
        ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/type_traits.hxx
        ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/unique_ptr_out.hxx
        ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/utility.hxx
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/task_queue.cxx
)

target_include_directories(notstd
    PUBLIC 
        ${NOTSTD_DIR_PUBLIC_INCLUDE}
)

list(APPEND NOTSTD_PUBLIC_HEADERS
    ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/flextype_wrapper.hxx
    ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/hash.hxx
    ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/memory.hxx
    ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/range.hxx
    ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/tostring.hxx
    ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/task_queue.hxx
    ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/type_traits.hxx
    ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/unique_ptr_out.hxx
    ${NOTSTD_DIR_PUBLIC_INCLUDE_PREFIX}/utility.hxx
)

target_link_libraries(notstd
    PUBLIC
        Threads::Threads
)

set_target_properties(notstd PROPERTIES FOLDER shared/notstd)
set_target_properties(notstd PROPERTIES PUBLIC_HEADER "${NOTSTD_PUBLIC_HEADERS}")

install(
    TARGETS notstd
    EXPORT notstd
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/notstd
)
