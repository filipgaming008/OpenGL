// Local Headers
#include "OpenGL.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Buffers.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Model.hpp"

// System Headers
#include <glm/gtc/type_ptr.hpp>

// Standard Headers
#include <iostream>
#include <vector>

double lastTime = 0.0;
double deltaTime = 0.0;

Camera camera;
glm::vec2 cursorPos = glm::vec2(0.0f, 0.0f);

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
    
    glfwSetInputMode(Window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

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

    Shader lightShader(shaders_location + "light.vert", shaders_location + "light.frag");

    
    // Vertex Data for a Plane with Normals and UV Coordinates
    // std::vector<float> vertices = {
    //     // Positions           // Normals         // UV Coordinates
    //     -0.5f,  0.0f, -0.5f,   0.0f, 1.0f,  0.0f,   0.0f, 0.0f, // Bottom-left
    //      0.5f,  0.0f, -0.5f,   0.0f, 1.0f,  0.0f,   1.0f, 0.0f, // Bottom-right
    //      0.5f,  0.0f,  0.5f,   0.0f, 1.0f,  0.0f,   1.0f, 1.0f, // Top-right
    //     -0.5f,  0.0f,  0.5f,   0.0f, 1.0f,  0.0f,   0.0f, 1.0f  // Top-left
    // };

    // std::vector<unsigned int> indices = {
    //     // Plane face
    //     0, 1, 2,
    //     0, 2, 3
    // };


    // Buffers
    // VertexLayout *layout = new VertexLayout();
    // layout->Push<float>(3); // Position
    // layout->Push<float>(3); // Normal
    // layout->Push<float>(2); // UV
    
    // VertexBuffer *VBO = new VertexBuffer(vertices.data(), (unsigned int)vertices.size() * sizeof(float));
    
    // VertexArray *VAO = new VertexArray();
    // VAO->AddBuffer(*VBO, *layout);

    // IndexBuffer *IBO = new IndexBuffer(indices.data(), (unsigned int)indices.size());

    // Light Position
    glm::dvec3 lightPos = glm::dvec3(0.0f, 0.0f, 0.0f); 

    Model *model1 = new Model(SPHERE);
    Model *model2 = new Model(CUBE);
    Model *lightModel = new Model(SPHERE); 
    
    // Rendering Loop
    while (glfwWindowShouldClose(Window) == false) {
        
        DeltaTime();
        // Process Input
        processInput(Window);
        cursor_position_callback(Window, cursorPos.x, cursorPos.y);
        
        camera.MouseUpdate(cursorPos);
        camera.Update();
        
        // Background Fill Color
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        
        // Draw
        //-------------------------------------------------------------
        lightPos.x = 5.0 * sin(glfwGetTime() * 0.1f);
        lightPos.y = 5.0 * cos(glfwGetTime() * 0.1f);
        
        lightModel->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(lightPos.x, lightPos.y, lightPos.z)));
        lightModel->SetModelMatrix(glm::scale(lightModel->GetModelMatrix(), glm::vec3(0.3f)));
        lightModel->Draw(lightShader, camera);
        
        shader1.use();
        shader1.setVec3f("lightPos", (float)lightPos.x, (float)lightPos.y, (float)lightPos.z);
        shader1.setFloat("Time", (float)getDeltaTime());
        shader1.setVec3f("viewDir", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        
        glDisable(GL_BLEND);
        shader1.setVec4f("objectColor", 0.8f, 0.1f, 0.1f, 1.0f);
        model2->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 5.0f)));
        model2->Draw(shader1, camera);
        
        glEnable(GL_BLEND);
        shader1.use();
        shader1.setVec4f("objectColor", 0.1f, 0.8f, 0.3f, 0.5f);
        model1->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 2.0f)));
        model1->Draw(shader1, camera);
        
        // Flip Buffers and Draw
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }   
    
    // Cleanup
    delete model1;
    delete model2;
    delete lightModel;
    // delete layout;
    // delete VAO;
    // delete VBO;
    // delete IBO;
    
    glfwTerminate();
    return EXIT_SUCCESS;
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos) {
    glfwGetCursorPos(window, &xpos, &ypos);

    cursorPos.x = (float)xpos;
    cursorPos.y = (float)ypos;
}

void processInput(GLFWwindow * window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
        camera.PositionUpdate(FORWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
        camera.PositionUpdate(BACKWARD);
    }
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
        camera.PositionUpdate(LEFT);
    }
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
        camera.PositionUpdate(RIGHT);
    }
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
        camera.PositionUpdate(UP);
    }
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS){
        camera.PositionUpdate(DOWN);
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT_BRACKET) == GLFW_PRESS){
        camera.Tilt(90.0f);
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT_BRACKET) == GLFW_PRESS){
        camera.Tilt(-90.0f);
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    (void) window;
    glViewport(0, 0, width, height);
}

void DeltaTime() {
    double currentTime = glfwGetTime();
    deltaTime = currentTime - lastTime;
    lastTime = currentTime;
}

double getDeltaTime() {
    return deltaTime;
}