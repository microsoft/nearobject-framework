
add_executable(uwb-test)

target_sources(uwb-test
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/protocols/fira/TestUwbFiraControleePreference.cxx
        ${CMAKE_CURRENT_LIST_DIR}/protocols/fira/TestUwbFiraRangingConfiguration.cxx
        ${CMAKE_CURRENT_LIST_DIR}/protocols/fira/TestUwbFiraRegulatoryInformation.cxx
        ${CMAKE_CURRENT_LIST_DIR}/protocols/fira/TestUwbFiraSecureRangingInfo.cxx
        ${CMAKE_CURRENT_LIST_DIR}/protocols/fira/TestUwbFiraStaticRangingInfo.cxx
        ${CMAKE_CURRENT_LIST_DIR}/protocols/fira/TestUwbFiraUwbCapability.cxx
        ${CMAKE_CURRENT_LIST_DIR}/protocols/fira/TestUwbFiraUwbConfiguration.cxx
        ${CMAKE_CURRENT_LIST_DIR}/protocols/fira/TestUwbFiraUwbConfigurationBuilder.cxx
        ${CMAKE_CURRENT_LIST_DIR}/protocols/fira/TestUwbFiraUwbSessionData.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestUwbDevice.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestUwbDeviceCallbacks.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestUwbJsonSerializers.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestUwbMacAddress.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestUwbPeer.cxx
)

target_link_libraries(uwb-test
    PRIVATE
        Catch2::Catch2WithMain
        uwb
        uwb-proto-fira
)

set_target_properties(uwb-test PROPERTIES FOLDER test/unit)

catch_discover_tests(uwb-test)
