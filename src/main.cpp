// Local Headers
#include "OpenGL.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Buffers.hpp"
#include "Texture.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <iostream>
#include <vector>

// Function Prototypes
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow * window);
double getDeltaTime();


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
    int version = gladLoadGL();
    if (version == 0) {
        std::cerr << "Failed to initialize OpenGL context" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;


    // Shaders init
    std::string shaders_location("../res/shaders/");
    std::string shader_name("shader");

    Shader shader1(shaders_location + shader_name + ".vert", shaders_location + shader_name + ".frag");


    // Vertex Data for a Plane with Normals and UV Coordinates
    std::vector<float> vertices = {
        // Positions           // Normals         // UV Coordinates
        -0.5f,  0.0f, -0.5f,   0.0f, 1.0f,  0.0f,   0.0f, 0.0f, // Bottom-left
         0.5f,  0.0f, -0.5f,   0.0f, 1.0f,  0.0f,   1.0f, 0.0f, // Bottom-right
         0.5f,  0.0f,  0.5f,   0.0f, 1.0f,  0.0f,   1.0f, 1.0f, // Top-right
        -0.5f,  0.0f,  0.5f,   0.0f, 1.0f,  0.0f,   0.0f, 1.0f  // Top-left
    };

    std::vector<unsigned int> indices = {
        // Plane face
        0, 1, 2,
        0, 2, 3
    };


    // Buffers
    VertexLayout *layout = new VertexLayout();
    layout->Push<float>(3); // Position
    layout->Push<float>(3); // Normal
    layout->Push<float>(2); // UV
    
    VertexBuffer *VBO = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
    
    VertexArray *VAO = new VertexArray();
    VAO->AddBuffer(*VBO, *layout);

    IndexBuffer *IBO = new IndexBuffer(indices.data(), indices.size());


    // View, Model and Projection Matrices
    //-------------------------------------------------
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    
    glm::vec3 lightPos;


    // Textures init
    std::string textures_location("../res/textures/");
    std::string texture_name("wheel.png");

    Texture *tex1 = new Texture();
    tex1->loadTexture(textures_location + texture_name);

    TextureParameters texParams;
    texParams.addParameter<GLint>(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    texParams.addParameter<GLint>(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    texParams.addParameter<GLint>(GL_TEXTURE_WRAP_S, GL_REPEAT);
    texParams.addParameter<GLint>(GL_TEXTURE_WRAP_T, GL_REPEAT);

    tex1->addTextureParameters(texParams);

    // Rendering Loop
    while (glfwWindowShouldClose(Window) == false) {
        
        // Process Input
        processInput(Window);
        
        // Background Fill Color
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        
        // Draw
        model = glm::rotate(model, (float)getDeltaTime() * glm::radians(25.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        
        lightPos.x = 5.0f * sin(glfwGetTime() * 0.5f);
        lightPos.z = 5.0f * cos(glfwGetTime() * 0.5f);
        
        shader1.use();
        tex1->bindTexture();
        shader1.setMVPMatrices(glm::value_ptr(model), glm::value_ptr(view), glm::value_ptr(projection));
        shader1.setVec3f("lightPos", lightPos.x, 2.0f, lightPos.z);
        shader1.setFloat("Time", (float)glfwGetTime());
        
        VAO->Bind();
        IBO->Bind();
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        

        // Flip Buffers and Draw
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }   

    // Cleanup
    delete tex1;
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

double getDeltaTime() {
    static double lastTime = 0.0;
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - lastTime;
    lastTime = currentTime;
    return deltaTime;
}