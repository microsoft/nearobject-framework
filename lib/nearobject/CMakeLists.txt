
add_library(nearobject STATIC "")

target_sources(nearobject
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/NearObject.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/NearObjectConnectionProfile.cxx
        ${CMAKE_CURRENT_SOURCE_DIR}/NearObjectSession.cxx
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/NearObject.hxx
        ${CMAKE_CURRENT_SOURCE_DIR}/NearObjectCapabilities.hpp
        ${CMAKE_CURRENT_SOURCE_DIR}/NearObjectConnectionProfile.hxx
        ${CMAKE_CURRENT_SOURCE_DIR}/NearObjectSession.hxx
        ${CMAKE_CURRENT_SOURCE_DIR}/NearObjectSessionEventCallbacks.hxx
)

target_include_directories(nearobject
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${NOF_DIR_LIB}/se
        ${NOF_DIR_LIB}/se/globalplatform
        ${NOF_DIR_LIB}/smartcard
        ${NOF_DIR_LIB}/uwb
        ${NOF_DIR_LIB}/uwb/uci
)

target_link_libraries(nearobject
    PUBLIC
        se
        seglobalplatform
        smartcard
        uwb
)

set_target_properties(nearobject PROPERTIES FOLDER lib/nearobject)

add_subdirectory(protocols)
add_subdirectory(service)