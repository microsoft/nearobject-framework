
include(FetchContent)

if (USE_VCPKG)
  find_package(Catch2 CONFIG ${VERSION_CATCH2} REQUIRED)
else()
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v${VERSION_CATCH2}
    GIT_SHALLOW ON
  )
  FetchContent_MakeAvailable(Catch2)
endif()

# Allow CTest to discover Catch2 tests automatically
list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/contrib")
include(Catch)

add_executable(nearobject-test)

target_sources(nearobject-test
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/NearObjectIdentityTokenTest.hxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectDeviceControllerDiscoveryAgent.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectDeviceManager.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectIdentityTokenUwb.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectManager.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectProfile.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectProfileManager.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectProfilePersister.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectService.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectSession.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectSessionIdGeneratorRandom.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestTlvSimple.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestTlvBer.cxx
)

target_link_libraries(nearobject-test
    PRIVATE
        Catch2::Catch2WithMain
        nearobject
        notstd
        tlv
        uwb
)

catch_discover_tests(nearobject-test)

add_subdirectory(notstd)
add_subdirectory(uwb)

if (BUILD_FOR_WINDOWS)
    add_subdirectory(windows)
endif()
