
add_library(linuxdevuwb STATIC "")

set(LINUXDEVUWB_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(LINUXDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX ${LINUXDEVUWB_DIR_PUBLIC_INCLUDE}/linux/uwb)

target_sources(linuxdevuwb
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbDevice.cxx
    PUBLIC
        ${LINUXDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDevice.hxx
        ${LINUXDEVUWB_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceDriver.hxx
)

target_include_directories(linuxdevuwb
    PUBLIC
        ${LINUXDEVUWB_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(linuxdevuwb
    PUBLIC
        uwb
)

set_target_properties(linuxdevuwb PROPERTIES FOLDER linux/devices)
