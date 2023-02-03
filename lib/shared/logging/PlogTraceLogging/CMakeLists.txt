
add_library(PlogTraceLogging STATIC "")

set(PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE_PREFIX ${PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE}/PlogTraceLogging)

target_sources(PlogTraceLogging
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/PlogTraceLogging.cxx
    PUBLIC
        ${PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE_PREFIX}/PlogTraceLogging.hxx
)

target_include_directories(PlogTraceLogging
    PUBLIC
        ${PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE}
)

list(APPEND PLOGTRACELOGGING_PUBLIC_HEADERS
    ${PLOGTRACELOGGING_DIR_PUBLIC_INCLUDE_PREFIX}/PlogTraceLogging.hxx
)

set_target_properties(PlogTraceLogging PROPERTIES FOLDER shared/PlogTraceLogging)
set_target_properties(PlogTraceLogging PROPERTIES PUBLIC_HEADER "${PLOGTRACELOGGING_PUBLIC_HEADERS}")

install(
    TARGETS PlogTraceLogging
    EXPORT PlogTraceLogging
    ARCHIVE
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/PlogTraceLogging
)
