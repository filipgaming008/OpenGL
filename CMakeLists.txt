cmake_minimum_required(VERSION 3.30)
project(OpenGL)


# FetchContent is used to download and manage external dependencies
include(FetchContent)
set(FETCHCONTENT_BASE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(FETCHCONTENT_QUIET FALSE)


# Cmake C++ standard
set(CMAKE_CXX_STANDARD 23) # Use C++23
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) # Disable compiler-specific extensions


# Set default build type to Debug if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()


# Set compiler flags
if(MSVC)
    # MSVC specific flags
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4 /MDd /Zi /Od") # Debug flags
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W3 /MD /O2")  # Release flags
else()
    # GCC/Clang-specific flags
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall -Wextra -Wpedantic") # Debug flags
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")                # Release flags
endif()

#----------------------------------------------------------------------
# Add external dependencies

# GLFW
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.4
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

# GLAD
FetchContent_Declare(
    glad
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG v2.0.8
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
    SOURCE_SUBDIR cmake
)

# GLM
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 1.0.1
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

# stb_image
FetchContent_Declare(
    stb
    GIT_REPOSITORY https://github.com/nothings/stb.git
    GIT_TAG master
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)

# assimp 
FetchContent_Declare(
    assimp
    GIT_REPOSITORY https://github.com/assimp/assimp.git
    GIT_TAG v5.4.3
    GIT_SHALLOW TRUE
    GIT_PROGRESS TRUE
)
# Set assimp options
set(BUILD_SHARED_LIBS OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INJECT_DEBUG_POSTFIX OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_WARNINGS_AS_ERRORS OFF CACHE BOOL "" FORCE)
SET(ASSIMP_NO_WARNINGS TRUE CACHE BOOL "" FORCE)


# Fetch all needed dependencies
FetchContent_MakeAvailable(glfw glad glm stb assimp)

# Set GLAD to use OpenGL 4.6 Core Profile
glad_add_library(glad_gl_core_46 STATIC REPRODUCIBLE LOADER API gl:core=4.6)

#-----------------------------------------------------------------------


# Shaders/Textures paths
set(SHADERS_RELATIVE_SRC_PATH "res/shaders")
set(TEXTURES_RELATIVE_SRC_PATH "res/textures")

# Include directories
include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glfw-src/include
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/glm-src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/stb-src
    ${CMAKE_CURRENT_SOURCE_DIR}/vendor/assimp-src/include
)

# Add source, header, shader and texture files
file(GLOB PROJECT_HEADERS include/*.hpp)
file(GLOB PROJECT_SOURCES src/*.cpp)
file(GLOB PROJECT_SHADERS ${SHADERS_RELATIVE_SRC_PATH}/*.comp
${SHADERS_RELATIVE_SRC_PATH}/*.frag
${SHADERS_RELATIVE_SRC_PATH}/*.geom
${SHADERS_RELATIVE_SRC_PATH}/*.vert
${SHADERS_RELATIVE_SRC_PATH}/*.vs
${SHADERS_RELATIVE_SRC_PATH}/*.fs
${SHADERS_RELATIVE_SRC_PATH}/*.glsl
)
file(GLOB PROJECT_TEXTURES ${TEXTURES_RELATIVE_SRC_PATH}/*.png
${TEXTURES_RELATIVE_SRC_PATH}/*.jpg
)

# Add source groups for better organization in IDEs
source_group("include" FILES ${PROJECT_HEADERS})
source_group("shaders" FILES ${PROJECT_SHADERS})
source_group("textures" FILES ${PROJECT_TEXTURES})
source_group("src" FILES ${PROJECT_SOURCES})

# Dont include OpenGL headers
# This is needed to avoid conflicts with glad
# and to use the OpenGL headers provided by glad
add_definitions(-DGLFW_INCLUDE_NONE
-DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

# Add executable target
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
${PROJECT_SHADERS} ${PROJECT_TEXTURES}
)

# Link libraries
target_link_libraries(${PROJECT_NAME} 
glad_gl_core_46
glfw
glm
assimp
)

# Copy shaders and textures to the output directory
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
COMMAND ${CMAKE_COMMAND} -E copy_directory
${CMAKE_SOURCE_DIR}/res
$<TARGET_FILE_DIR:${PROJECT_NAME}>/../res)

# Set output directories for the executable and libraries
set_target_properties(${PROJECT_NAME}
PROPERTIES
ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/lib"
LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/lib"
RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${PROJECT_NAME}/bin"
)