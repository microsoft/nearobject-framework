
add_library(windows-devices-uwb-simulator STATIC "")

set(WINDEVUWBSIM_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE}/windows/devices/uwb/simulator)

target_sources(windows-devices-uwb-simulator
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbDeviceSimulator.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbDeviceSimulatorConnector.cxx
        ${CMAKE_CURRENT_LIST_DIR}/UwbSessionSimulator.cxx
    PUBLIC
        ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX}/IUwbDeviceSimulatorDdi.hxx
        ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceSimulator.hxx
        ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceSimulatorConnector.hxx
        ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSessionSimulator.hxx
)

target_include_directories(windows-devices-uwb-simulator
    PUBLIC
        ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(windows-devices-uwb-simulator
    PRIVATE
        plog::plog
        windows-devices-util
    PUBLIC
        uwb-proto-fira
        uwbsim-driver
        WIL::WIL
        windows-devices-uwb
)

list(APPEND WINDEVUWBSIM_PUBLIC_HEADERS
    ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX}/IUwbDeviceSimulatorDdi.hxx
    ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceSimulator.hxx
    ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceSimulatorConnector.hxx
    ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX}/UwbSessionSimulator.hxx
)

set_target_properties(windows-devices-uwb-simulator PROPERTIES FOLDER windows/devices/uwb)
set_target_properties(windows-devices-uwb-simulator PROPERTIES PUBLIC_HEADER "${WINDEVUWBSIM_PUBLIC_HEADERS}")

install(
    TARGETS windows-devices-uwb-simulator 
    EXPORT uwbsimulator
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/devices/uwb/simulator
)
