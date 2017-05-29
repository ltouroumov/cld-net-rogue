cmake_minimum_required(VERSION 3.7)
project(prez_game)

# Require SFML
find_package(SFML 2 COMPONENTS system window graphics REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_FLAGS -Wno-deprecated-declarations)
set(EXECUTABLE prez_game_client)
include(ProtobufTools)

set(PROTOBUF_GENERATE_CPP_APPEND_PATH YES)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ../shared/game.proto)
include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(../shared/src)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/imgui)

file(GLOB SOURCE_FILES src/*.cpp src/**/*.cpp imgui/*.cpp)
file(GLOB SOURCE_FILES_SHARED ../shared/src/*.cpp ../shared/src/**/*.cpp)
add_executable(${EXECUTABLE} ${PROTO_SRCS} ${PROTO_HDRS} ${SOURCE_FILES} ${SOURCE_FILES_SHARED})
target_link_libraries(${EXECUTABLE} yaml-cpp rltk GLU GL ${SFML_LIBRARIES} curl crypto aws-cpp-sdk-core aws-cpp-sdk-gamelift protobuf pthread)

# message("-- " ${CMAKE_SOURCE_DIR})

add_custom_command(
        TARGET ${EXECUTABLE} PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ARGS
        ${CMAKE_SOURCE_DIR}/client/assets
        ${CMAKE_CURRENT_BINARY_DIR}/assets)

add_custom_command(
        TARGET ${EXECUTABLE} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/shared/data
        ${CMAKE_CURRENT_BINARY_DIR}/data)