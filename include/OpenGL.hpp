#pragma once

// System Headers
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

// Local Headers
#include <string>

// Define Some Constants
const std::string name = "OpenGL";
const unsigned int SCR_WIDTH = 1600;
const unsigned int SCR_HEIGHT = 900;

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow * window);
void DeltaTime();
double getDeltaTime();