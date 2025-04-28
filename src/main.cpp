// Local Headers
#include "OpenGL.hpp"
#include "Shader.hpp"

#include "Renderer.hpp"
#include "Buffers.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <iostream>
#include <vector>
#include <string>

const std::string name = "Shader Testing Stuff";

// Function Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow * window);

int main(){

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    
    // Create a Window
    GLFWwindow * Window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, &name[0], nullptr, nullptr);
    if (Window == nullptr) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return EXIT_FAILURE;
    }
    glfwMakeContextCurrent(Window);

    // Load OpenGL Functions
    gladLoadGL();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Shader's init
    std::string shaders_location("../res/shaders/");
    std::string shader_name("shader");

    Shader shader1(shaders_location + shader_name + ".vert", shaders_location + shader_name + ".frag");

    // Vertex Data for a Cube
    std::vector<float> vertices = {
        // Positions         
        -0.5f, -0.5f, -0.5f, // Bottom-left-back
         0.5f, -0.5f, -0.5f, // Bottom-right-back
         0.5f,  0.5f, -0.5f, // Top-right-back
        -0.5f,  0.5f, -0.5f, // Top-left-back
        -0.5f, -0.5f,  0.5f, // Bottom-left-front
         0.5f, -0.5f,  0.5f, // Bottom-right-front
         0.5f,  0.5f,  0.5f, // Top-right-front
        -0.5f,  0.5f,  0.5f  // Top-left-front
    };

    std::vector<unsigned int> indices = {
        // Back face
        0, 1, 2,
        0, 2, 3,
        // Front face
        4, 5, 6,
        4, 6, 7,
        // Left face
        0, 4, 7,
        0, 7, 3,
        // Right face
        1, 5, 6,
        1, 6, 2,
        // Bottom face
        0, 1, 5,
        0, 5, 4,
        // Top face
        3, 2, 6,
        3, 6, 7
    };

    // Buffers
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    VertexBuffer *VBO = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);

    IndexBuffer *IBO = new IndexBuffer(indices.data(), indices.size());

    glm::mat4 model = glm::mat4(1.0f);
    // model = glm::rotate(model, glm::radians(-45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    
    // Rendering Loop
    while (glfwWindowShouldClose(Window) == false) {
        
        // Process Input
        processInput(Window);
        
        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        
        model = glm::rotate(model, (float)0.016 * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        
        shader1.use();
        shader1.setMat4f("model", glm::value_ptr(model));
        shader1.setMat4f("view", glm::value_ptr(view));
        shader1.setMat4f("projection", glm::value_ptr(projection));
        glBindVertexArray(VAO);
        IBO->Bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        
        
        // Flip Buffers and Draw
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }   


    glDeleteVertexArrays(1, &VAO);
    delete VBO;
    delete IBO;
    
    glfwTerminate();
    return EXIT_SUCCESS;
}

void processInput(GLFWwindow * window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}