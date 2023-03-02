
add_library(windev-uwb-simulator STATIC "")

set(WINDEVUWBSIM_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE}/windows/devices/uwb/simulator)

target_sources(windev-uwb-simulator
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbDeviceSimulator.cxx
    PUBLIC
        ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceSimulator.hxx
        ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSessionSimulator.hxx
)

target_include_directories(windev-uwb-simulator
    PUBLIC
        ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(windev-uwb-simulator
    PRIVATE
        windev-util
    PUBLIC
        uwb-proto-fira
        uwbsim-driver
        WIL::WIL
        windev-uwb
)

list(APPEND WINDEVUWBSIM_PUBLIC_HEADERS
    ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceSimulator.hxx
    ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSessionSimulator.hxx
)

set_target_properties(windev-uwb-simulator PROPERTIES FOLDER windows/devices/uwb)
set_target_properties(windev-uwb-simulator PROPERTIES PUBLIC_HEADER "${WINDEVUWBSIM_PUBLIC_HEADERS}")

install(
    TARGETS windev-uwb-simulator 
    EXPORT uwbsimulator
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/devices/uwb/simulator
)
