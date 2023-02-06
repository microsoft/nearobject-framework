# Windows UWB Simulator Driver

This project is a simulated UMDF driver for Ultra-Wideband (UWB) devices on Windows. It is intended to be used to test components of the [nearobject framework](https://github.com/microsoft/nearobject-framework).

## Project Structure

This project is organized to make use of the common development libraries provided by the nearobject-framework. However, being a UMDF Windows driver, it is not (yet) supported to be configured or built directly with CMake (see [here](https://gitlab.kitware.com/cmake/cmake/-/issues/23643) and [here](https://developercommunity.visualstudio.com/t/wdk-cmake-support/795126)). Consequently, it is a traditional MSBuild UMDF driver project using the `WindowsUserModeDriver10.0` platform toolset. See the [build architecture section](#build-architecture) for additional details.

## Development Environment Setup

Pre-requisites:

* [vcpkg](https://vcpkg.io/en/index.html)
* [Visual Studio 2022](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Enterprise&rel=17)
* [Windows 11 SDK version 22H2 SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/)
* [Windows Driver Kit (WDK) version 22H2 SDK](https://go.microsoft.com/fwlink/?linkid=2196230)

Follow the instructions on the [Windows WDK download page](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk) to install the above pre-requisites. In Step 1 when installing Visual Studio and selecting workloads, in addition to the `Desktop development with C++` workload, you must also install the `Linux and embedded development with C++` workload to obtain built-in support for vcpkg. The vcpkg support applies to Windows as well, but the feature started out on Linux and hasn't yet been moved to an appropriate VS installer feature set option.

Last, install vcpkg system-wide using the instructions [here](https://devblogs.microsoft.com/cppblog/vcpkg-environment-activation-in-visual-studio/). Once installed, execute the following command: [`vcpkg integrate install`](https://vcpkg.io/en/docs/commands/integrate.html#vcpkg-integrate-install) which integrates vcpkg with Visual Studio.

The project also contains a build task for Visual Studio Code. However, it is simple and only meant for convenience. Most build tasks and deployment scenarios will be much easier to accomplish from Visual Studio.

## Build Architecture

The simulator driver depends on a few libraries from the main nearobject-framework [project](../../../../README.md). These dependencies are resolved using the newly added [vcpkg support in Visual Studio](https://devblogs.microsoft.com/cppblog/vcpkg-environment-activation-in-visual-studio/) (Visual Studio 2022 17.4+). This is controlled by two files in the driver project:

1. [vcpkg.json](./vcpkg.json): The vcpkg "manifest" which defines the dependencies this project depends on.
2. [vcpkg-configuration.json](./vcpkg-configuration.json): The vcpkg configuration which defines where to resolve the dependencies defined in the vcpkg.json manifest.

The main project defines a vcpkg called [`nearobject-framework`](../../../../packaging/vcpkg/ports/nearobject-framework/vcpkg.json) which makes its libraries available via vcpkg tooling. The package exposes a feature called `msvc-link-static-runtime` which forces its libraries to statically link against the VC++ runtime, which is required when using the `WindowsUserModeDriver10.0` platform toolset per [here](https://learn.microsoft.com/en-us/windows-hardware/drivers/develop/using-the-microsoft-c-runtime-with-user-mode-drivers-and-apps). Consequently, this feature is selected in the driver's vcpkg manifest. The package also exposes a ferature called `enable-development` which controls the version of the code the `nearobject-framework` vcpkg will make available. Typically, the package will expose a stable version of the code for public consumption. However, during early development of the feature, it is more common for the inner development loop to use the `HEAD` revision of the code. Consequently, the this feature is currently selected in the driver's vcpkg manifest. Eventually, this will be removed once a baseline working simulator driver has been established.

### Quickstart: How to Build the Driver

0. Ensure all pre-requisites have been installed.
1. Check out the main project if not already done.
2. Configure the project with CMake (see [here](../../../../README.md#cmake)).
3. Open the simulator driver solution file [UwbSimulator.sln](UwbSimulator.sln) in Visual Studio 2022.
4. Build the [`UwbSimulator`](./UwbSimulator.vcxproj) driver project.

### Quickstart: How to push library updates into the driver project

Any time new changes are made to dependent libraries in the main project (eg. `uwb`, `uwbcxadapter`, `uwbcx-driver`, etc.), Visual Studio must be instructed to rebuild the `nearobject-framework` vcpkg. The easiest way to do this is to delete the timestamp file associated with the package. This can be found under `${MSBuildProjectDirectory}/vcpkg_installed/x64-windows/.msbuildstamp-x64-windows.stamp`, for example `D:\src\nearobject-framework\windows\drivers\uwb\simulator\vcpkg_installed\x64-windows\.msbuildstamp-x64-windows.stamp`. So, the process to ingest the changes is:

1. Make the changes to the main project library or libraries and ensure the `install` (or `ALL_BUILD`) target builds successfully.
2. Commit the changes. The changes __do not__ have to be pushed to a remote; committing to the local repository is fine.
3. Manually delete the UwbSimulator driver vcpkg timestamp file (`${MSBuildProjectDirectory}/vcpkg_installed/x64-windows/.msbuildstamp-x64-windows.stamp`).
4. Rebuild the UwbSimulator driver project in Visual Studio.

The project output window should display a line indicating the the `nerarobject-framework` vcpkg needs to be rebuilt, similar to below:

```Shell
1>The following packages will be rebuilt:
1>    nearobject-framework[core,enable-development,msvc-link-static-runtime]:x64-windows -> 1.0.0 -- D:\src\nearobject-framework\windows\drivers\uwb\simulator\../../../../packaging/vcpkg/ports\nearobject-framework
```

The main project will eventually be updated to detect when a commit is made and automatically regenerate the vcpkg port file. Until then, this brief manual process must be followed.
