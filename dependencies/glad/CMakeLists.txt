project(GLAD LANGUAGES C)

add_library(GLAD STATIC
    src/glad.c
)

target_include_directories(GLAD PUBLIC
    include/
)

find_package(OpenGL REQUIRED)
target_link_libraries(GLAD PUBLIC OpenGL::GL)