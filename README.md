# Near Object (NO) Framework

This project is a framework for interacting with short-range devices, providing secure location, ranging, radar, or proximity services. There is a particular focus on [IEEE 802.15.4z-2020](https://standards.ieee.org/ieee/802.15.4z/10230/) Ultra-Wideband (UWB) devices using the [Fine Ranging Consortium (FiRa)](https://www.firaconsortium.org/), however, the framework is not limited to this.

## Project Structure

This project is organized to allow development both in the internal Windows build environment (razzle) and outside of it, for example, using other build systems and/or on alternate operating systems. Hence, build configuration is included for both NTBuild (`build.exe`) and [CMake](https://cmake.org/). Consequently, there is an OS-independent source tree `lib`, and OS-dependent source trees `windows`, `linux`, etc..

Note that all language feature configuration is constrained by the Windows build system since it is the most limiting factor. As such, the current C++ language version being used is C++ 20.

## Coding Guidelines

Where possible, we will attempt to use primitives provided by the [C++ Standard Library](https://en.cppreference.com/w/cpp/header) for interoperability between common and OS-dependent code. The use of OS-specific primitives and libraries is reserved for scenarios where they are strictly needed (eg. calling an OS/System API), or where the highest possible performance is required and only the OS implementation can provide this. 

The coding style is dictated by both `.clang-format` and `.clang-tidy` files at the root of the project. Please configure your editor to format and lint sources accordingly. Above all, ***the coding style should be kept as consistent as possible***. The exact style used is not overly important.

To help keep the code consistent, please follow these general guidelines:

* **DO** use spaces instead of tabs.
* **DON'T** prefix variable names to describe their type or scope.
* **DO** use `std::filesystem` for storage and UNIX path separators (`/`) where possible.
* **DO** use complete words for variable and function names.
* **DO** use the following table for variable naming:

| Block | Style | Example |
| ----- | ----- | -------- |
| Types | PascalCase | `struct NearObject {};` |
| Functions | PascalCase | `NearObject GetNearObject()` |
| Variables | camelCase | `NearObject nearObject;` |
| Parameters | camelCase | `void registerEventCallback(NearObjectEventCallback& eventCallback)` |
| Namespaces | lowercase | `namespace nearobject` |
| Public Members | PascalCase | `struct NearObject { NearObjectProperties Properties; }` |
| Private Members | camelCase with `m_` prefix | `class NearObjectSession { uint64_t m_sessionId; }` |

## Development Environment Setup

Pre-requisites:

* C++ 20 Compiler
* CMake version >= 3.12

### Compiler

Both a compiler and standard C++ library supporting C++20 are required. The C++ reference pages for [C++20 core language features](https://en.cppreference.com/w/cpp/compiler_support#cpp20) and [C++20 library features](https://en.cppreference.com/w/cpp/compiler_support#C.2B.2B20_library_features) provide complete details about current compiler and library support.

#### Windows

[Visual Studio 2022](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Enterprise&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false) generally satisfies the requirements, however, the full integrated development environment (IDE) is not needed. A much leaner alternative for those using other editors such as Visual Studio Code can instead install [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022). The build tools come with a C++ compatible compiler and standard library.

#### Linux

g++ or llvm/clang are suitable, however, some care must be taken to obtain a compatible standard library. A known, working environment is ubuntu 20.04 (focal) with LLVM 15.0.1 and the `libstdc++-10-dev` package. LLVM 15.0.1 can be installed using the [automatic installation script](https://apt.llvm.org/#llvmsh) and `libstdc++-10-dev` can be installed with `sudo apt-get install -y libstdc++-10-dev`.

### CMake

CMake may be installed in any form, as long as the version meets the minimum. One popular way of installing it on Windows is to use [Chocolately](https://chocolatey.org/install) with `choco install -y cmake`. On Linux, all standard package managers provide a cmake package (eg. `apt-get install -y cmake`, `yum install -y cmake`, etc.).

To bootstrap the build environment, instruct CMake to generate the build files. It is strongly recommended to do this in a directory that is separate from the source; this allows one to easily destroy and recreate the build environment without affecting the checked-out source and changes in progress. Typically, a new directory called `build` at the top-level project tree is used for this purpose:

```Shell
git clone git@github.com:abeltrano/nearobject-framework.git
cd nearobject-framework
mkdir build && cd $_ 
cmake ..
cmake --build .
```

#### CMake with Visual Studio Code

Alternatively, Microsoft provides a [CMake Tools](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools) Visual Studio Code extension that automates this process. After installing the extension and cloning the repository in VSCode, hit `Ctrl+Shift+P`, then find the command `CMake: Delete Cache and Reconfigure`. This will generate the build configuration in a `build` folder at the top-level of the source tree. Once done, you can build the `ALL` target (default) with the `CMake: Build` command again, `Ctrl+Shift+P`, type cmake, find the command).

The following shows a list of CMake commands available from the VSCode CMake Tools extension:

![Visual Studio Code CMake Tools Extension Commands](/assets/vscode_cmake_actions.png)

In general, you set a build target and variant, then use the `CMake: Build` command to build incrementally. All build targets can be found in the `CMake: Project Outline` activity bar, but a list of them will also be shown when invoking actions that involve targets:

![Visual Studio Code CMake Project Outline Activity Bar](/assets/vscode_cmake_targets.png)

### Razzle

The source tree is not *currently* configured for building in Razzle. The parts of the tree that can be built in Windows will eventually be augmented with [nmake](https://docs.microsoft.com/en-us/cpp/build/nmake-reference), NTBuild (`build.exe`), and/or MSBuild configuration files.
