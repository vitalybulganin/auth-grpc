cmake_minimum_required(VERSION 3.5)
set(PROJECT_NAME auth_grpc)
project(${PROJECT_NAME})

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
# set output folders
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin CACHE PATH "Single Directory for all")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib CACHE PATH "Single Directory for all")
# Include cmake file.
include(cmake/CMakeLists.txt)
# Dependency libraries.
set(GRPC_LIBS ${GRPC_LIBS} grpc++ grpc gpr ${PROTOBUF_LIBS} grpc++_reflection)
set(SSL_LIBS ${SSL_LIBRARIES} ssl crypto z)
set(CURL_LIBS ${CURL_LIBS} curl)
set(THREAD_LIBS ${THREAD_LIBS} pthread dl)
# Finds GRPC package.
find_package(Threads)
find_package(protobuf REQUIRED)
find_package(gRPC CONFIG REQUIRED)
link_libraries(gRPC::grpc++)
# Includes sources.
file(GLOB_RECURSE SOURCE_INCS src/*.h src/*.hpp)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp src/*.cc)

add_executable(${PROJECT_NAME} ${SOURCE_INCS} ${SOURCE_FILES})
target_link_libraries(
    ${PROJECT_NAME}
    ${GRPC_LIBS}
    ${SSL_LIBS}
    ${CURL_LIBS}
    ${THREAD_LIBS}
)
