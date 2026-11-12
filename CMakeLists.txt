# update Cmake to make it more versatile

cmake_minimum_required(VERSION 3.2.0)

project(tuya_daemon_monitoring
    VERSION 1.0
    LANGUAGES C
)

set(SRC_DIR src)
set(OBJ_DIR build)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(TUYA_SDK_DIR "${CMAKE_SOURCE_DIR}/tuya-iot-core-sdkk")

add_compile_options(
    -Wall
    -Wextra
    -Wno-error
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CJSON REQUIRED IMPORTED_TARGET libcjson)

add_executable(monitor
    src/main.c
    src/daemon.c
    src/config.c
    src/payload.c
    src/cpu_usage.c
    src/memory.c
    src/metrics.c
    src/network_interface.c
    src/system_time.c
    src/tuya_client.c
    ${LINK_SDK_SOURCES}
)

add_subdirectory(tuya-iot-core-sdkk)

target_include_directories(monitor PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${LINK_SDK_INCLUDE_PUBLIC_DIRS}
    "${TUYA_SDK_DIR}/utils"
    "${TUYA_SDK_DIR}/include"
    "${TUYA_SDK_DIR}/interface"
    # ${PROJECT_SOURCE_DIR}/tuya-iot-core-sdkk/include
)

include("${TUYA_SDK_DIR}/tuyaFilePaths.cmake")

target_link_libraries(monitor PRIVATE
    PkgConfig::CJSON
    link_core
    middleware_implementation
    platform_port
    utils_modules
    cjson
)