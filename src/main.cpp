// Local Headers
#include "OpenGL.hpp"
#include "Shader.hpp"
#include "Renderer.hpp"
#include "Buffers.hpp"
#include "Texture.hpp"
#include "Camera.hpp"

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
    
    VertexBuffer *VBO = new VertexBuffer(vertices.data(), (unsigned int)vertices.size() * sizeof(float));
    
    VertexArray *VAO = new VertexArray();
    VAO->AddBuffer(*VBO, *layout);

    IndexBuffer *IBO = new IndexBuffer(indices.data(), (unsigned int)indices.size());


    // View, Model and Projection Matrices
    //-------------------------------------------------
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(45.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    
    glm::dvec3 lightPos; 


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
        
        DeltaTime();
        // Process Input
        processInput(Window);
        cursor_position_callback(Window, cursorPos.x, cursorPos.y);

        camera.MouseUpdate(cursorPos);
        camera.Update();
        
        // Background Fill Color
        glClearColor(0.3f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        
        // Draw
        // model = glm::rotate(model, (float)getDeltaTime() * glm::radians(25.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        
        lightPos.x = 5.0 * sin(glfwGetTime() * 0.5f);
        lightPos.z = 5.0 * cos(glfwGetTime() * 0.5f);
        
        shader1.use();
        tex1->bindTexture();
        shader1.setMVPMatrices(glm::value_ptr(model), glm::value_ptr(camera.GetViewMatrix()), glm::value_ptr(camera.GetProjectionMatrix()));
        shader1.setVec3f("lightPos", (float)lightPos.x, 2.0f, (float)lightPos.z);
        shader1.setFloat("Time", (float)getDeltaTime());
        
        VAO->Bind();
        IBO->Bind();
        glDrawElements(GL_TRIANGLES, (GLsizei)indices.size(), GL_UNSIGNED_INT, nullptr);

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