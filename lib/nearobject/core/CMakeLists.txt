
add_library(nearobject-core STATIC "")

target_sources(nearobject-core
    PRIVATE
        NearObject.cxx
        NearObject.hxx
        NearObjectConnectionProfile.cxx
        NearObjectConnectionProfile.hxx
        NearObjectSession.cxx
        NearObjectSession.hxx
        NearObjectSessionEventCallbacks.hxx
        NearObjectSessionEventHandler.cxx
        NearObjectSessionEventHandler.hxx
)

target_include_directories(nearobject-core
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        ${NOF_DIR_LIB}/se
        ${NOF_DIR_LIB}/se/globalplatform
        ${NOF_DIR_LIB}/smartcard
        ${NOF_DIR_LIB}/uwb
        ${NOF_DIR_LIB}/uwb/uci
)

target_link_libraries(nearobject-core
    PRIVATE
        se
        seglobalplatform
        smartcard
        uwb
)

set_target_properties(nearobject-core PROPERTIES FOLDER lib/nearobject/core)
