set(SCHEMA_PATH ${PROJECT_SOURCE_DIR}/protos)

find_program(PROTOC "protoc")
IF(NOT PROTOC)
    message(FATAL_ERROR "protoc is not found")
    return()
ENDIF()

if (NOT GRPC_CPP_PLUGIN)
    find_program(GRPC_CPP_PLUGIN NAMES grpc_cpp_plugin)
endif()

if (NOT GRPC_CPP_PLUGIN)
    message(FATAL_ERROR "grpc_cpp_plugin not found, try to run cmake with -DGRPC_CPP_PLUGIN=/path/to/plugin or install it")
    return()
endif()
# mark_as_advanced(GRPC_CPP_PLUGIN)
set(GRPC_CPP_PLUGIN "C:/software/grpc/bin/grpc_cpp_plugin.exe")
message(STATUS "Using grpc cpp plugin ${GRPC_CPP_PLUGIN}")

if (NOT SCHEMA_PATH)
    set(SCHEMA_PATH $ENV{SCHEMA_PATH})
    if (NOT SCHEMA_PATH)
        if (EXISTS ${SCHEMA_SYSTEM_PATH})
            set(SCHEMA_PATH ${SCHEMA_SYSTEM_PATH})
            message(STATUS "Using schema from ${SCHEMA_PATH}.")
        else()
            message(FATAL_ERROR "schema not found, try to run cmake with -DSCHEMA_PATH=/path/to/schema or set environment variable SCHEMA_PATH or run `make standalone` at ${GRPC_OUTPUT_STUBS_DIR}")
        endif()
    endif(NOT SCHEMA_PATH)
endif(NOT SCHEMA_PATH)
message(STATUS "Found schema ${SCHEMA_PATH}")
# temporary dir to save stubs
set(TEMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/temp)
execute_process(COMMAND mkdir -p ${TEMP_DIR})

function(execute_command_with_check)
    execute_process(${ARGV} RESULT_VARIABLE rv)
    if (${rv})
        execute_process(COMMAND rm -rf ${TEMP_DIR})
        message(FATAL_ERROR "${ARGV}")
    endif()
endfunction()
execute_process(COMMAND rm -rf ${PROJECT_SOURCE_DIR}/src/stubs)
# Getting a list of proto files.
file(GLOB_RECURSE proto_files "${SCHEMA_PATH}/*.proto")
# Generating stub files.
execute_command_with_check(COMMAND protoc -I ${SCHEMA_PATH} --cpp_out=${TEMP_DIR} --grpc_out=${TEMP_DIR} --plugin=protoc-gen-grpc=${GRPC_CPP_PLUGIN} ${proto_files})
# preserve stub's modification time if it's content did't change; so that 'make' will not compile it again
execute_command_with_check(COMMAND rsync -rc --delete ${TEMP_DIR}/ ${PROJECT_SOURCE_DIR}/src/stubs)
# Removing temp directory.
execute_process(COMMAND rm -rf ${TEMP_DIR})
# Including stub headers into project.
include_directories("${PROJECT_SOURCE_DIR}/src/stubs")
