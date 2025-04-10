cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 20)

project(vrmlproc)

# --------------------------------------------------------------------------------------- #

# Create the DLL.
add_library(vrmlproc SHARED

    # Third party libraries.
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/result/result.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../vendor/json/nlohmann"

    # DLL export macro.
    "src/VrmlProcessingExport.hpp"

    # Core.
    "src/core/Object.hpp"
    "src/core/io/FileReader.hpp"
    "src/core/io/FileWriter.hpp"
    "src/core/io/SimpleFileReader.hpp"
    "src/core/io/SimpleFileReader.cpp"
    "src/core/io/MemoryMappedFileReader.hpp"
    "src/core/io/MemoryMappedFileReader.cpp"
    "src/core/io/JsonFileReader.hpp"
    "src/core/io/errors/IoError.hpp"
    "src/core/config/VrmlProcConfig.hpp"
    "src/core/config/Config.hpp"

    "src/core/errors/Error.hpp"
    "src/core/errors/NullPointerError.hpp"
    "src/core/errors/UnsupportedOperationError.hpp"
    "src/core/errors/JsonError.hpp"

    "src/core/utils/ScopedTimer.hpp"
    "src/core/utils/ManualTimer.hpp"
    "src/core/utils/TypeToString.hpp"
    "src/core/utils/Range.hpp"
    "src/core/utils/FormatString.hpp"
    "src/core/utils/Hash.hpp"

    "src/core/logger/Logger.hpp"
    "src/core/logger/Logger.cpp"

    "src/core/contracts/Comparable.hpp"

    # Parser.
    "src/parser/models/VrmlUnits.hpp"
    "src/parser/models/VrmlFile.hpp"

    "src/parser/models/VrmlField.hpp"
    "src/parser/models/Vec3f.hpp"
    "src/parser/models/Vec2f.hpp"
    "src/parser/models/Vec4f.hpp"
    "src/parser/models/Vec2fArray.hpp"
    "src/parser/models/Vec3fArray.hpp"
    "src/parser/models/Int32Array.hpp"
    "src/parser/models/VrmlNode.hpp"
    "src/parser/models/UseNode.hpp"

    "src/parser/models/utils/VrmlFieldExtractor.hpp"
    "src/parser/models/utils/ExtractorCache.hpp"
    "src/parser/models/validators/ModelValidator.hpp"
    "src/parser/models/validators/errors/ModelValidationError.hpp"

    "src/parser/services/VrmlNodeManager.hpp"
    "src/parser/services/VrmlNodeManager.cpp"
    "src/parser/services/VrmlNodeManagerPopulator.hpp"
    "src/parser/services/VrmlNodeManagerPopulator.cpp"

    "src/parser/models/utils/VrmlTreePrinter.hpp"
    "src/parser/models/utils/VrmlTreePrinter.cpp"

    "src/parser/BufferView.hpp"
    "src/parser/Parser.hpp"
    "src/parser/ParserResult.hpp"
    "src/parser/VrmlParser.hpp"
    "src/parser/VrmlParser.cpp"

    "src/parser/errors/ParserError.hpp"

    "src/parser/grammars/BaseGrammar.hpp"
    "src/parser/grammars/CommentSkipper.hpp"
    "src/parser/grammars/IdentifierGrammar.hpp"
    "src/parser/grammars/Vec2fGrammar.hpp"
    "src/parser/grammars/Vec3fGrammar.hpp"
    "src/parser/grammars/Vec2fArrayGrammar.hpp"
    "src/parser/grammars/Vec3fArrayGrammar.hpp"
    "src/parser/grammars/Vec4fGrammar.hpp"
    "src/parser/grammars/Int32ArrayGrammar.hpp"
    "src/parser/grammars/QuotedStringGrammar.hpp"
    "src/parser/grammars/BooleanGrammar.hpp"
    "src/parser/grammars/VrmlFileGrammar.hpp"

    # Conversion context.
    "src/conversion_context/BaseConversionContext.hpp"
    "src/conversion_context/Mergeable.hpp"
    "src/conversion_context/VectorConversionContext.hpp"

    # Traversors.
    "src/traversors/VrmlNodeTraversor.hpp"
    "src/traversors/VrmlFileTraversor.hpp"
    "src/traversors/VrmlNodeTraversorParameters.hpp"
    "src/traversors/VrmlFileTraversorParameters.hpp"

    "src/traversors/errors/FileTraversorError.hpp"
    "src/traversors/errors/NodeTraversorError.hpp"

    "src/traversors/node_descriptors/NodeDescriptor.hpp"
    "src/traversors/node_descriptors/NodeDescriptorFieldType.hpp"
    "src/traversors/node_descriptors/NodeView.hpp"
    
    "src/traversors/node_descriptors/NodeDescriptorMap.hpp"
    "src/traversors/node_descriptors/VrmlCanonicalHeaders.hpp"

    "src/traversors/utils/ConversionContextActionExecutor.hpp"

    "src/traversors/handlers/HandlerResult.hpp"
    "src/traversors/handlers/HandlerToActionBundle.hpp"
    "src/traversors/handlers/HandlerUtils.hpp"
    "src/traversors/handlers/ShapeHandler.hpp"
    "src/traversors/handlers/GroupHandler.hpp"
    "src/traversors/handlers/TransformHandler.hpp"
    "src/traversors/handlers/WorldInfoHandler.hpp"
    "src/traversors/handlers/BoxHandler.hpp"
    "src/traversors/handlers/IndexedFaceSetHandler.hpp"
    "src/traversors/handlers/IndexedLineSetHandler.hpp"
    "src/traversors/handlers/CoordinateHandler.hpp"
    "src/traversors/handlers/NormalHandler.hpp"
    "src/traversors/handlers/ColorHandler.hpp"
    "src/traversors/handlers/TextureCoordinateHandler.hpp"
    "src/traversors/handlers/SwitchHandler.hpp"

    "src/traversors/validation/NodeValidationUtils.hpp"
    "src/traversors/validation/errors/NodeValidationError.hpp"

    # Actions.
    "src/actions/ConversionContextActionMap.hpp"
    "src/actions/ConversionContextAction.hpp"

    # Math
    "src/math/Transformation.hpp"
    "src/math/TransformationMatrix.hpp"
    "src/math/TransformationMatrix.cpp"
    "src/math/Angle.hpp"
    "src/math/Quaternion.hpp"
    "src/math/cgal/CGALBaseTypesForVrml.hpp"
)

