set(PYTHON_SCRIPT_DIR python_scripts)
macro(add_python_target target)
    foreach (file ${target})
        get_filename_component(file_name ${file} NAME)
        configure_file(${file} ${PYTHON_SCRIPT_DIR}/${file_name} COPYONLY)
    endforeach ()
endmacro()

if (APPLE)
    # Apple hard sets these variables for system Python
    unset(PYTHON_LIBRARY CACHE)
    unset(PYTHON_INCLUDE_DIR CACHE)
    unset(PYTHON_LIBRARIES CACHE)
endif()

cmake_minimum_required(VERSION 3.1.0)

project(minotaur-cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} -std=c++11")

find_package(PythonLibs 3.4 REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

if (Qt5_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif()

## Third Party files
set(THIRD_PARTY_COMMON
        third-party/qextserialport.h
        third-party/qextserialport_p.h
        third-party/qextserialport_global.h
        third-party/qextserialenumerator.h
        third-party/qextserialenumerator_p.h
        third-party/qextserialport.cpp
        third-party/qextserialenumerator.cpp)
if (WIN32)
    set(THIRD_PARTY
            third-party/qextserialenumerator_win.cpp
            third-party/qextserialport_win.cpp)
endif()
if (APPLE)
    set(THIRD_PARTY
            third-party/qextserialenumerator_unix.cpp
            third-party/qextserialport_unix.cpp)
endif()
if (UNIX)
    set(THIRD_PARTY
            third-party/qextserialenumerator_unix.cpp
            third-party/qextserialport_unix.cpp)
endif()

file(GLOB_RECURSE SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/code/*.cpp
        ${PROJECT_SOURCE_DIR}/code/controller/*.cpp
        ${PROJECT_SOURCE_DIR}/code/graphics/*.cpp
        ${PROJECT_SOURCE_DIR}/code/gui/*.cpp
        ${PROJECT_SOURCE_DIR}/code/interpreter/*.cpp
        ${PROJECT_SOURCE_DIR}/code/utility/*.cpp)

file(GLOB_RECURSE HEADER_FILES
        ${PROJECT_SOURCE_DIR}/code/controller/*.h
        ${PROJECT_SOURCE_DIR}/code/graphics/*.h
        ${PROJECT_SOURCE_DIR}/code/gui/*.h
        ${PROJECT_SOURCE_DIR}/code/interpreter/*.h
        ${PROJECT_SOURCE_DIR}/code/utility/*.h)

file(GLOB_RECURSE FORM_FILES
        ${PROJECT_SOURCE_DIR}/code/gui/*.ui)

add_python_target(code/scripts/__init__.py)
add_python_target(code/scripts/robot.py)
add_python_target(code/scripts/testScript.py)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(third-party)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

get_target_property(QtCore_location Qt5::Core LOCATION)
add_executable(minotaur-cpp ${SOURCE_FILES} ${HEADER_FILES} ${FORM_FILES} ${THIRD_PARTY} ${THIRD_PARTY_COMMON})

target_link_libraries(minotaur-cpp Qt5::Widgets Qt5::Core Qt5::Gui)
target_link_libraries(minotaur-cpp ${PYTHON_LIBRARIES})
target_include_directories(minotaur-cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(minotaur-cpp PRIVATE PYTHON_SCRIPT_DIR="${PYTHON_SCRIPT_DIR}")

if (WIN32)
    target_link_libraries(minotaur-cpp advapi32 user32 setupapi)
endif ()
