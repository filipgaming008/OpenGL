cmake_minimum_required(VERSION 3.30)
project(OpenGL)

# C++ standard
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set default build type
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type (Debug, Release, RelWithDebInfo, MinSizeRel)" FORCE)
endif()

# Compiler flags
if(MSVC)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /W4 /MDd /Zi /Od")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /W3 /MD /O2")
else()
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -g -O0 -Wall -Wextra -Wpedantic")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG") 
endif()

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)

add_subdirectory(vendor/glfw)

set(SHADERS_RELATIVE_SRC_PATH "res/shaders")
set(TEXTURES_RELATIVE_SRC_PATH "res/textures")

include_directories(include/
                    vendor/glad/include/
                    vendor/glfw/include/
                    vendor/glm/
                    vendor/stb/)

file(GLOB VENDORS_SOURCES vendor/glad/src/glad.c)
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

file(GLOB PROJECT_CONFIGS CMakeLists.txt
                          Readme.md
                         .gitattributes
                         .gitignore
                         .gitmodules)

source_group("include" FILES ${PROJECT_HEADERS})
source_group("shaders" FILES ${PROJECT_SHADERS})
source_group("textures" FILES ${PROJECT_TEXTURES})
source_group("src" FILES ${PROJECT_SOURCES})
source_group("vendors" FILES ${VENDORS_SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE
                -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                               ${PROJECT_SHADERS} ${PROJECT_TEXTURES} ${PROJECT_CONFIGS}
                               ${VENDORS_SOURCES})
target_link_libraries(${PROJECT_NAME}
		      glfw
                      ${GLFW_LIBRARIES} ${GLAD_LIBRARIES}
		      )


#set (source "${CMAKE_SOURCE_DIR}/res")
#set (destination "${CMAKE_CURRENT_BINARY_DIR}/res")

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/res
        $<TARGET_FILE_DIR:${PROJECT_NAME}>/../res)


set_target_properties(${PROJECT_NAME}
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/bin/src"
)