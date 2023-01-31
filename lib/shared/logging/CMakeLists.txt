
add_library(logging STATIC "")

set(LOGGING_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)

target_sources(logging
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/LogUtils.cxx
    PUBLIC
        ${LOGGING_DIR_PUBLIC_INCLUDE}/LogUtils.hxx
)

target_include_directories(logging
    PUBLIC
        ${LOGGING_DIR_PUBLIC_INCLUDE}
)

list(APPEND LOGGING_PUBLIC_HEADERS
    ${LOGGING_DIR_PUBLIC_INCLUDE}/LogUtils.hxx
)

set_target_properties(logging PROPERTIES FOLDER shared/logging)
set_target_properties(logging PROPERTIES PUBLIC_HEADER "${LOGGING_PUBLIC_HEADERS}")

install(
    TARGETS logging
    EXPORT logging
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/logging
)
