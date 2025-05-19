# vrmlx
Toolkit for parsing and traversing VRML files.
Includes a standalone VRML parser library (```vrmlproc```) and a conversion library for transforming VRML into geometry format such as STL (```togeom```), with modular C++ backends and Python bindings (```vrmlxpy```).

The modular architecture allows users to define their own actions—custom functions that process VRML nodes in any desired way. This flexibility enables conversions beyond STL, such as transforming VRML data into a custom geometry JSON format. Simply implement the necessary actions to achieve your desired output.

## License
This project is licensed under the **GNU General Public License v3.0 or later** (GPL-3.0-or-later). See the [LICENSE](LICENSE) file for more details.

## Manual
- You can find detailed usage instructions in the [manual](manual.md).  

## Run as Python library
- Please visit [official vrmlxpy PyPi page](https://pypi.org/project/vrmlxpy/) to read more.
- Basically, the steps contain only the installation of *vrmlxpy* library via ```pip install vrmlxpy``` command.
- To get the idea how to use the library in action, have a look at example [script](run_vrmlxpy.py).