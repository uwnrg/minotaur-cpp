set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(Util)
include(Python)

if (APPLE)
    # Apple hard sets these variables for system Python
    unset(PYTHON_LIBRARY CACHE)
    unset(PYTHON_INCLUDE_DIR CACHE)
    unset(PYTHON_LIBRARIES CACHE)
endif ()

cmake_minimum_required(VERSION 3.1.0)

project(minotaur-cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} -std=c++11")
set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(PythonLibs 3.4 REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5Multimedia REQUIRED)
find_package(Qt5MultimediaWidgets REQUIRED)
find_package(Threads REQUIRED)

if (Qt5_POSITION_INDEPENDENT_CODE)
    set(CMAKE_POSITION_INDEPENDENT_CODE ON)
endif ()

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
endif ()
if (APPLE)
    set(THIRD_PARTY
            third-party/qextserialenumerator_unix.cpp
            third-party/qextserialport_unix.cpp)
endif ()
if (UNIX)
    set(THIRD_PARTY
            third-party/qextserialenumerator_unix.cpp
            third-party/qextserialport_unix.cpp)
endif ()

file(GLOB_RECURSE SOURCE_FILES
        ${PROJECT_SOURCE_DIR}/code/*.cpp
        ${PROJECT_SOURCE_DIR}/code/**/*.cpp)

file(GLOB_RECURSE HEADER_FILES
        ${PROJECT_SOURCE_DIR}/code/*.h
        ${PROJECT_SOURCE_DIR}/code/**/*.h)

file(GLOB_RECURSE FORM_FILES
        ${PROJECT_SOURCE_DIR}/code/gui/*.ui)

add_python_target(code/scripts/__init__.py)
add_python_target(code/scripts/robot.py)

# Include library directories
include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(third-party)

# AutoMOC and AutoUIC for Qt compiler
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

get_target_property(QtCore_location Qt5::Core LOCATION)
add_executable(minotaur-cpp ${SOURCE_FILES} ${HEADER_FILES} ${FORM_FILES} ${THIRD_PARTY} ${THIRD_PARTY_COMMON})

# Uncomment this line to compile in debug mode
option(NO_DEBUG "Build minotaur without debugging output" OFF)
if (NO_DEBUG)
    log("Building without debugging")
    target_compile_definitions(minotaur-cpp PRIVATE QT_NO_DEBUG)
    target_compile_definitions(minotaur-cpp PRIVATE NDEBUG)
else ()
    log("Building with debugging on")
endif ()

# Attempt to find GOTURN files
include(Goturn)
if (${GOTURN_FILES_FOUND})
    target_compile_definitions(minotaur-cpp PRIVATE GOTURN_FOUND)
endif ()

# Find and link OpenCV 3 package
find_package(OpenCV 3 REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})
target_link_libraries(minotaur-cpp ${OpenCV_LIBS})

# Link Qt, Python, and Thread libraries
target_link_libraries(minotaur-cpp Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Multimedia Qt5::MultimediaWidgets)
target_link_libraries(minotaur-cpp ${PYTHON_LIBRARIES})
target_link_libraries(minotaur-cpp "${CMAKE_THREAD_LIBS_INIT}")

# Include current directory to find Qt MOC files
target_include_directories(minotaur-cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
# Add definition for location of python scripts
target_compile_definitions(minotaur-cpp PRIVATE PYTHON_SCRIPT_DIR="${PYTHON_SCRIPT_DIR}")

# Windows requires separate libraries
if (WIN32)
    target_link_libraries(minotaur-cpp advapi32 user32 setupapi)
endif ()
