cmake_minimum_required(VERSION 3.27)
project(RegLab_test_task)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_PREFIX_PATH "/usr/lib/x86_64-linux-gnu/cmake/Qt5")

set(SRC_PATH "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(INCLUDE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(RESOURCE_PATH "resources.qrc")

include_directories(${INCLUDE_PATH})

file(GLOB_RECURSE CPP_SOURCES "${SRC_PATH}/*.cpp")
file(GLOB_RECURSE H_SOURCES "${INCLUDE_PATH}/*.h")

find_package(Qt5 REQUIRED COMPONENTS
        Core
        Widgets
)

qt_add_resources(RESOURCES ${RESOURCE_PATH})

add_executable(RegLab_test_task
        ${CPP_SOURCES}
        ${H_SOURCES}
        ${RESOURCES}
)

target_link_libraries(RegLab_test_task
        Qt5::Core
        Qt5::Widgets
)
