
# Windows UWB Framework

# Development Environment Setup

As described in the main project [`README`](/README.md), a C++ 20 compiler and CMake are required. A known, minimal working environment is using Visual Studio 2022 Build Tools which provides all of the necessary development requirements. Note that the build tools do not provide the Visual Studio IDE, so if this is desired, the Visual Studio product of choice may be installed instead using the component list provided in step 1 below. Instructions for setting up this environment are provided below.

## 1. Install Visual Studio 2022 Build Tools

Download the latest version of [Visual Studio 2022 Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022) and run the installer. Select the 'Individual Components' tab at the top, and select the following required components:

* C++ Build Tools core features
* C++ core features
* C++ CMake tools for Windows
* MSVC v143 - VS 2022 C++ x64/x86 build tools (Latest)
* MSVC v143 - VS 2022 C++ x74/x86 Spectre-mitigated libs (Latest)
* Windows 11 SDK (10.0.22621.0)

The following components are optional, but strongly recommended for a more complete development experience:

* C++ AddressSanitizer
* C++ Clang Compiler for Windows (15.0.1)
* C++ Clang-cl for v143 build tools (x64/x86)

The final list with all optional components would look like this:

![Visual Studio 2022 Build Tools Components List](/assets/vsbuildtools_finallist.png)

Once installed, the project can be built using CMake as described in the main [`README`](/README.md#cmake).