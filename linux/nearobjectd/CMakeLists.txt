
add_executable(nearobjectd "")

target_sources(nearobjectd
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
)

target_link_libraries(nearobjectd
    PRIVATE
        logging-utils
        nearobject-service-linux
        plog::plog
)

set_target_properties(nearobjectd PROPERTIES FOLDER linux/service/nearobjectd)
