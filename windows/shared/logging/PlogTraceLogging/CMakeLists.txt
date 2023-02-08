
add_library(plog-trace-logging STATIC "")

set(PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE_PREFIX ${PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE}/PlogTraceLogging)

target_sources(plog-trace-logging
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/PlogTraceLogging.cxx
    PUBLIC
        ${PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE_PREFIX}/PlogTraceLogging.hxx
)

target_include_directories(plog-trace-logging
    PUBLIC
        ${PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(plog-trace-logging
PUBLIC
    plog::plog
)

list(APPEND PLOGTRACELOGGING_PUBLIC_HEADERS
    ${PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE_PREFIX}/PlogTraceLogging.hxx
)

set_target_properties(plog-trace-logging PROPERTIES FOLDER shared/PlogTraceLogging)
set_target_properties(plog-trace-logging PROPERTIES PUBLIC_HEADER "${PLOGTRACELOGGING_PUBLIC_HEADERS}")

install(
    TARGETS plog-trace-logging
    EXPORT plog-trace-logging
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/plog-trace-logging
)
