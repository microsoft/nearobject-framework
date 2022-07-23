
add_library(nearobject-service STATIC "")

target_sources(nearobject-service
    PRIVATE
        Service.hxx
        Service.cxx
)

target_include_directories(nearobject-service
    PRIVATE
        ${NOF_DIR_LIB}/nearobject/core
)

target_link_libraries(nearobject-service
    PRIVATE
        Threads::Threads
)

set_target_properties(nearobject-service PROPERTIES FOLDER lib/nearobject/service)
