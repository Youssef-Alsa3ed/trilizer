#PROJECT INITIALIZATION
cmake_minimum_required(VERSION 4.0.0)
project(trilizer VERSION 0.1.0 DESCRIPTION "A simple OpenGL Renderer." LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#SOURCE FILES
file(GLOB SOURCES
    "src/main.cpp"
    "src/enginepch.h"
    "src/enginepch.cpp"
    "src/Core/Application.h"
    "src/Core/Application.cpp"
    "src/Core/Window.h"
    "src/Core/Window.cpp"
    "src/Events/Event.h"
    "src/Events/KeyEvent.h"
    "src/Events/MouseEvent.h"
    "src/Events/WindowEvent.h"
    "src/Logger/Logger.h"
    "src/Logger/Logger.cpp"
)
message("${SOURCES}")

if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.16 AND Athanor_BUILD_USING_PCH)
  target_precompile_headers(trilizer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/enginepch.h")
endif()

#EXECUTABLE
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(trilizer PUBLIC ${CMAKE_SOURCE_DIR}/src)

#DEPENDENCIES
add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/glfw)

target_link_libraries(trilizer PUBLIC GLAD)
target_link_libraries(trilizer PUBLIC glfw)

#SETTINGS
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(WIN32)
  add_definitions(-DTRILIZER_PLATFORM_WINDOWS=1)
endif()
if(UNIX)
  add_definitions(-DTRILIZER_PLATFORM_UNIX=1)
endif()