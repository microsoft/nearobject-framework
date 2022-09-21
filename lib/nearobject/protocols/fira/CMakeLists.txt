add_library(nearobject-proto-fira)

set(NO_PROT_FIRA_DIR_PUBLIC_INCLUDE ${NO_DIR_PUBLIC_INCLUDE})
set(NO_PROT_FIRA_DIR_PUBLIC_INCLUDE_PREFIX ${NO_PROT_FIRA_DIR_PUBLIC_INCLUDE}/nearobject/protocols/fira)

target_sources(nearobject-proto-fira
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/FiraDevice.cxx
    PUBLIC
        ${NO_PROT_FIRA_DIR_PUBLIC_INCLUDE_PREFIX}/FiraDevice.hxx
)

target_include_directories(nearobject-proto-fira
    PUBLIC
        ${NO_PROT_FIRA_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(nearobject-proto-fira
    PRIVATE
        nearobject-proto-fira-uci
)

set_target_properties(nearobject-proto-fira PROPERTIES FOLDER lib/nearobject/protocol/fira)

add_subdirectory(uci)
