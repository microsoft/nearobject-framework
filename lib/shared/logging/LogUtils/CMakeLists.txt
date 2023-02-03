
add_library(LogUtils STATIC "")

set(LOGUTILS_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(LOGUTILS_DIR_PUBLIC_INCLUDE_PREFIX ${LOGUTILS_DIR_PUBLIC_INCLUDE}/LogUtils)

target_sources(LogUtils
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/LogUtils.cxx
    PUBLIC
        ${LOGUTILS_DIR_PUBLIC_INCLUDE_PREFIX}/LogUtils.hxx
)

target_include_directories(LogUtils
    PUBLIC
        ${LOGUTILS_DIR_PUBLIC_INCLUDE}
)

list(APPEND LOGUTILS_PUBLIC_HEADERS
    ${LOGUTILS_DIR_PUBLIC_INCLUDE_PREFIX}/LogUtils.hxx
)

set_target_properties(LogUtils PROPERTIES FOLDER shared/LogUtils)
set_target_properties(LogUtils PROPERTIES PUBLIC_HEADER "${LOGUTILS_PUBLIC_HEADERS}")

install(
    TARGETS LogUtils
    EXPORT LogUtils
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/LogUtils
)
