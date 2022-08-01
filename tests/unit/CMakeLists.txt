
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

target_link_libraries(nearobject-test
    PRIVATE
        Catch2::Catch2
        nearobject
)

target_include_directories(nearobject-test
    PRIVATE
        ${NOF_DIR_LIB}
)

target_sources(nearobject-test
    PRIVATE
        Main.cxx
        TestNearObjectDeviceDiscoveryAgent.cxx
        TestNearObjectDeviceManager.cxx
        TestNearObjectManager.cxx
        TestNearObjectProfileManager.cxx
        TestNearObjectService.cxx
        TestNearObjectSession.cxx
)

catch_discover_tests(nearobject-test)

if (BUILD_FOR_WINDOWS)
    add_subdirectory(windows)
endif()
