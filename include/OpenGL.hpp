#pragma once

// System Headers
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <string>

// Reference: https://github.com/nothings/stb/blob/master/stb_image.h#L4
// To use stb_image, add this in *one* C++ source file.
//     #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>

// Define Some Constants
static double lastTime = 0.0;
static double deltaTime = 0.0;
const std::string name = "OpenGL";
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;
