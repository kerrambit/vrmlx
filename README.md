# vrmlxpy
Toolkit for parsing and traversing VRML files.
Includes a standalone VRML parser library and a conversion library for transforming VRML into geometry format (such as STL), with modular C++ backends and Python bindings.

The modular architecture allows users to define their own actions—custom functions that process VRML nodes in any desired way. This flexibility enables conversions beyond STL, such as transforming VRML data into a custom geometry JSON format. Simply implement the necessary actions to achieve your desired output.

## License
This project is licensed under the **GNU General Public License v3.0 or later** (GPL-3.0-or-later). See the [LICENSE](LICENSE) file for more details.

## Run as Docker container
- Recommended way how to run *vrmlxpy* is to build *Docker* image and run it.
- See steps in this [manual](doc/docker_steps.md).

## [*Experimental*] Run as Python library
- Use *vrmlxpy* as a *Python* library - you do not need to build anything yourself.
- Please visit [official vrmlxpy PyPi page](https://pypi.org/project/vrmlxpy/) to read more.
- Basically, the steps are to install *vrmlxpy* library via ```pip install vrmlxpy``` command.
- To get the idea how to use the library in action, have a look at example [script](scripts/run_vrmlxpy.py).
- **Note that it has not been tested!** It is possible that on Linux machine, you have to have certain *Boost* libraries installed. In theory, it should work out of box on Windows machines.

## Prerequisites
- [Boost](https://www.boost.org/) - please follow instructions [here](doc/boost_installation.md)
- [CGAL](https://www.cgal.org/) - study instructions [here](doc/cgal_installation.md)
- [CMake](https://cmake.org/)
- [Ninja](https://ninja-build.org/) - please follow the installation instructions of this software based on your system vendor
- C++ compiler

## Build

- The *vrmlxpy* project primarily consists of two libraries: *vrmlproc* and *togeom*. However, these are not standalone executables. To use them, you need an application that links against these libraries:
	- There are test executables that perform unit testing on the solution.
	- Additionally, you can build and link two C++ applications: *vrmlxpyConversionApp* and *vrmlxpyBulkConversionApp* (more details [here](doc/docker_steps.md)).
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
- **Note that some functionality is not supported on Windows yet as they are linking issues!** For example ```AlphaSphereCalculator``` returns
  ```UnsupportedOperationError``` on Windows in the current state.

## Run C++ executable files
### Linux
- You can run unit tests using *CTest* which is a program that comes with *CMake*.
- Before running the command below, make sure that tests have access to configuration file ```testConfig.json``` ([example file](testConfig.json.example)) which should
  be inside ```vrmlxpy\out\build\<BUILD_CONFIGURATION>\to_geom``` folder. The paths in the configuration file should point to directories with test data (**test data are not part of repository yet!**).
- Run the following commands: ```ctest --test-dir out/build/<BUILD_CONFIGURATION> vrml_proc``` and ```ctest --test-dir out/build/<BUILD_CONFIGURATION>/to_geom```.
- You might want to use ```--verbose``` for the command above.
<!-- Empty line here -->
- Otherwise, you can run C++ applications *vrmlxpyConversionApp* and *vrmlxpyBulkConversionApp* the standart way.

### Windows
- TODO

## Troubleshooting