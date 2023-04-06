
# Windows NearObject WinRT API

This project defines the Windows Runtime (WinRT) API for the NearObject service on Windows. The API interface is defined using the [Microsoft Interface Definition Language (MIDL) 3.0](https://learn.microsoft.com/en-us/uwp/midl-3/) and is implemented with the [C++/WinRT](https://learn.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/intro-to-using-cpp-with-winrt) language projection.

CMake support for modern Windows Runtime components using C++/WinRT is not yet mature enough for us to generate this project with CMake, so it is statically defined here instead and added as an external MSBuild project to the CMake project tree.

The following are helpful resources regarding Windows Runtime components, C++/WinRT, and authoring WinRT APIs using C++/WinRT:

* [Introduction to C++/WinRT](https://learn.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/intro-to-using-cpp-with-winrt)
* [Windows Runtime components with C++/WinRT](https://learn.microsoft.com/en-us/windows/uwp/winrt-components/create-a-windows-runtime-component-in-cppwinrt)
* [Author APIs with C++/WinRT](https://learn.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/author-apis#if-youre-authoring-a-runtime-class-in-a-windows-runtime-component)
* [Interop between C++/WinRT and the ABI](https://learn.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/interop-winrt-abi?source=recommendations)

## Development Guidelines

Each WinRT object must be defined in its own `.idl` file which will result in the compiler generating one Windows Metadata `.winmd` file per object. The C++/WinRT toolchain has been configured to merge all component Windows metadata files into a single `.winmd` with named with the root namespace name (here, `Windows.Devices.NearObject.winmd`).

### Adding a new `.idl` file

To add a new `.idl` file for an API component, click `Project -> Add New Item -> View Model (C++/WinRT)` and name the file after the name of the component. For example, if adding an `.idl` file for a component called `NearObject`, then name the file `NearObject.idl`. This template adds a new `.idl` file with a default `runtimeclass` definition and adds C++/WinRT header and source stubs (eg. `NearObject.h`, `NearObject.cpp`) for the implementation and makes them dependent on the `.idl` file.

Re-build the project such that the `cppwinrt.exe` tool will generate new source stubs for the newly added `.idl` file. These generated stub files are found under `<ProjectRoot>/Generated Files/sources/<component-name>.(h|cpp)`. The build will fail (this is expected) but the generated files can now be used as templates. Copy the generated `.h` and `.cpp` files associated with the new `.idl` to the project root, remove the `static_assert` and comments from each, and re-build the project, which should now succeed.

### Updating an existing `.idl` file

TODO
