
#PROJECT INITIALIZATION
cmake_minimum_required(VERSION 4.0.0)
project(trilizer VERSION 0.1.0 DESCRIPTION "A simple OpenGL Renderer." LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
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
    "src/Renderer/Renderer.h"
    "src/Renderer/Shader.h"
    "src/Renderer/Shader.cpp"
    "src/Renderer/VertexBuffer.h"
    "src/Renderer/VertexBuffer.cpp"
    "src/Renderer/VertexArray.h"
    "src/Renderer/VertexArray.cpp"
    "src/Renderer/VertexBufferLayout.h"
    "src/Renderer/IndexBuffer.h"
    "src/Renderer/IndexBuffer.cpp"
    "src/Renderer/Texture.h"
    "src/Renderer/Texture.cpp"
    "src/Renderer/stb_image.cpp"
    "src/Renderer/stb_image.h"
    "src/Renderer/TriMesh.h"
    "src/Renderer/TriMesh.cpp"
      "src/Renderer/Material.h"
      "src/Renderer/Material.cpp"
      "src/Renderer/Model.h"
      "src/Renderer/Model.cpp"
    "src/Core/Input.h"
    "src/Core/Input.cpp"
)
message("${SOURCES}")


if(CMAKE_VERSION VERSION_GREATER_EQUAL 3.16 AND Athanor_BUILD_USING_PCH)
  target_precompile_headers(trilizer PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/src/enginepch.h")
endif()

#EXECUTABLE
add_executable(${PROJECT_NAME} ${SOURCES})

target_include_directories(trilizer PUBLIC ${CMAKE_SOURCE_DIR}/src)
#SETTINGS
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
if(WIN32)
  add_definitions(-DTRILIZER_PLATFORM_WINDOWS=1)
endif()
if(UNIX)
  add_definitions(-DTRILIZER_PLATFORM_UNIX=1)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  add_definitions(-DDEBUG)
endif()

# ---- Assimp configuration ----
set(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_OBJ_IMPORTER  ON CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_GLTF_IMPORTER ON CACHE BOOL "" FORCE)

set(ASSIMP_BUILD_ASSIMP_TOOLS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS       OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_SAMPLES     OFF CACHE BOOL "" FORCE)

# Optional: no exporters
set(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT OFF CACHE BOOL "" FORCE)

#DEPENDENCIES
add_subdirectory(dependencies/glad)
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/glm)
add_subdirectory(dependencies/assimp)
target_include_directories(trilizer PUBLIC dependencies)

target_link_libraries(trilizer PUBLIC GLAD)
target_link_libraries(trilizer PUBLIC glfw)
target_link_libraries(trilizer PUBLIC glm)
target_link_libraries(trilizer PUBLIC assimp)
