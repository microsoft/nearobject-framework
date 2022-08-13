
add_library(se STATIC "")

set(SE_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(SE_DIR_PUBLIC_INCLUDE_PREFIX ${SE_DIR_PUBLIC_INCLUDE}/se)

target_sources(se
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/SecureElement.cxx
        ${CMAKE_CURRENT_LIST_DIR}/SecureElementChannel.cxx
    PUBLIC
        ${SE_DIR_PUBLIC_INCLUDE_PREFIX}/SecureElement.hxx
        ${SE_DIR_PUBLIC_INCLUDE_PREFIX}/SecureElementChannel.hxx
)

target_include_directories(se
    PUBLIC
        ${SE_DIR_PUBLIC_INCLUDE}
)

set_target_properties(se PROPERTIES FOLDER lib/se)

add_subdirectory(globalplatform)
