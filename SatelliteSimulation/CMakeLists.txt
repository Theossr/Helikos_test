cmake_minimum_required(VERSION 3.15)
project(SatelliteSimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(GTest REQUIRED)

set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto)
include_directories(${PROTO_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/external/sgp4/CMakeLists.txt)
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/external/sgp4 ${CMAKE_CURRENT_BINARY_DIR}/sgp4_build)
    set(SGP4_LIB sgp4)
else()
    message(STATUS "external/sgp4 not found as cmake project; you may need to compile/link sgp4 manually")
    set(SGP4_LIB /home/theossr/Helikos_test/SatelliteSimulation/external/sgp4/build/libsgp4/libsgp4.a)
endif()

file(GLOB SAT_PB_SRCS "${PROTO_DIR}/*pb.cc" "${PROTO_DIR}/*grpc.pb.cc")

add_executable(SatelliteSimulation
    src/SatSim.cpp
    src/OrbitNodes.cpp
    ${SAT_PB_SRCS}
)

target_include_directories(SatelliteSimulation PRIVATE
    ${Protobuf_INCLUDE_DIRS}
    ${PROTO_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(SatelliteSimulation PRIVATE
    gRPC::grpc++
    protobuf::libprotobuf
    ${SGP4_LIB}
)

enable_testing()
include(GoogleTest)

set(TEST_SRCS
    tests/OrbitNodesTests.cpp
    src/OrbitNodes.cpp
)

add_executable(OrbitNodesTests ${TEST_SRCS})

target_include_directories(OrbitNodesTests PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${PROTO_DIR}
)


target_link_libraries(OrbitNodesTests
    PRIVATE
    GTest::gtest_main
    pthread
    protobuf::libprotobuf
    gRPC::grpc++
    ${SGP4_LIB}
)

gtest_discover_tests(OrbitNodesTests)
