
add_executable(uwb-test)

target_sources(uwb-test
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/protocols/fira/TestUwbFiraControleePreference.cxx
        ${CMAKE_CURRENT_LIST_DIR}/protocols/fira/TestUwbFiraRegulatoryInformation.cxx
        ${CMAKE_CURRENT_LIST_DIR}/protocols/fira/TestUwbFiraSecureRangingInfo.cxx
        ${CMAKE_CURRENT_LIST_DIR}/protocols/fira/TestUwbFiraStaticRangingInfo.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestUwbDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestUwbMacAddress.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestUwbPeer.cxx
)

target_link_libraries(uwb-test
    PRIVATE
        Catch2::Catch2WithMain
        uwb
        uwb-proto-fira
)

catch_discover_tests(uwb-test)
