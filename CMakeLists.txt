cmake_minimum_required(VERSION 3.16...3.30)
project(DSA_Visualizer_Studio VERSION 1.0.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4 /O2)
else()
    add_compile_options(-Wall -Wextra -O2)
endif()

# Raylib settings
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
set(CUSTOMIZE_BUILD OFF CACHE BOOL "" FORCE)

add_subdirectory(third_party/raylib)

# 1. Core Data Structures & Algorithms Library (Zero STL containers)
set(DSA_CORE_SOURCES
    src/dsa/Stack.cpp
    src/dsa/Queue.cpp
    src/dsa/SinglyLinkedList.cpp
    src/dsa/BinarySearchTree.cpp
    src/algorithms/SortingAlgorithms.cpp
)

add_library(dsa_core STATIC ${DSA_CORE_SOURCES})
target_include_directories(dsa_core PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# 2. Main Desktop GUI Application (raylib)
set(GUI_SOURCES
    src/gui/UICommon.cpp
    src/gui/UIComponents.cpp
    src/gui/DashboardView.cpp
    src/gui/StackView.cpp
    src/gui/QueueView.cpp
    src/gui/LinkedListView.cpp
    src/gui/BSTView.cpp
    src/gui/SortingView.cpp
    src/gui/Application.cpp
    src/main.cpp
)

add_executable(dsa_studio ${GUI_SOURCES})
target_include_directories(dsa_studio PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(dsa_studio PRIVATE
    dsa_core
    raylib
)

if(WIN32)
    target_link_libraries(dsa_studio PRIVATE
        opengl32
        gdi32
        winmm
    )
endif()

# 3. Unit Test Executable
add_executable(test_dsa tests/test_dsa.cpp)
target_include_directories(test_dsa PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
target_link_libraries(test_dsa PRIVATE
    dsa_core
)

enable_testing()
add_test(NAME CoreDSATests COMMAND test_dsa)
