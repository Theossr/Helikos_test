cmake_minimum_required(VERSION 3.15)
project(EarthSimulation LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

find_package(Protobuf REQUIRED)
find_package(gRPC REQUIRED)
find_package(GTest REQUIRED)

set(PROTO_DIR ${CMAKE_CURRENT_SOURCE_DIR}/proto)
include_directories(
    ${PROTO_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

file(GLOB EARTH_PB_SRCS
    "${PROTO_DIR}/*pb.cc"
    "${PROTO_DIR}/*grpc.pb.cc"
)

add_library(EarthCore src/EarthCore.cpp)
target_include_directories(EarthCore PUBLIC src)

add_executable(EarthSimulation
    src/EarthSim.cpp
    ${EARTH_PB_SRCS}
)

target_include_directories(EarthSimulation PRIVATE ${Protobuf_INCLUDE_DIRS} ${PROTO_DIR})
target_link_libraries(EarthSimulation
    PRIVATE
    EarthCore            
    gRPC::grpc++
    protobuf::libprotobuf
)

enable_testing()
include(GoogleTest)

add_executable(EarthCoreTests tests/EarthCoreTests.cpp)
target_link_libraries(EarthCoreTests
    PRIVATE
    EarthCore
    GTest::gtest_main
    pthread
)

gtest_discover_tests(EarthCoreTests)
