# Linux UWB Framework

This project tree is meant to exercise and test the OS-agnostic portions of the
framework.

# Development Environment Setup

As described in the main project [`README`](/README.md), a C++ 20 compiler and CMake are required. So, any distribution satisfying these requirements may be used. A known working environment is ubuntu 22.04 (jammy) with a few development packages. Instructions for setting up this environment are provided below.

## 1. Install Ubuntu 22.04 (jammy)

If development on Windows is desired, ubuntu may be installed in WSL using a rootfs image. To install WSL, on newer versions of Windows 10, use the following command: `wsl --install --no-distribution`. For complete instructions, refer to [https://learn.microsoft.com/en-us/windows/wsl/install](https://learn.microsoft.com/en-us/windows/wsl/install). Then follow these steps to install jammy on WSL:

1. Download the ubuntu 22.04 wsl rootfs archive [https://cloud-images.ubuntu.com/wsl/jammy/current/ubuntu-jammy-wsl-amd64-wsl.rootfs.tar.gz](https://cloud-images.ubuntu.com/wsl/jammy/current/ubuntu-jammy-wsl-amd64-wsl.rootfs.tar.gz).
2. Unzip the archive to get the .tar file (eg. [7-zip](https://www.7-zip.org/download.html)). **DO NOT UNTAR THE ARCHIVE**
3. Choose a location to store the WSL ubuntu filesystem image. A good way to keep organized is to use a top-level directory for all wsl filesystem images such as `c:\wsl`, then add a sub-directory for each distribution installed, such as `c:\wsl\focal`, `c:\wsl\fedora35`, etc. It's also recommended to do this on a non-system drive if one is available for performance reasons.
4. Start an elevated command-prompt, and enter the following series of commands:
```Shell
mkdir c:\wsl\jammy
wsl --import jammy c:\wsl\jammy c:\users\myusername\Downloads\ubuntu-22.04-server-cloudimg-amd64-wsl.rootfs.tar
wsl -d jammy
```
5. You are now running in a root shell on a fresh ubuntu 22.04 installation. Set up a non-root user to use by default:
```bash
adduser mycoolusername
# <accept all defaults by successively hitting 'Enter', and set a password>
usermod -G sudo mycoolusername
# install an editor if you don't like the ones pre-installed (vi and nano are available out of the box)
```
6. Create the file `/etc/wsl.conf` (need sudo), and save the following contents to it:
```ini 
[user]
default=mycoolusername
```
7. Exit ubuntu:
```bash
exit
```
8. Shutdown WSL so it can pick up the new default user changes, and re-run ubuntu:
```Shell
wsl --shutdown
wsl -d jammy
```

You should now be logged on as `mycoolusername`.

## 2. Install Development Dependencies

Execute the following commands in a shell:
```bash
sudo apt update
sudo apt-get install -y build-essential git ninja-build clang clang-format clang-tidy llvm lldb gnupg gdb zip unzip tar curl pkg-config wget
```

The above will install all the tools required to compile, lint, and debug the project, except for CMake. The build requires CMake version 3.25+ and the version shipping with Ubuntu 22.04 doesn't meet this requirement. Therefore, it must be installed manually using Kitware's standalone apt repository. Perform the following steps:

1. Remove the default version provided by CMake, in case it was installed:

```bash
sudo apt purge --auto-remove cmake
```

2. Update apt sources and install dependencies:

```bash
sudo apt update && \
sudo apt install -y software-properties-common lsb-release && \
sudo apt clean all
```

3. Add the Kitware apt repository:

```bash
wget -O - https://apt.kitware.com/keys/kitware-archive-latest.asc 2>/dev/null | gpg --dearmor - | sudo tee /etc/apt/trusted.gpg.d/kitware.gpg >/dev/null
sudo apt-add-repository -y "deb https://apt.kitware.com/ubuntu/ $(lsb_release -cs) main"
```

4. Add Kitware's keyring packaging to keep their signing key up to date:

```bash
sudo apt update
sudo apt install kitware-archive-keyring
sudo rm /etc/apt/trusted.gpg.d/kitware.gpg
```

5. Install the CMake package:

```bash
sudo apt update
sudo apt install cmake
```

If running `sudo apt update` outputs the following error regarding signature verification:
> W: An error occurred during the signature verification. The repository is not updated and the previous index files will be used. GPG error: https://apt.kitware.com/ubuntu jammy InRelease: The following signatures couldn't be verified because the public key is not available: NO_PUBKEY 42D5A192B819C5DA

copy the public key (here `42D5A192B819C5DA`), and issue the following command to add it, then re-run apt update and install:

```bash
sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys 42D5A192B819C5DA
sudo apt update
sudo apt install cmake
```
