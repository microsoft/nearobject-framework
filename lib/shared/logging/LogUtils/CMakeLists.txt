
add_library(logutils STATIC "")

set(LOGUTILS_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(LOGUTILS_DIR_PUBLIC_INCLUDE_PREFIX ${LOGUTILS_DIR_PUBLIC_INCLUDE}/logging)

target_sources(logutils
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/LogUtils.cxx
    PUBLIC
        ${LOGUTILS_DIR_PUBLIC_INCLUDE_PREFIX}/LogUtils.hxx
)

target_include_directories(logutils
    PUBLIC
        ${LOGUTILS_DIR_PUBLIC_INCLUDE}
)

list(APPEND LOGUTILS_PUBLIC_HEADERS
    ${LOGUTILS_DIR_PUBLIC_INCLUDE_PREFIX}/LogUtils.hxx
)

set_target_properties(logutils PROPERTIES FOLDER shared/logging)
set_target_properties(logutils PROPERTIES PUBLIC_HEADER "${LOGUTILS_PUBLIC_HEADERS}")

install(
    TARGETS logutils
    EXPORT logutils
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/logging
)
