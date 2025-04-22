cmake_minimum_required(VERSION 3.10.0)

project(honoursproject VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(CTest)
enable_testing()

find_package(nlohmann_json CONFIG REQUIRED)
find_package(boost_graph CONFIG REQUIRED)
find_package(boost_heap CONFIG REQUIRED)
find_package(shapelib CONFIG REQUIRED)

# Include directories
include_directories(
        ${PROJECT_SOURCE_DIR}/HonoursProject
)

add_executable(honoursproject
    HonoursProject/PathSolverBuilder.cpp
    HonoursProject/Analysis.cpp
    HonoursProject/BaseContractionGraph.cpp
    HonoursProject/DijkstraSolverBuilder.hpp
    HonoursProject/Analyser.hpp
    HonoursProject/Dijkstra.hpp
    HonoursProject/BuildTimesAnalyser.cpp
    HonoursProject/ShortcutHierarchy.cpp
    HonoursProject/BaseGraph.hpp
    HonoursProject/OSMParser.cpp
    HonoursProject/CICHBuilder.hpp
    HonoursProject/TestCase.cpp
    HonoursProject/Logger.inl
    HonoursProject/TestSuite.cpp
    HonoursProject/CachedPathSolverBuilder.hpp
    HonoursProject/Logger.hpp
    HonoursProject/BUContractionQueue.inl
    HonoursProject/RoadDParser.cpp
    HonoursProject/Dijkstra.inl
    HonoursProject/BUCHBuilder.cpp
    HonoursProject/BuildTimesAnalyser.hpp
    HonoursProject/ContractionHierarchy.cpp
    HonoursProject/DijkstraSolverBuilder.cpp
    HonoursProject/HDCalculator.hpp
    HonoursProject/GraphAnalyser.cpp
    HonoursProject/DijkstraSolver.hpp
    HonoursProject/Output.hpp
    HonoursProject/TestCase.hpp
    HonoursProject/BUCHBuilder.hpp
    HonoursProject/CachedPathSolverBuilder.inl
    HonoursProject/BaseContractionGraph.inl
    HonoursProject/ShortcutGraph.hpp
    HonoursProject/GraphParser.hpp
    HonoursProject/StreamOutput.hpp
    HonoursProject/Serialize.inl
    HonoursProject/JSON.hpp
    HonoursProject/Analyser.cpp
    HonoursProject/BaseContractionGraph.hpp
    HonoursProject/StreamOutput.cpp
    HonoursProject/WeightedGraph.hpp
    HonoursProject/BUContractionQueue.hpp
    HonoursProject/CIHierarchyBuilder.hpp
    HonoursProject/ShortcutHierarchy.hpp
    HonoursProject/Exec.hpp
    HonoursProject/Output.cpp
    HonoursProject/FileAnalyser.cpp
    HonoursProject/TestInst.hpp
    HonoursProject/Util.hpp
    HonoursProject/TestSuite.hpp
    HonoursProject/Filters.hpp
    HonoursProject/ContractionHierarchy.hpp
    HonoursProject/Main.cpp
    HonoursProject/GraphParser.cpp
    HonoursProject/ShapefileParser.cpp
    HonoursProject/WeightedGraph.cpp
    HonoursProject/OSMParser.hpp
    HonoursProject/Profiler.inl
    HonoursProject/Logger.cpp
    HonoursProject/FileOutput.cpp
    HonoursProject/PathSolverBuilder.hpp
    HonoursProject/HDCalculator.cpp
    HonoursProject/PathSolver.hpp
    HonoursProject/ShortcutGraph.cpp
    HonoursProject/ShortcutHierarchyAnalyser.hpp
    HonoursProject/GraphAnalyser.hpp
    HonoursProject/CIHierarchyBuilder.cpp
    HonoursProject/Factory.hpp
    HonoursProject/RoadDParser.hpp
    HonoursProject/Factory.inl
    HonoursProject/FileAnalyser.hpp
    HonoursProject/ShortcutHierarchyAnalyser.cpp
    HonoursProject/Serialize.hpp
    HonoursProject/CICHBuilder.cpp
    HonoursProject/ShapefileParser.hpp
    HonoursProject/Profiler.hpp
    HonoursProject/Analysis.hpp
    HonoursProject/JSON.inl
    HonoursProject/ContractionGraph.hpp
    HonoursProject/FileOutput.hpp
    HonoursProject/ContractionGraph.cpp
    HonoursProject/Profiler.cpp
    HonoursProject/DijkstraSolver.cpp
    HonoursProject/BaseGraph.inl
    HonoursProject/TestInst.cpp
)

target_link_libraries(honoursproject PRIVATE nlohmann_json::nlohmann_json Boost::graph Boost::heap shapelib::shp)

if (PROJECT_IS_TOP_LEVEL AND UNIX)
    execute_process(
        COMMAND ${CMAKE_COMMAND} -E create_symlink
            ${CMAKE_BINARY_DIR}/compile_commands.json
            ${CMAKE_CURRENT_SOURCE_DIR}/compile_commands.json
    )
endif()