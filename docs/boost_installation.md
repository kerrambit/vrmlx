# Boost Setup Guide

## Prerequisites

Before you can build this project, you'll need to have the Boost libraries installed on your system. Boost provides a collection of powerful libraries that are widely used in C++ development.

### Windows Installation

1. **Download Boost**:
   - The easiest way how to get Boost on Windows is to download prebuilt binaries. Check [Boost C++ Libraries Files on SourceForge](https://sourceforge.net/projects/boost/files/boost-binaries/), choose version and download
     executable file such as *boost_1_87_0-msvc-14.3-64.exe*. Run the installer and place unzipped data for example to the ```/vendor``` folder.

2. **(Manual building)**:
   - **Before continuing, note that build pipeline was tested only with prebuilt binaries!**
   - Visit the [Boost Downloads Page](https://www.boost.org/users/download/) and download the latest Boost release (e.g., `boost_1_87_0.zip`).
   - Extract the Boost archive to a directory of your choice. For example, extract it to:
     ```
     C:\path\to\your\boost
     ```
   
   - You can use also ```vendor``` folder in this project to keep your dependencies organized.
   - You should now have a folder structure like:
     ```
     C:\path\to\your\boost
     ├── boost
     ├── doc
     ├── libs
     ├── more
     ├── status
     └── tools
     ```
   
   - You also need to build Boost (to create libraries like `boost_filesystem`). You can use the following commands:
     1. Open a **Developer Command Prompt for Visual Studio**.
     2. Navigate to the Boost root directory:
        ```
        cd C:\path\to\your\boost
        ```
     3. Run the Boost build system (`b2`) to build the libraries:
        ```
        bootstrap.bat
        b2
        ```

4. **Set the `BOOST_ROOT` environment variable**:
   - Set the `BOOST_ROOT` environment variable to the Boost installation directory.
   - This step is important, because CMake uses `BOOST_ROOT` environment variable to set up the project.
   - If you do not know how to set up environment variables, see the following steps:
        - Go to **System Properties** → **Advanced** → **Environment Variables**.
        - Under **System variables**, click **New** and add:
            - **Variable Name**: `BOOST_ROOT`
            - **Variable Value**: `C:\path\to\your\boost`

5. **Verify the setup**:
- To verify that Boost is correctly set up, you can run the CMake command in your project directory. It should detect Boost automatically if `BOOST_ROOT` is set.
- Note that you might need to delete CMake cache.

---

### Linux Installation

1. **Install Boost via package manager**:
- On most Linux distributions, you can install Boost via your package manager. For example:

  **Ubuntu/Debian**:
  ```
  sudo apt-get install libboost-all-dev
  ```

  **Fedora**:
  ```
  sudo dnf install boost-devel
  ```

  **Arch Linux**:
  ```
  sudo pacman -S boost
  ```

2. **Alternatively, build Boost from source**:
- If you prefer to build Boost from source or need a specific version:
  1. Download the Boost source tarball from the [Boost Downloads Page](https://www.boost.org/users/download/).
  2. Extract the tarball:
     ```
     tar -xvzf boost_1_86_0.tar.gz
     ```
  3. Navigate to the Boost directory:
     ```
     cd boost_1_86_0
     ```
  4. Run the Boost build system:
     ```
     ./bootstrap.sh
     ./b2
     ```

3. **Set the `BOOST_ROOT` environment variable**:
- You might need to set `BOOST_ROOT` environment variable when building Boost from source files.
- In the case you need it, proceed with setting the `BOOST_ROOT` environment variable to the Boost installation directory.
- 
    - Add the following line to your `.bashrc`, `.zshrc`, or other shell configuration file:
    ```
    export BOOST_ROOT=/path/to/your/boost
    ```
    - After editing the file, reload it:
    ```
    source ~/.bashrc  # or `source ~/.zshrc`
    ```
- If this does not set up the Boost correctly, please use [this](#cmake-configuration) approach.

4. **Verify the setup**:
- To verify that Boost is correctly set up, you can run the CMake command in your project directory. It should detect Boost automatically if `BOOST_ROOT` is set.

---

## CMake Configuration

Once Boost is installed and the `BOOST_ROOT` environment variable is set, CMake will automatically detect Boost when you run the configuration for your project.

If Boost is not found, you may need to specify the path manually using the `BOOST_ROOT` variable in your `CMakeLists.txt`:

```cmake
set(BOOST_ROOT "/path/to/your/boost")
```

Or if this approach does not work for you, try to run ```cmake``` command like this with your specified path:
```
 cmake -DBOOST_ROOT=/path/to/your/boost -DBoost_NO_SYSTEM_PATHS=ON -DBoost_NO_BOOST_CMAKE=ON ..
```
