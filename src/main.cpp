// Local Headers
#include "OpenGL.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Quaternion.hpp"

// System Headers
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <iostream>
#include <vector>
#include <string>


// Function Prototypes
static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow * window);

const std::string name = "Shader Testing Stuff";

Camera camera;
glm::dvec2 mousePos(0.0f, 0.0f);


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
    gladLoadGL();
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Shaders init
    std::string shaders_location("../res/shaders/");
    std::string shader_name("shader");

    Shader shader1(shaders_location + shader_name + ".vert", shaders_location + shader_name + ".frag");

    shader1.use();
    shader1.setVec2f("resolution", float(SCR_WIDTH), float(SCR_HEIGHT));

    // Vertex Data
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    vertices.push_back(-1.0f); vertices.push_back(-1.0f); // bottom left
    vertices.push_back(1.0f);  vertices.push_back(-1.0f); // bottom right
    vertices.push_back(1.0f);  vertices.push_back(1.0f);  // top right
    vertices.push_back(-1.0f); vertices.push_back(1.0f);  // top left

    indices.push_back(0); indices.push_back(1); indices.push_back(2); // first triangle
    indices.push_back(0); indices.push_back(2); indices.push_back(3); // second triangle

    // Buffers
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);


    // Rendering Loop
    while (glfwWindowShouldClose(Window) == false) {
        
        // Process Input
        processInput(Window);
        cursor_position_callback(Window, mousePos.x, mousePos.y);
        camera.mouse(glm::make_vec2(mousePos));

        // Background Fill Color
        glClearColor(0.25f, 0.25f, 0.25f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        shader1.use();
        shader1.setFloat("time", (float)glfwGetTime());
        shader1.setVec3f("rayOrigin" , camera.position.x, camera.position.y, camera.position.z);
        shader1.setVec3f("rayDirection" , camera.direction.x, camera.direction.y, camera.direction.z);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, nullptr);
        

        // Flip Buffers and Draw
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }   

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    
    glfwTerminate();
    return EXIT_SUCCESS;
}

void processInput(GLFWwindow * window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        // std::cout << "W key pressed" << std::endl;
        camera.moveF(0.05f);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        // std::cout << "S key pressed" << std::endl;
        camera.moveF(-0.05f);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        // std::cout << "A key pressed" << std::endl;
        camera.moveR(0.05f);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        // std::cout << "D key pressed" << std::endl;
        camera.moveR(-0.05f);
    if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
        // std::cout << "E key pressed" << std::endl;
        camera.moveU(-0.05f);
    if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS)
        // std::cout << "Q key pressed" << std::endl;
        camera.moveU(0.05f);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        // std::cout << "Space key pressed" << std::endl;
        camera.position = glm::vec3(0.0, 0.0, -2.0);
}

static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos)
{
    glfwGetCursorPos(window, &xpos, &ypos);
    // std::cout<<"Mouse Position: " << xpos << ", " << ypos << std::endl;
    mousePos.x = xpos;
    mousePos.y = ypos;
}