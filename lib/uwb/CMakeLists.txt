
add_library(uwb STATIC "")

target_sources(uwb
    PRIVATE
        UwbDevice.cxx
        UwbRangingSession.cxx
        UwbSession.cxx
    PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/UwbDevice.hxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbRangingSession.hxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbSession.hxx
)

target_include_directories(uwb
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
)

set_target_properties(uwb PROPERTIES FOLDER lib/uwb)
