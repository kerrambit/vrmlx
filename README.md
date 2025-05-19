# vrmlx
Toolkit for parsing and traversing VRML files.
Includes a standalone VRML parser library (```vrmlproc```) and a conversion library for transforming VRML into geometry format such as STL (```togeom```), with modular C++ backends and Python bindings (```vrmlxpy```).

The modular architecture allows users to define their own actions—custom functions that process VRML nodes in any desired way. This flexibility enables conversions beyond STL, such as transforming VRML data into a custom geometry JSON format. Simply implement the necessary actions to achieve your desired output.

## License
This project is licensed under the **GNU General Public License v3.0 or later** (GPL-3.0-or-later). See the [LICENSE](LICENSE) file for more details.

## Manual
- You can find detailed usage instructions in the [manual](doc/manual.md).  
- However, before reading that, make sure to understand how to obtain and run the application as described below.

## Run as Docker container
- Recommended way how to run *vrmlx* is to build *Docker* image and run it.
- See steps in this [manual](doc/docker_steps.md).

## Run as Executables
- Whether you build the project yourself or use the executable from the **Releases** tab, running an executable is straightforward.
- However, unlike using Docker, running the binary directly might expose you to platform-specific issues — for example, missing shared libraries (DLLs or `.so` files).
- **Tip:** When running the application on Linux, you need to configure the dynamic linker to look for shared libraries in the current directory  ```LD_LIBRARY_PATH=. ./vrmlxConversionApp```

## [*Experimental*] Run as Python library
- Please visit [official vrmlxpy PyPi page](https://pypi.org/project/vrmlxpy/) to read more.
- Basically, the steps contain only the installation of *vrmlxpy* library via ```pip install vrmlxpy``` command.
- To get the idea how to use the library in action, have a look at example [script](scripts/run_vrmlxpy.py).
- **Note:** this procedure has not been fully tested! It is possible that on Linux machine, you have to have certain *Boost* libraries installed. In theory, it should work out of box on Windows machines.

## Prerequisites
- [Boost](https://www.boost.org/) - please follow instructions [here](doc/boost_installation.md)
- [CGAL](https://www.cgal.org/) - study instructions [here](doc/cgal_installation.md)
- [CMake](https://cmake.org/)
- [Ninja](https://ninja-build.org/)
- C++ compiler supporting C++20

## Build

- The *vrmlx* project primarily consists of two libraries: *vrmlproc* and *togeom*. However, these are not standalone executables. To use them, you need an application that links against these libraries:
	- There are test executables that perform unit testing on the solution.
	- Additionally, you can build and link C++ application: *vrmlxConversionApp* (more details [here](doc/docker_steps.md)).
	- Lastly, there is a *Python* binding that allows you to use *vrmlx* as a *Python* module ([example script](scripts/run_vrmlxpy_from_docker.py)).

- The project supports these build configurations:

	| Configuration     | Libraries | Tests | C++ Applications | Python Binding |
	|------------------|-----------|-------|------------------|-----------------|
	| **Debug**        | ✅         | ✅     | ✅                | ✅         |
	| **Release**      | ✅         | ✅     | ✅                | ✅         |
	| **Production**   | ✅         | ❌     | ✅                | ❌         |
	| **LibrariesOnly** | ✅         | ❌     | ❌                | ❌        |


### Linux
- You can choose to build the library based on four types of build configurations as seen above:

  ```
	cmake --preset <BUILD_CONFIGURATION>
  ```
- After necessary files are generated, you are able to build.
	```
	cmake --build out/build/<BUILD_CONFIGURATION>
	```

### Windows
- It is recommended to develop in *Visual Studio*.
- To open the project, navigate yourself to ```File - Open - Cmake...``` and choose the root *CMakeLists.txt*.
- *Visual Studio* should handle the process of generating and building the library automatically.

## Run C++ tests

This project includes C++ unit tests that can be executed using either *CTest* or by running the test executables directly. The tests require a configuration file (`testConfig.json`) that defines paths to input and expected test data.

### Linux
- Use the [```testConfig.linux.json```](testConfig.linux.json) file as a base. Copy or rename it to `testConfig.json`.
- Place this file inside the build output directory:  
  ```vrmlx/out/build/<BUILD_CONFIGURATION>/to_geom```.
- The configuration file references input and expected data located at:
  - Input: `vrmlx/data/segmentation_data`
  - Expected: `vrmlx/data/expected_data`
- Input data is archived. Make sure to extract it first (either `archive.tar.gz` or `archive.zip`).
- You can run tests using **CTest**:
  ```ctest --test-dir out/build/<BUILD_CONFIGURATION>/vrml_proc``` and 
  ```ctest --test-dir out/build/<BUILD_CONFIGURATION>/to_geom```.
- Add the ```--verbose``` flag for more detailed output: ```ctest --test-dir out/build/<BUILD_CONFIGURATION>/to_geom --verbose```.

### Windows
- Use the [```testConfig.windows.json```](testConfig.windows.json) file as a base. Copy or rename it to `testConfig.json`.
- Place this file inside: ```vrmlx\out\build\<BUILD_CONFIGURATION>\to_geom```.
- The configuration file references input and expected data located at:
  - Input: `vrmlx\data\segmentation_data`
  - Expected: `vrmlx\data\expected_data`
- Make sure the input data archive (```archive.zip``` or ```rchive.tar.gz```) has been extracted.
- You can run tests using **CTest** from Command Prompt or PowerShell:
  ``` ctest --test-dir out\build\<BUILD_CONFIGURATION>\vrml_proc``` and
  ```ctest --test-dir out\build\<BUILD_CONFIGURATION>\to_geom```.
- Add ```--verbose``` for detailed output: ```ctest --test-dir out\build\<BUILD_CONFIGURATION>\to_geom --verbose```.

## Developer documentation
- Documentation is available at https://kerrambit.github.io/vrmlx/gh_pages/doxygen/index.html.

## Troubleshooting