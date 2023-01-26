# Windows UWB Simulator Driver

This project is a simulated UMDF driver for Ultra-Wideband (UWB) devices on Windows. It is intended to be used to test components of the [nearobject framework](https://github.com/microsoft/nearobject-framework).

## Project Structure

This project is organized to make use of the common development libraries provided by the nearobject-framework. However, being a UMDF Windows driver, it is not (yet) supported to be configured or built directly with CMake (see [here](https://gitlab.kitware.com/cmake/cmake/-/issues/23643) and [here](https://developercommunity.visualstudio.com/t/wdk-cmake-support/795126)). Consequently, it is a traditional MSBuild UMDF driver project using the `WindowsUserModeDriver10.0` platform toolset.

## Development Environment Setup

Pre-requisites:

* [vcpkg](https://vcpkg.io/en/index.html)
* [Visual Studio 2022](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Enterprise&rel=17)
* [Windows 11 SDK version 22H2 SDK](https://developer.microsoft.com/en-us/windows/downloads/windows-sdk/)
* [Windows Driver Kit (WDK) version 22H2 SDK](https://go.microsoft.com/fwlink/?linkid=2196230)

Follow the instructions on the [Windows WDK download page](https://learn.microsoft.com/en-us/windows-hardware/drivers/download-the-wdk) to install the above pre-requisites. In Step 1 when installing Visual Studio and selecting workloads, in addition to the `Desktop development with C++` workload, you must also install the `Linux and embedded development with C++` workload to obtain built-in support for vcpkg. The vcpkg support applies to Windows as well, but the feature started out on Linux and hasn't yet been moved to an appropriate VS installer feature set option.

The project also contains a build task for Visual Studio Code. However, it is simple and only meant for convenience. Most build tasks and deployment scenarios will be much easier to accomplish from Visual Studio.
