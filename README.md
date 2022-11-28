# Near Object (NO) Framework 

[![Build Release [CMake]](https://github.com/aep-microsoft/nearobject-framework/actions/workflows/cmake.yml/badge.svg)](https://github.com/aep-microsoft/nearobject-framework/actions/workflows/cmake.yml)
[![CodeQL](https://github.com/aep-microsoft/nearobject-framework/actions/workflows/codeql.yml/badge.svg?branch=main)](https://github.com/aep-microsoft/nearobject-framework/actions/workflows/codeql.yml)
[![Dependency Review](https://github.com/aep-microsoft/nearobject-framework/actions/workflows/dependency-review.yml/badge.svg)](https://github.com/aep-microsoft/nearobject-framework/actions/workflows/dependency-review.yml)

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

[Visual Studio 2022](https://visualstudio.microsoft.com/thank-you-downloading-visual-studio/?sku=Enterprise&channel=Release&version=VS2022&source=VSLandingPage&cid=2030&passive=false) generally satisfies the requirements, however, the full integrated development environment (IDE) is not needed. A much leaner alternative for those using other editors such as Visual Studio Code can instead install [Visual Studio Build Tools](https://visualstudio.microsoft.com/downloads/#build-tools-for-visual-studio-2022). The build tools come with a C++ compatible compiler and standard library. Detailed development environment setup instructions can be found in the Windows [`README`](/windows/README.md).

#### Linux

g++ or llvm/clang are suitable, however, some care must be taken to obtain a compatible standard library. A known, working environment is ubuntu 22.04 (jammy) with clang 14.0.0 and LLVM 14.0.0. Both are both provided by the official ubuntu package repository so can be installed using `apt`. Detailed development environment setup instructions can be found in the Linux [`README`](/linux/README.md).

### Obtaining the source

#### Git Configuration

To access the repository, you must configure git with your user name, email, and ssh key. To configure your git user name and email, execute the following commands:

```bash
git config --global user.name "Andrew Beltrano"
git config --global user.email anbeltra@microsoft.com
```

If you do not have a pre-existing key, generate a new key with OpenSSH per your preferences, or a default key per the below command, accepting all of the defaults:
```bash
ssh-keygen
```

This will generate a new RSA key pair, with the private key under the user home directory `~/.ssh/id_rsa` and associated public key at `~/.ssh/id_rsa.pub` (`%HOME%\.ssh` on Windows). The public key must be registered with your GitHub user account to be able to access the repository. Get the contents of the public key:
```bash
cat ~/.ssh/id_rsa.pub
> <key-content>
```

In a browser, navigate to the GitHub SSH keys page at [https://github.com/settings/keys](https://github.com/settings/keys) and click 'New SSH Key'. Give the key a descriptive name in the `Title` box, ensure the `Key type` is set to `Authentication Key`, then copy + paste the complete output of the above `cat` command in the `Key` box. It should look something like this:

![GitHub Add SSH Key](/assets/github_add_ssh_key.png)

Then click `Add SSH key`. This should pop up a notification indicating the key was added and bring you back to the SSH keys page. Here, find the key that was added, then click `Configure SSO`. This should show `aep-microsoft` under the `Single sign-on organizations`; click the `Authorize` button beside it and complete the authentication procedure. Once completed, you are now able to access the repository:

```bash
git clone git@github.com:aep-microsoft/nearobject-framework.git
> Cloning into 'nearobject-framework'...
```

#### Commit Signing

While not required, it is strongly recommended to configure git with a [GNU Privacy Guard (GPG)](https://gnupg.org/) signing key. This allows GitHub to verify commits were pushed by a specific user and will show a green `Verified` status beside each verified commit. Follow these steps to configure a signing key for commit verification:

1. Install the gpg tools for the target operating system (see the OS-specific `README` files for details).
2. [Generate a gpg key](https://docs.github.com/en/authentication/managing-commit-signature-verification/generating-a-new-gpg-key)
3. [Add the gpg key to your github account](https://docs.github.com/en/authentication/managing-commit-signature-verification/adding-a-gpg-key-to-your-github-account)
4. [Configure git to use the gpg key](https://docs.github.com/en/authentication/managing-commit-signature-verification/telling-git-about-your-signing-key)

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

### Source Dependencies

Source dependencies can be satisfied in two (2) ways:

1. From a URL using the CMake [`FetchContent`](https://cmake.org/cmake/help/latest/module/FetchContent.html) module (default).
2. From the [`vcpkg`](https://github.com/microsoft/vcpkg) package manager.

The `FetchContent` method works out of the box with no setup required, so is the default method. The `vcpkg` method requires one-time bootstrapping, described below.

#### vcpkg

The `vcpkg` source dependency method is configurable through the CMake build option `USE_VCPKG`. This option defaults to `OFF` since `FetchContent` is much faster for development loop tasks. Set this to `ON` to use `vcpkg` for source dependency resolution (eg. via command line argument such as `cmake -DNOF_USE_VCPKG:BOOL=ON .`, or by editing the CMake cache directly). Note that this is a cache setting, thus it is sticky; once set, the chosen setting will be stored in the CMake variable cache (`CMakeCache.txt`) and so it must be explicitly reset to `OFF` when no longer desired.

`vcpkg` has extra dependencies on Linux including curl, zip, unzip, tar, and pkg-config. These can be installed on Debian or Ubuntu distributions using the following command:

```bash
sudo apt-get install curl zip unzip tar pkg-config
```

Once configured in the build, the `vcpkg` binary must be made available. This can be installed globally for the entire system, locally for the current user, or directly in the project workspace.

For project-direct installation, the project has been configured to include `vcpkg` source as a sub-module @ `./vcpkg` and attempt to automatically bootstrap it. If this fails for some reason, the following steps can be taken to manually bootstrap `vcpkg`:

The `vcpkg` submodule must first be initialized and updated to sync it to the fixed, known working release tag, which is accomplished by issuing the following git command at the project root:

```Shell
git submodule update --init --recursive
```

Next, run the OS-specific bootstapping script (`./vcpkg/bootstrap-vcpkg.[bat|sh]`). This should produce the binary `./vcpkg/vcpkg`.
