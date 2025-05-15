cmake_minimum_required(VERSION 3.31)

project(Simulator)
add_executable(${PROJECT_NAME}
        src/Shader.cpp
        src/Shader.h
        src/constants.h
        src/Ball.cpp
        src/Ball.h
        src/Camera.cpp
        src/Camera.h
        src/Solver.cpp
        src/Solver.h
)
target_compile_features(${PROJECT_NAME} PRIVATE cxx_std_17)

# Enable many good warnings
if (MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /WX /W3)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Werror -Wall -Wextra -Wpedantic -pedantic-errors -Wimplicit-fallthrough)
endif()

# Set the folder where the executable is created
set_target_properties(${PROJECT_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

# Prevents compiler-specific extensions to C++ because they might allow code to compile on your machine but not on other people's machine
set_target_properties(${PROJECT_NAME} PROPERTIES
        CXX_EXTENSIONS OFF)

# Add all the files
file(GLOB_RECURSE MY_SOURCES CONFIGURE_DEPENDS src/*)
target_sources(${PROJECT_NAME} PRIVATE ${MY_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE src)

# ---Add GLFW---
add_subdirectory(lib/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)
find_library(GLFWLIB glfw REQUIRED)
message(WARNING ${GLFWLIB})
#add_subdirectory(lib/stb)
#target_link_libraries(${PROJECT_NAME} PRIVATE stb)
# ---Add glad---
add_library(glad lib/glad/src/glad.c)
target_include_directories(glad PUBLIC lib/glad/include)
target_include_directories(${PROJECT_NAME}
        PRIVATE
        lib/glad/include
        lib/glfw/include
        lib/glm
)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)



# Copy the assets to the output folder (where the executable is created)
include("lib/CMakeUtils/files_and_folders.cmake")