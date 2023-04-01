
add_executable(nearobject-service-host-windows "")

target_sources(nearobject-service-host-windows
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
)

target_link_libraries(nearobject-service-host-windows
    PRIVATE
        nearobject-service-windows
        notstd
        logging-plog-windows
)

set_target_properties(nearobject-service-host-windows PROPERTIES FOLDER windows/service)