# Add source files and vendor dependencies.
target_include_directories(vrmlproc PUBLIC

    ${CMAKE_CURRENT_SOURCE_DIR}/src

    ${CMAKE_CURRENT_SOURCE_DIR}/src/core
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/io
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/io/errors
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/contracts
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/errors
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/logger
    ${CMAKE_CURRENT_SOURCE_DIR}/src/core/config

    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/models
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/models/validators
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/models/validators/errors
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/models/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/interfaces
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/grammars
     ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/errors
    ${CMAKE_CURRENT_SOURCE_DIR}/src/parser/services

    ${CMAKE_CURRENT_SOURCE_DIR}/src/conversion_context

    ${CMAKE_CURRENT_SOURCE_DIR}/src/actions

    ${CMAKE_CURRENT_SOURCE_DIR}/src/traversors
    ${CMAKE_CURRENT_SOURCE_DIR}/src/traversors/node_descriptors
    ${CMAKE_CURRENT_SOURCE_DIR}/src/traversors/utils
    ${CMAKE_CURRENT_SOURCE_DIR}/src/traversors/errors
    ${CMAKE_CURRENT_SOURCE_DIR}/src/traversors/validation
    ${CMAKE_CURRENT_SOURCE_DIR}/src/traversors/validation/errors
    ${CMAKE_CURRENT_SOURCE_DIR}/src/traversors/handlers

    ${CMAKE_CURRENT_SOURCE_DIR}/src/math
    ${CMAKE_CURRENT_SOURCE_DIR}/src/math/cgal

    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/result
    ${CMAKE_CURRENT_SOURCE_DIR}/../vendor/json
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

# Explicitly link Boost::iostreams (problems with compilation on Linux with GNU 11.4.0).
target_link_libraries(vrmlproc PRIVATE CGAL::CGAL Boost::iostreams Boost::log Boost::date_time Boost::log_setup Boost::system Boost::thread)

# --------------------------------------------------------------------------------------- #

if (INCLUDE_TESTS STREQUAL "ON")
message(STATUS "Tests will be included")

# Add the test executables.
add_executable(VrmlFileGrammarUnitTests "tests/VrmlFileGrammarTest.cpp")
add_executable(Vec3fGrammarUnitTests "tests/Vec3fGrammarTest.cpp")
add_executable(Vec4fGrammarUnitTests "tests/Vec4fGrammarTest.cpp")
add_executable(Vec3fArrayGrammarUnitTests "tests/Vec3fArrayGrammarTest.cpp")
add_executable(Int32ArrayGrammarUnitTests "tests/Int32ArrayGrammarTest.cpp")
add_executable(VrmlFieldExtractorUnitTests "tests/VrmlFieldExtractorTest.cpp")
add_executable(OtherVrmlProcComponentsTest "tests/OtherVrmlProcComponentsTest.cpp")

# Link Catch2 with the test executables.
target_link_libraries(VrmlFileGrammarUnitTests PRIVATE Catch2::Catch2WithMain vrmlproc)
target_link_libraries(Vec3fGrammarUnitTests PRIVATE Catch2::Catch2WithMain vrmlproc)
target_link_libraries(Vec4fGrammarUnitTests PRIVATE Catch2::Catch2WithMain vrmlproc)
target_link_libraries(Vec3fArrayGrammarUnitTests PRIVATE Catch2::Catch2WithMain vrmlproc)
target_link_libraries(Int32ArrayGrammarUnitTests PRIVATE Catch2::Catch2WithMain vrmlproc)
target_link_libraries(VrmlFieldExtractorUnitTests PRIVATE Catch2::Catch2WithMain vrmlproc)
target_link_libraries(OtherVrmlProcComponentsTest PRIVATE Catch2::Catch2WithMain vrmlproc)

# Enable testing (use CTest to run the unit tests).
enable_testing()
add_test(NAME VrmlFileGrammarUnitTests COMMAND VrmlFileGrammarUnitTests)
add_test(NAME Vec3fGrammarUnitTests COMMAND Vec3fGrammarUnitTests)
add_test(NAME Vec4fGrammarUnitTests COMMAND Vec4fGrammarUnitTests)
add_test(NAME Vec3fArrayGrammarUnitTests COMMAND Vec3fArrayGrammarUnitTests)
add_test(NAME Int32ArrayGrammarUnitTests COMMAND Int32ArrayGrammarUnitTests)
add_test(NAME VrmlFieldExtractorUnitTests COMMAND VrmlFieldExtractorUnitTests)
add_test(NAME OtherVrmlProcComponentsTest COMMAND OtherVrmlProcComponentsTest)

endif()