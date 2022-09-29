
add_library(uwb STATIC "")

set(UWB_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(UWB_DIR_PUBLIC_INCLUDE_PREFIX ${UWB_DIR_PUBLIC_INCLUDE}/uwb)

target_sources(uwb
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbMacAddress.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbSession.cxx
    PUBLIC
        ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDevice.hxx
        ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbMacAddress.hxx
        ${UWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSession.hxx
)

target_include_directories(uwb
    PUBLIC
        ${UWB_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(uwb
    PRIVATE
        magic_enum::magic_enum
    PUBLIC
        notstd
)

set_target_properties(uwb PROPERTIES FOLDER lib/uwb)
