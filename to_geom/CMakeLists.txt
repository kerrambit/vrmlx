cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

project(togeom)

# --------------------------------------------------------------------------------------- #

# Create the DLL.
add_library(togeom SHARED

    # Third party libraries.
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/result/result.hpp"

    # DLL export macro.
    "src/VrmlToGeomExport.hpp"

    # Core.
    "src/core/Mesh.hpp"
    "src/core/MeshTask.hpp"
    "src/core/io/ExportFormats.hpp"
    "src/core/io/StlFileWriter.hpp"
    "src/core/io/ObjFileWriter.hpp"
    "src/core/io/PlyFileWriter.hpp"
    "src/core/config/ToGeomConfig.hpp"

    # Actions.
    "src/actions/GeometryAction.hpp"
    "src/actions/IndexedFaceSetAction.hpp"
    "src/actions/IndexedFaceSetAction.cpp"      
    "src/actions/IndexedLineSetAction.hpp"
    "src/actions/IndexedLineSetAction.cpp" 
    "src/actions/GroupAction.hpp"
    "src/actions/GroupAction.cpp"
    "src/actions/BoxAction.hpp"
    "src/actions/BoxAction.cpp"
    "src/actions/ShapeAction.hpp"
    "src/actions/ShapeAction.cpp"
    "src/actions/TransformAction.hpp"
    "src/actions/TransformAction.cpp"
    "src/actions/SwitchAction.hpp"
    "src/actions/SwitchAction.cpp"

    # Conversion contexts
    "src/conversion_context/Vec3fArrayConversionContext.hpp"
    "src/conversion_context/MeshTaskConversionContext.hpp"
    "src/conversion_context/ToGeomActionMap.hpp"

    # Calculators.
    "src/calculators/CalculatorResult.hpp"
    "src/calculators/BoxCalculator.hpp"
    "src/calculators/BoxCalculator.cpp"
    "src/calculators/IndexedFaceSetCalculator.hpp"
    "src/calculators/IndexedFaceSetCalculator.cpp"
    "src/calculators/IndexedLineSetCalculator.hpp"
    "src/calculators/IndexedLineSetCalculator.cpp"
    "src/calculators/MeshSimplificator.hpp"
    "src/calculators/errors/CalculatorError.hpp"
)

# Add source files and vendor dependencies.
target_include_directories(togeom PUBLIC 

    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/actions
    ${CMAKE_CURRENT_SOURCE_DIR}/src/conversion_context
    ${CMAKE_CURRENT_SOURCE_DIR}/src/calculators
    ${CMAKE_CURRENT_SOURCE_DIR}/src/calculators/errors
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/io
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/config

    ${CMAKE_CURRENT_SOURCE_DIR}/../vrml_proc/src

    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/result
    ${CGAL_INCLUDE_DIRS}
)

# Two build types - Debug and Release.
if(MSVC)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /Zi")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /O2 /DNDEBUG")
else()
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g")
  set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
endif()

# Offer two macros based on the build type.
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(DEBUG)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(RELEASE)
endif()

# Link the togeom library with vrmlproc shared library.
target_link_libraries(togeom PRIVATE vrmlproc)

# --------------------------------------------------------------------------------------- #

if (INCLUDE_TESTS STREQUAL "ON")
message(STATUS "Tests will be included")

# Add the test executables.
add_executable(VrmlFileTraversorUnitTests "tests/VrmlFileTraversorTest.cpp")
add_executable(CalculatorsUnitTests "tests/CalculatorsTest.cpp")

# Apply /bigobj flag for MSVC.
if (MSVC)
    target_compile_options(VrmlFileTraversorUnitTests PRIVATE /bigobj)
endif()

# Link Catch2 with the test executables.
target_link_libraries(VrmlFileTraversorUnitTests PRIVATE Boost::iostreams Boost::log Boost::date_time Boost::log_setup Boost::system Boost::thread Catch2::Catch2WithMain vrmlproc togeom)
target_link_libraries(CalculatorsUnitTests PRIVATE Catch2::Catch2WithMain vrmlproc togeom)

# Enable testing (use CTest to run the unit tests).
enable_testing()
add_test(NAME VrmlFileTraversorUnitTests COMMAND VrmlFileTraversorUnitTests)
add_test(NAME CalculatorsUnitTests COMMAND CalculatorsUnitTests)

# Copy vrmlproc.dll to the folder for tests to run.
add_custom_command(TARGET VrmlFileTraversorUnitTests POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:vrmlproc>"
        "$<TARGET_FILE_DIR:VrmlFileTraversorUnitTests>"
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "$<TARGET_FILE:togeom>"
        "$<TARGET_FILE_DIR:VrmlFileTraversorUnitTests>"
)

endif()