set(CMAKE_CXX_STANDARD 11)

set(GTEST_INCLUDE_DIR ${gtest_SOURCE_DIR}/include)
set(MINOTAUR_INCLUDE_DIR ${CMAKE_SOURCE_DIR})

include_directories(${GTEST_INCLUDE_DIR})
include_directories(${MINOTAUR_INCLUDE_DIR})

file(GLOB TEST_FILES
        "*.h"
        "*.cpp"
        "./**/*.h"
        "./**/*.cpp")

add_executable(tests ${TEST_FILES})
target_link_libraries(tests gtest)
target_link_libraries(tests minotaur-lib)
add_dependencies(tests gtest)
add_dependencies(tests minotaur-lib)