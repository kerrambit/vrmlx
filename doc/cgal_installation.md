# CGAL Setup Guide

## Prerequisites

Before you can build this project, you'll need to have the CGAL libraries installed on your system. CGAL is an open source software project that provides an access to geometric algorithms.

### Windows Installation

1. **Download CGAL**:
   - You can use [official site](https://www.cgal.org/download/windows.html) for steps. However, we provide our steps which should offer non-problematic way how to use it in our project.
   - So, firstly, download desired archive from *Releases* on [official CGAL GitHug page](https://github.com/CGAL/cgal) and place unzipped data for example to the ```/vendor``` folder.
   
2. **CGAL prerequisites**:
   ##### GMP and MPFR
   - Precompiled version of GMP and MPFR are provided in the assets of CGAL library (*Releases* on [official CGAL GitHug page](https://github.com/CGAL/cgal)).
   - Extract *CGAL-[some version]-win64-auxiliary-libraries-gmp-mpfr.zip* archive inside the CGAL directory in ```\vendor``` folder. You should get the directory ```vendor\CGAL-[some version]\auxiliary\gmp```.
   - Then set up two new environment variables poitning to the path ```auxiliary\gml```.
        - ```GMP_DIR = C:\dev\vrmlxpy\vendor\CGAL-6.0.1\auxiliry\gmp```
        - ```MPFR_DIR = C:\dev\vrmlxpy\vendor\CGAL-6.0.1\auxiliry\gmp```
   - If you do not know how to set up environment variables, see the following steps:
        - Go to **System Properties** → **Advanced** → **Environment Variables**.
        - Under **System variables**, click **New** and add name and path.
   - The reason why we set up two individual varibles if that if you install these libraries yourself, you might have different paths for both programs.
   
   ##### Boost
   - Boost is a mandatory dependency of CGAL. The installation process for Boost is already described [here](boost_installation.md).
   - You need also to set up two new environment variables to point respectively to the path of the libraries and the headers (note that you have to use the right location of Boost on your system):
        - ```BOOST_LIBRARYDIR = C:\dev\libboost_1_71_0\lib64-msvc-XX.Y```
        - ```BOOST_INCLUDEDIR = C:\dev\libboost_1_71_0```

3. **Install CGAL**:
    - To install CGAL, CMake needs to know the location of CGAL on your system. You have to set up an environment variable to point to ```CGALConfig.cmake``` file:
        - ``` CGAL_DIR = C:\dev\CGAL-6.0.1\lib\cmake\CGAL ```

---

### Linux Installation

1. **Install Boost via package manager**:
- On most Linux distributions, you can install CGAL via your package manager. For example:

  **Ubuntu/Debian**:
  ```
  sudo apt-get install libcgal-dev
  ```
- It pulls in a bunch of required packages, including: ```libgmp-dev``` for the GMP development and ```libmpfr-dev``` for the MPFR development.
---

## CMake Configuration

Once CGAL is installed and the `CGAL_DIR` environment variable is set, CMake will automatically detect CGAL when you run the configuration for your project.