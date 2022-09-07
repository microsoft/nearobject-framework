
include(FetchContent)

FetchContent_Declare(
  Catch2
  GIT_REPOSITORY https://github.com/catchorg/Catch2.git
  GIT_TAG v2.13.9
  GIT_SHALLOW ON
)

FetchContent_MakeAvailable(Catch2)

# Allow CTest to discover Catch2 tests automatically
list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/contrib")
include(Catch)

add_executable(nearobject-test)

target_sources(nearobject-test
    PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}/Main.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectDeviceDiscoveryAgent.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectDeviceManager.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectManager.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectProfile.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectProfileManager.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectProfilePersister.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectService.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestNearObjectSession.cxx
        ${CMAKE_CURRENT_LIST_DIR}/TestTlvSimple.cxx
)

target_link_libraries(nearobject-test
    PRIVATE
        Catch2::Catch2
        nearobject
        notstd
        tlv
)

catch_discover_tests(nearobject-test)

add_subdirectory(notstd)

if (BUILD_FOR_WINDOWS)
    add_subdirectory(windows)
endif()
