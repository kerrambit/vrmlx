# vrmlxpy
Toolkit for parsing and traversing VRML files.
Includes a standalone VRML parser library and a conversion library for transforming VRML into geometry format (such as STL), with modular C++ backends and Python bindings.

The modular architecture allows users to define their own actions—custom functions that process VRML nodes in any desired way. This flexibility enables conversions beyond STL, such as transforming VRML data into a custom geometry JSON format. Simply implement the necessary actions to achieve your desired output.

## License
This project is licensed under the **GNU General Public License v3.0 or later** (GPL-3.0-or-later). See the [LICENSE](LICENSE) file for more details.

## Run as Docker container
- Recommended way how to run *vrmlxpy* is to build *Docker* image and run it.
- See steps in this [manual](doc/docker_steps.md).

## Run as Executables
- Whether you build the project yourself or use the executable from the **Releases** tab, running an executable is straightforward.
- However, unlike using Docker, running the binary directly might expose you to platform-specific issues — for example, missing shared libraries (DLLs or `.so` files).
- **Tip:** When running the application on Linux, you need to configure the dynamic linker to look for shared libraries in the current directory  ```LD_LIBRARY_PATH=. ./vrmlxpyConversionApp```

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

- The *vrmlxpy* project primarily consists of two libraries: *vrmlproc* and *togeom*. However, these are not standalone executables. To use them, you need an application that links against these libraries:
	- There are test executables that perform unit testing on the solution.
	- Additionally, you can build and link C++ application: *vrmlxpyConversionApp* (more details [here](doc/docker_steps.md)).
	- Lastly, there is a *Python* binding that allows you to use *vrmlxpy* as a *Python* module ([example script](scripts/run_vrmlxpy_from_docker.py)).

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
### Linux
- You can run unit tests using *CTest*, which is a program that comes with *CMake*.
- Before running the command below, make sure that tests have access to configuration file (for Windows, use [```this file```](testConfig.windows.json), and for Linux [```this file```](testConfig.linux.json)), which should placed inside ```vrmlxpy\out\build\<BUILD_CONFIGURATION>\to_geom``` folder (and make sure that the name is only ```testConfig.json```). The paths in these configuration files point to directories with input test data (```vrmlxpy\data\segmentation_data```) and expected data ```vrmlxpy\data\expected_data```.
- Run the following commands: ```ctest --test-dir out/build/<BUILD_CONFIGURATION> vrml_proc``` and ```ctest --test-dir out/build/<BUILD_CONFIGURATION>/to_geom```.
- You might want to use ```--verbose``` for the command above.

### Windows
- TODO

## Troubleshooting