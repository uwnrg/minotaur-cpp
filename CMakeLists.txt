set(PYTHON_SCRIPT_DIR python-scripts)
macro(add_python_target target)
    foreach (file ${target})
        get_filename_component(file_name ${file} NAME)
        configure_file(${file} ${PYTHON_SCRIPT_DIR}/${file_name} COPYONLY)
    endforeach ()
endmacro()

cmake_minimum_required(VERSION 3.1.0)

project(minotaur-cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_PREFIX_PATH X:\\Qt\\5.8\\mingw53_32)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${Qt5Widgets_EXECUTABLE_COMPILE_FLAGS} -std=c++11")

find_package(PythonLibs REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)

set(SOURCE_FILES code/main.cpp

        code/controller/actuator.cpp
        code/controller/controller.cpp
        code/controller/simulator.cpp

        code/graphics/robotgraphicsitem.cpp
        code/graphics/simulatorscene.cpp

        code/gui/actuatorsetup.cpp
        code/gui/mainwindow.cpp
        code/gui/pythoninterpreter.cpp
        code/gui/simulatorwindow.cpp

        code/utility/clock_time.cpp
        code/utility/logger.cpp
        code/utility/vector2i.cpp

        third-party/qextserialport.cpp
        third-party/qextserialport_win.cpp
        third-party/qextserialenumerator.cpp
        third-party/qextserialenumerator_win.cpp)

set(HEADER_FILES

        code/controller/actuator.h
        code/controller/controller.h
        code/controller/simulator.h

        code/graphics/robotgraphicsitem.h
        code/graphics/simulatorscene.h

        code/gui/actuatorsetup.h
        code/gui/mainwindow.h
        code/gui/pythoninterpreter.h
        code/gui/simulatorwindow.h

        code/utility/clock_time.h
        code/utility/logger.h
        code/utility/vector2i.h

        third-party/qextserialport.h
        third-party/qextserialport_p.h
        third-party/qextserialport_global.h
        third-party/qextserialenumerator.h
        third-party/qextserialenumerator_p.h)

set(FORM_FILES

        code/gui/actuatorsetup.ui
        code/gui/mainwindow.ui
        code/gui/pythoninterpreter.ui
        code/gui/simulatorwindow.ui code/interpreter/PythonEngine.cpp code/interpreter/PythonEngine.h code/interpreter/EmbeddedController.cpp code/interpreter/EmbeddedController.h)

add_python_target(code/scripts/robot.py)

include_directories(${Qt5Widgets_INCLUDE_DIRS})
include_directories(${PYTHON_INCLUDE_DIRS})
include_directories(third-party)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

get_target_property(QtCore_location Qt5::Core LOCATION)
add_executable(minotaur-cpp ${SOURCE_FILES} ${HEADER_FILES} ${FORM_FILES})

target_link_libraries(minotaur-cpp Qt5::Widgets Qt5::Core Qt5::Gui)
target_link_libraries(minotaur-cpp ${PYTHON_LIBRARIES})
target_include_directories(minotaur-cpp PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(minotaur-cpp PRIVATE PYTHON_SCRIPT_DIR="${PYTHON_SCRIPT_DIR}")

if (WIN32)
    target_link_libraries(minotaur-cpp advapi32 user32 setupapi)
endif ()