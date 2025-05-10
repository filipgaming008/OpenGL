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

unsigned int renderState = 1;

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


    // Shaders init
    //-------------------------------------------------
    std::string shaders_location("../res/shaders/");
    std::string shader_name("shader");

    Shader *shader1 = new Shader(shaders_location + shader_name + ".vert", shaders_location + shader_name + ".frag");

    shader_name = "dartboard";
    Shader *dartBoardShader = new Shader(shaders_location + shader_name + ".vert", shaders_location + shader_name + ".frag");

    shader_name = "abstracttex";
    Shader *abstractTexShader = new Shader(shaders_location + shader_name + ".vert", shaders_location + shader_name + ".frag");


    // Vertex Data for a Plane with UV Coordinates
    //-------------------------------------------------
    std::vector<float> vertices = {
        // Positions           // UV Coordinates
        -0.5f,  0.5f,  0.0f,   0.0f, 1.0f, // Top-left
         0.5f,  0.5f,  0.0f,   1.0f, 1.0f, // Top-right
         0.5f, -0.5f,  0.0f,   1.0f, 0.0f, // Bottom-right
        -0.5f, -0.5f,  0.0f,   0.0f, 0.0f  // Bottom-left
    };

    std::vector<unsigned int> indices = {
        // Plane face
        0, 1, 2,
        0, 2, 3
    };


    // Buffers
    //-------------------------------------------------
    VertexLayout *layout = new VertexLayout();
    layout->Push<float>(3); // Position
    layout->Push<float>(2); // UV
    
    VertexBuffer *VBO = new VertexBuffer(vertices.data(), vertices.size() * sizeof(float));
    
    VertexArray *VAO = new VertexArray();
    VAO->AddBuffer(*VBO, *layout);
    
    IndexBuffer *IBO = new IndexBuffer(indices.data(), indices.size());
    

    // View, Model and Projection Matrices
    //-------------------------------------------------
    glm::mat4 model = glm::mat4(1.0f);
    double modelXOffset = 0.0f;
    
    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    
    
    // Textures init
    //-------------------------------------------------
    std::string textures_location("../res/textures/");
    std::string texture_name("wheel.png");
    
    Texture *tex1 = new Texture();
    tex1->loadTexture(textures_location + texture_name);

    texture_name = "tex1.jpg";
    Texture *aTex1 = new Texture();
    aTex1->loadTexture(textures_location + texture_name);

    texture_name = "tex2.png";
    Texture *aTex2 = new Texture();
    aTex2->loadTexture(textures_location + texture_name);


    // Rendering Loops
    while (glfwWindowShouldClose(Window) == false) {
        
        // Background Fill Color
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);

        // Process Input
        processInput(Window);
        
        if(renderState == 1){ 
            model = glm::translate(glm::mat4(1.0f), glm::vec3(modelXOffset, 0.0f, 0.0f)) 
            * glm::rotate(glm::mat4(1.0f), (float)-sin(glfwGetTime() * 0.5f) * 3.0f, glm::vec3(0.0f, 0.0f, 1.0f)) 
            * glm::translate(glm::mat4(1.0f), glm::vec3(-modelXOffset, 0.0f, 0.0f));
            
            modelXOffset = sin(glfwGetTime() * 0.5f) * 3.0f;
            model = glm::translate(model, glm::vec3(modelXOffset, 0.0f, 0.0f));
            
            shader1->use();
            tex1->bindTexture();
            shader1->setMVPMatrices(glm::value_ptr(model), glm::value_ptr(view), glm::value_ptr(projection));

        }else if(renderState == 2){
            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(2.4, 2.4f, 1.0f));

            dartBoardShader->use();
            dartBoardShader->setVec2f("resolution", SCR_WIDTH, SCR_HEIGHT);
            dartBoardShader->setMVPMatrices(glm::value_ptr(model), glm::value_ptr(view), glm::value_ptr(projection));

        }else if(renderState == 3){
            model = glm::mat4(1.0f);
            model = glm::scale(model, glm::vec3(10.0f, 10.0f, 1.0f));

            model = glm::rotate(model, (float)glfwGetTime() * glm::radians(25.0f), glm::vec3(0.0f, 0.0f, 1.0f));
            model = glm::scale(model, glm::vec3(sin(glfwGetTime()) * 0.5f + 1.0f, cos(glfwGetTime()) * 0.5f + 1.0f, 1.0f));

            aTex1->bindTexture(0);
            aTex2->bindTexture(1);
            
            abstractTexShader->use();
            abstractTexShader->setInt("texture1", 0);
            abstractTexShader->setInt("texture2", 1);
            abstractTexShader->setFloat("time", (float)glfwGetTime());
            abstractTexShader->setMVPMatrices(glm::value_ptr(model), glm::value_ptr(view), glm::value_ptr(projection));

        }

        // // Draw
        VAO->Bind();
        IBO->Bind();
        glDrawElements(GL_TRIANGLES, IBO->GetCount(), GL_UNSIGNED_INT, nullptr);

        // Flip Buffers and Draw
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }   


    // Cleanup
    delete tex1;
    delete aTex1;
    delete aTex2;
    delete shader1;
    delete dartBoardShader;
    delete abstractTexShader;
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
    if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS){
        renderState = 1;
    }
    if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS){
        renderState = 2;
    }
    if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS){
        renderState = 3;
    }
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