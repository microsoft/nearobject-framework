
add_library(windevuwbsimulator STATIC "")

set(WINDEVUWBSIM_DIR_PUBLIC_INCLUDE ${CMAKE_CURRENT_LIST_DIR}/include)
set(WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE}/windows/devices/uwb/simulator)

target_sources(windevuwbsimulator
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/UwbDeviceSimulator.cxx
    PUBLIC
        ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceSimulator.hxx
)

target_include_directories(windevuwbsimulator
    PUBLIC
        ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE}
)

target_link_libraries(windevuwbsimulator
    PRIVATE
        windevutil
    PUBLIC
        uwb-proto-fira
        uwbsim-driver
        WIL::WIL
)

list(APPEND WINDEVUWBSIM_PUBLIC_HEADERS
    ${WINDEVUWBSIM_DIR_PUBLIC_INCLUDE_PREFIX}/UwbDeviceSimulator.hxx
)

set_target_properties(windevuwbsimulator PROPERTIES FOLDER windows/devices/uwb)
set_target_properties(windevuwbsimulator PROPERTIES PUBLIC_HEADER "${WINDEVUWBSIM_PUBLIC_HEADERS}")

install(
    TARGETS windevuwbsimulator 
    EXPORT uwbsimulator
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/windows/uwb/simulator
)
