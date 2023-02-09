
add_library(logging-plog-windows STATIC "")

set(PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE_PREFIX ${PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE}/logging/windows)

target_sources(logging-plog-windows
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/PlogTraceLogging.cxx
        ${CMAKE_CURRENT_LIST_DIR}/PlogDebugWrapper.cxx
    PUBLIC
        ${PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE_PREFIX}/PlogTraceLogging.hxx
        ${PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE_PREFIX}/PlogDebugWrapper.hxx
)

target_include_directories(logging-plog-windows
    PUBLIC
        ${PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(logging-plog-windows
    PUBLIC
        plog::plog
        logutils
)

list(APPEND PLOGTRACELOGGING_PUBLIC_HEADERS
    ${PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE_PREFIX}/PlogTraceLogging.hxx
    ${PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE_PREFIX}/PlogDebugWrapper.hxx
)

set_target_properties(logging-plog-windows PROPERTIES FOLDER shared/PlogTraceLogging)
set_target_properties(logging-plog-windows PROPERTIES PUBLIC_HEADER "${PLOGTRACELOGGING_PUBLIC_HEADERS}")

install(
    TARGETS logging-plog-windows
    EXPORT logging-plog-windows
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/logging-plog-windows
)
