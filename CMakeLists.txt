cmake_minimum_required(VERSION 3.10)
project(vrmlx)

set(CMAKE_CXX_STANDARD 20)

if (MSVC)
    add_compile_options(/bigobj)
endif()

if(WIN32)
    # On Windows, use static Boost libraries to avoid DLL dependency issues.
    # This ensures that Boost is linked statically into the executable, making distribution easier.
    set(Boost_USE_STATIC_LIBS ON)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
else()
    # On Linux, prefer dynamic Boost libraries for better compatibility and smaller binaries.
    # Most Linux distributions provide Boost as shared libraries, making this the default.
    set(Boost_USE_STATIC_LIBS OFF)
    set(Boost_USE_MULTITHREADED ON)
    set(Boost_USE_STATIC_RUNTIME OFF)
endif()

# CMake will look for Boost libraries specified by BOOST_ROOT variable.
find_package(Boost REQUIRED COMPONENTS iostreams log date_time log_setup thread system)
if (Boost_FOUND)
    message(STATUS "Boost found!")
    message(STATUS "Boost include directories: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost libraries: ${Boost_LIBRARIES}")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "Boost was not found. Please set BOOST_ROOT to your Boost installation.")
endif()

# Set up GTM and MPFR libraries on Windows.
if(WIN32)
    message(STATUS "GMP_DIR from env: $ENV{GMP_DIR}")
    message(STATUS "MPFR_DIR from env: $ENV{MPFR_DIR}")
    set(GMP_ROOT "$ENV{GMP_DIR}")
    set(MPFR_ROOT "$ENV{MPFR_DIR}")
    set(GMP_INCLUDE_DIR "${GMP_ROOT}/include")
    set(GMP_LIBRARY "${GMP_ROOT}/lib/gmp.lib")
    set(MPFR_INCLUDE_DIR "${MPFR_ROOT}/include")
    set(MPFR_LIBRARY "${GMP_ROOT}/lib/mpfr.lib")
endif()

set(CGAL_CMAKE_EXACT_NT_BACKEND "GMP")
set(CGAL_GMP_INCLUDE_DIR "${GMP_INCLUDE_DIR}")
set(CGAL_MPFR_INCLUDE_DIR "${MPFR_INCLUDE_DIR}")
set(CGAL_GMP_LIBRARIES "${GMP_LIBRARY}")
set(CGAL_MPFR_LIBRARIES "${MPFR_LIBRARY}")
set(CGAL_Boost_INCLUDE_DIR ${Boost_INCLUDE_DIRS})
set(CGAL_Boost_LIBRARY_DIR ${Boost_LIBRARY_DIRS})
set(CGAL_Boost_USE_STATIC_LIBS ${Boost_USE_STATIC_LIBS})  # Ensure CGAL links Boost the same way
set(CGAL_Boost_USE_MULTITHREADED ${Boost_USE_MULTITHREADED})
set(CGAL_DO_NOT_USE_BOOST_MP ON)

# Find CGAL.
find_package(CGAL CONFIG COMPONENTS Core REQUIRED)
if(CGAL_FOUND)
    message(STATUS "CGAL found!")
    message(STATUS "CGAL version: ${CGAL_VERSION}")
    message(STATUS "CGAL include directory: ${CGAL_INCLUDE_DIRS}")
    message(STATUS "CGAL libraries: ${CGAL_LIBRARIES}")
    message(STATUS "GMP library: ${CGAL_GMP_LIBRARIES}")
    message(STATUS "MPFR library: ${CGAL_MPFR_LIBRARIES}")
    include_directories(${CGAL_INCLUDE_DIRS} ${GMP_INCLUDE_DIR} ${MPFR_INCLUDE_DIR})  
    link_directories(${CGAL_LIBRARIES}  ${CGAL_GMP_LIBRARIES} ${CGAL_MPFR_LIBRARIES})
else()
    message(FATAL_ERROR "CGAL not found. Please set CGAL_DIR to the correct path.")
endif()

# Find TaskFlow.
include(FetchContent)
FetchContent_Declare(
  taskflow
  GIT_REPOSITORY https://github.com/taskflow/taskflow.git
  GIT_TAG        master
)
FetchContent_MakeAvailable(taskflow)

# If tests will be compiled, find Catch2 framework.
if (INCLUDE_TESTS STREQUAL "ON")
message(STATUS "Catch2 will be fetched")

# Add Catch2 unit tests using FetchContent.
include(FetchContent)
FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.4.0
)
FetchContent_MakeAvailable(Catch2)

endif()

# Add the vrml_proc and to_geom directories.
add_subdirectory(vrml_proc)
add_subdirectory(to_geom)

# Create Python bindings.
if (INCLUDE_PYTHON_BINDINGS STREQUAL "ON")
message(STATUS "Python bindings will be included")

# Add pybind11 using FetchContent.
FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11.git
    GIT_TAG v2.13.6
)

FetchContent_MakeAvailable(pybind11)

# Create the Python bindings module.
pybind11_add_module(vrmlxpy SHARED src/vrmlx.cpp src/bindings.cpp)

# Link against C++ libraries.
target_link_libraries(vrmlxpy PRIVATE vrmlproc togeom pybind11::module Boost::iostreams Boost::log Boost::date_time Boost::log_setup Boost::system Boost::thread)

# Ensure vrmlx.cpp can access headers from vrml_proc, to_geom and pybind11 projects.
target_include_directories(vrmlxpy PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vrml_proc
    ${CMAKE_CURRENT_SOURCE_DIR}/to_geom
    #${CMAKE_CURRENT_SOURCE_DIR}/../vendor/pybind11/include/pybind11
)

# Copy vrmlproc.dll and togeom.dll to the folder for the binding to run.
add_custom_command(TARGET vrmlxpy POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:vrmlproc>"
        "$<TARGET_FILE_DIR:vrmlxpy>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:togeom>"
        "$<TARGET_FILE_DIR:vrmlxpy>"
)

endif()

# Create C++ runner application.
if (INCLUDE_RUNNER STREQUAL "ON")
message(STATUS "C++ CLI runners will be included")

add_executable(vrmlxConversionApp "src/vrmlx.cpp" "src/vrmlxConversionApp.cpp")

# Link against C++ libraries.
target_link_libraries(vrmlxConversionApp PRIVATE vrmlproc togeom Boost::iostreams Boost::log Boost::date_time Boost::log_setup Boost::system Boost::thread)

# Ensure vrmlx.cpp and vrmlxConversionApp.cpp can access headers from vrml_proc and to_geom projects.
target_include_directories(vrmlxConversionApp PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/vrml_proc
    ${CMAKE_CURRENT_SOURCE_DIR}/to_geom
)

# Copy vrmlproc.dll and togeom.dll to the folder for the runner to run.
add_custom_command(TARGET vrmlxConversionApp POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:vrmlproc>"
        "$<TARGET_FILE_DIR:vrmlxConversionApp>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:togeom>"
        "$<TARGET_FILE_DIR:vrmlxConversionApp>"
)

endif()