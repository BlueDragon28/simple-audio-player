cmake_minimum_required(VERSION 3.16.0)
project(simple-audio-player VERSION 0.1 LANGUAGES CXX)

# Set C++ standard to C++ 17
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6
find_package(Qt6 COMPONENTS Core Gui Quick QuickControls2 REQUIRED)

# Find simple-audio-library
find_package(PkgConfig REQUIRED)
pkg_check_modules(SAL REQUIRED simple-audio-library_static)

# Search for every headers in sources file.
file(GLOB HEADERS include/*.h)
file(GLOB SOURCES src/*.cpp)

set(RESOURCES "simple-audio-player.qrc")

# Include the include directory
include_directories(include/)

# Add library directory path.
link_directories(${SAL_LIBRARY_DIRS})

# Create the executable.
qt_add_executable(${PROJECT_NAME} ${HEADERS} ${SOURCES} ${RESOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE 
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    ${SAL_LIBRARIES})
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

qt_add_qml_module(${PROJECT_NAME} URI "SimpleAudioPlayer" VERSION 1.0)