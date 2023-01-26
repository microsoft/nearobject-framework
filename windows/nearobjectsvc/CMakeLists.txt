
add_executable(nearobjectsvc "")

target_sources(nearobjectsvc
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
)

target_link_libraries(nearobjectsvc
    PRIVATE
        nearobject-service-windows
        notstd
)

set_target_properties(nearobjectsvc PROPERTIES FOLDER windows/service)
