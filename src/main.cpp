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


    // Vertex Data for a Cube with Normals
    std::vector<float> vertices = {
        // Positions           // Normals
        -0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f, // Bottom-left-back
         0.5f, -0.5f, -0.5f,   0.0f,  0.0f, -1.0f, // Bottom-right-back
         0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f, // Top-right-back
        -0.5f,  0.5f, -0.5f,   0.0f,  0.0f, -1.0f, // Top-left-back
        -0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f, // Bottom-left-front
         0.5f, -0.5f,  0.5f,   0.0f,  0.0f,  1.0f, // Bottom-right-front
         0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f, // Top-right-front
        -0.5f,  0.5f,  0.5f,   0.0f,  0.0f,  1.0f, // Top-left-front
        -0.5f, -0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, // Bottom-left-back
        -0.5f,  0.5f, -0.5f,  -1.0f,  0.0f,  0.0f, // Top-left-back
        -0.5f,  0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, // Top-left-front
        -0.5f, -0.5f,  0.5f,  -1.0f,  0.0f,  0.0f, // Bottom-left-front
         0.5f, -0.5f, -0.5f,   1.0f,  0.0f,  0.0f, // Bottom-right-back
         0.5f,  0.5f, -0.5f,   1.0f,  0.0f,  0.0f, // Top-right-back
         0.5f,  0.5f,  0.5f,   1.0f,  0.0f,  0.0f, // Top-right-front
         0.5f, -0.5f,  0.5f,   1.0f,  0.0f,  0.0f, // Bottom-right-front
        -0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f, // Bottom-left-back
         0.5f, -0.5f, -0.5f,   0.0f, -1.0f,  0.0f, // Bottom-right-back
         0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f, // Bottom-right-front
        -0.5f, -0.5f,  0.5f,   0.0f, -1.0f,  0.0f, // Bottom-left-front
        -0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f, // Top-left-back
         0.5f,  0.5f, -0.5f,   0.0f,  1.0f,  0.0f, // Top-right-back
         0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f, // Top-right-front
        -0.5f,  0.5f,  0.5f,   0.0f,  1.0f,  0.0f  // Top-left-front
    };

    std::vector<unsigned int> indices = {
        // Back face
        0, 1, 2,
        0, 2, 3,
        // Front face
        4, 5, 6,
        4, 6, 7,
        // Left face
        8, 9, 10,
        8, 10, 11,
        // Right face
        12, 13, 14,
        12, 14, 15,
        // Bottom face
        16, 17, 18,
        16, 18, 19,
        // Top face
        20, 21, 22,
        20, 22, 23
    };


    // Buffers
    VertexLayout *layout = new VertexLayout();
    layout->Push<float>(3); // Position
    layout->Push<float>(3); // Normal
    
    VertexBuffer *VBO = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
    
    VertexArray *VAO = new VertexArray();
    VAO->AddBuffer(*VBO, *layout);

    IndexBuffer *IBO = new IndexBuffer(indices.data(), indices.size());


    // View, Model and Projection Matrices
    //-------------------------------------------------
    glm::mat4 model = glm::mat4(1.0f);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    
    glm::vec3 lightPos(2.0f, 5.0f, 0.0f);


    // Rendering Loop
    while (glfwWindowShouldClose(Window) == false) {
        
        // Process Input
        processInput(Window);
        
        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
    
        // Draw
        model = glm::rotate(model, (float)0.016 * glm::radians(50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        
        lightPos.x = 5.0f * sin(glfwGetTime() * 0.5f);
        lightPos.y = 5.0f * cos(glfwGetTime() * 0.5f);

        shader1.use();
        shader1.setMat4f("model", glm::value_ptr(model));
        shader1.setMat4f("view", glm::value_ptr(view));
        shader1.setMat4f("projection", glm::value_ptr(projection));
        shader1.setVec3f("lightPos", lightPos.x, lightPos.y, lightPos.z);
        shader1.setFloat("Time", (float)glfwGetTime());
        
        VAO->Bind();
        IBO->Bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        
        
        // Flip Buffers and Draw
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }   

    // Cleanup
    delete layout;
    delete VAO;
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