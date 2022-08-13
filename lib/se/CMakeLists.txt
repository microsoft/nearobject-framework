
add_library(se STATIC "")

target_sources(se
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/SecureElement.cxx
        ${CMAKE_CURRENT_LIST_DIR}/SecureElementChannel.cxx
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/SecureElement.hxx
        ${CMAKE_CURRENT_LIST_DIR}/SecureElementChannel.hxx
)

set_target_properties(se PROPERTIES FOLDER lib/se)

add_subdirectory(globalplatform)
