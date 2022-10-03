# Linux UWB Framework

This project tree is meant to exercise and test the OS-agnostic portions of the
framework.

# Development Environment Setup

 As described in the main project [`README`](/README.md), a C++ 20 compiler and CMake are required. So, any distribution satisfying these requirements may be used. A known working environment is ubuntu 22.04 (jammy) with a few development packages. Instructions for setting up this environment are provided below.

 ## 1. Install Ubuntu 22.04 (jammy)

If development on Windows is desired, ubuntu may be installed in WSL using a rootfs image. To install WSL, refer to [https://learn.microsoft.com/en-us/windows/wsl/install](https://learn.microsoft.com/en-us/windows/wsl/install). This will install a default version of ubuntu from the store, which can be removed by running `wsl --unregister Ubuntu`. Follow these steps to install jammy on WSL:

1. Download the ubuntu 20.04 wsl rootfs archive [https://cloud-images.ubuntu.com/releases/22.04/release/ubuntu-22.04-server-cloudimg-amd64-wsl.rootfs.tar.gz](https://cloud-images.ubuntu.com/releases/22.04/release/ubuntu-22.04-server-cloudimg-amd64-wsl.rootfs.tar.gz)
2. Unzip the archive to get the .tar file (eg. [7-zip](https://www.7-zip.org/download.html)). **DO NOT UNTAR THE ARCHIVE**
3. Choose a location to store the WSL ubuntu filesystem image. A good way to keep organized is to use a top-level directory for all wsl filesystem images such as `c:\wsl`, then add a sub-directory for each distribution installed, such as `c:\wsl\focal`, `c:\wsl\fedora35`, etc. It's also recommended to do this on a non-system drive if one is available for performance reasons.
4. Start an elevated command-prompt, and enter the following series of commands:
```Shell
mkdir c:\wsl\jammy
wsl --import ubuntu-jammy c:\wsl\jammy c:\users\myusername\Downloads\ubuntu-22.04-server-cloudimg-amd64-wsl.rootfs.tar
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
sudo apt-get install build-essential git cmake ninja-build clang clang-format clang-tidy llvm lldb
```

The above will install all the tools required to compile, lint, and debug the project.

## 3. Generate an SSH key

An SSH key must be used to clone the repository. Generate a new key, accepting all of the defaults:
```bash
ssh-keygen
```

This will generate a new RSA private key at `~/.ssh/id_rsa` and associated public key at `~/.ssh/id_rsa.pub`. The public key must be registered with your GitHub user account to be able to access the repository. Get the contents of the public key:
```bash
cat ~/.ssh/id_rsa.pub
> <key-content>
```

In a browser, navigate to the GitHub SSH keys page at [https://github.com/settings/keys](https://github.com/settings/keys) and click 'New SSH Key'. Give the key a descriptive name in the `Title` box, ensure the `Key type` is set to `Authentication Key`, then copy + paste the complete output of the above cat command in the `Key` box. It should look something like this:

![GitHub Add SSH Key](/assets/github_add_ssh_key.png)

Then click `Add SSH key`. This should pop up a notification indicating the key was added and bring you back to the SSH keys page. Here, find the key that was added, then click `Configure SSO`. This should show `aep-microsoft` under the `Single sign-on organizations`; click the `Authorize` button beside it and complete the authentication procedure. Once completed, you are now able to access the repository:

```bash
git clone git@github.com:aep-microsoft/nearobject-framework.git
> Cloning into 'nearobject-framework'...
```

Last, configure your git user name and email:
```bash
git config --global user.name "Andrew Beltrano"
git config --global user.email anbeltra@microsoft.com
```

It is also strongly recommended to create and configure a GPG signing key. This allows GitHub to verify commits were pushed by a specific user and will show a `Verified` status (green checkmark) beside each verified commit. Complete instructions for this can be found [here](https://docs.github.com/en/authentication/managing-commit-signature-verification/adding-a-gpg-key-to-your-github-account).
