cmake_minimum_required(VERSION 3.7)
project(prez_game)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -Wno-deprecated-declarations)
set(EXECUTABLE prez_game_server)
include(ProtobufTools)

add_definitions(-DGAMELIFT_USE_STD)
add_definitions(-DHAS_GAMELIFT)

set(PROTOBUF_GENERATE_CPP_APPEND_PATH YES)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../shared/game.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(../shared/src)

file(GLOB SOURCE_FILES src/*.cpp src/**/*.cpp)
file(GLOB SOURCE_FILES_SHARED ../shared/src/*.cpp ../shared/src/**/*.cpp)
add_executable(${EXECUTABLE} ${PROTO_SRCS} ${PROTO_HDRS} ${SOURCE_FILES} ${SOURCE_FILES_SHARED})
target_link_libraries(${EXECUTABLE} yaml-cpp aws-cpp-sdk-gamelift-server sioclient boost_date_time boost_random boost_system protobuf pthread)

add_custom_command(
        TARGET ${EXECUTABLE} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shared/data
        ${CMAKE_CURRENT_BINARY_DIR}/data)