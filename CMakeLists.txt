cmake_minimum_required(VERSION 3.5)

project(tuya_daemon_monitoring
    VERSION 1.0
    LANGUAGES C
)

set(SRC_DIR src)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR})
set(TUYA_SDK_DIR "${CMAKE_SOURCE_DIR}/tuya-iot-core-sdkk")


add_compile_options(
    -Wall
    -Wextra
)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CJSON REQUIRED IMPORTED_TARGET libcjson)

add_executable(tuyad
    src/main.c
    src/daemon.c
    src/config.c
    src/payload.c
    src/cpu_usage.c
    src/ram_memory.c
    src/metrics.c
    src/network_interface.c
    src/system_time.c
    src/tuya_client.c
    src/log_utils.c
    src/signal_utils.c
    ${LINK_SDK_SOURCES}
)


add_subdirectory(tuya-iot-core-sdkk)

target_include_directories(tuyad PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${LINK_SDK_INCLUDE_PUBLIC_DIRS}
    "${TUYA_SDK_DIR}/utils"
    "${TUYA_SDK_DIR}/include"
    "${TUYA_SDK_DIR}/interface"
)

include("${TUYA_SDK_DIR}/tuyaFilePaths.cmake")

target_link_libraries(tuyad PRIVATE
    PkgConfig::CJSON
    link_core
    middleware_implementation
    platform_port
    utils_modules
    cjson
    m
)