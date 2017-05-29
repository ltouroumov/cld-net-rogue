cmake_minimum_required(VERSION 3.7)
project(prez_game)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake ${CMAKE_MODULE_PATH})

set(EXT_PATH ${CMAKE_CURRENT_LIST_DIR}/ext)

include_directories(${EXT_PATH}/include)
link_directories(${EXT_PATH}/lib)

set(PROTOBUF_LIBRARY ${EXT_PATH}/lib/libprotobuf.a)
set(PROTOBUF_PROTOC_LIBRARY ${EXT_PATH}/lib/libprotoc.a)
set(PROTOBUF_INCLUDE_DIR ${EXT_PATH}/include)
set(PROTOBUF_PROTOC_EXECUTABLE ${EXT_PATH}/bin/protoc)

add_subdirectory(client)
add_subdirectory(server)
