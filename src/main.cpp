#include <OpenGL.hpp>

#include <GLFW/glfw3.h>

#include <Shader.hpp>

#include <iostream>
#include <string>
#include <vector>
#include <cmath>
#include <math.h>

double M_PI = 3.14159265358979323846;

const std::string program_name = ("GLSL Shader class example");

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, program_name.c_str(), nullptr, nullptr);
    if (window == nullptr)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // build and compile our shader program
    // ------------------------------------

    std::string shader_location("../res/shaders/");

    std::string used_shaders("shader1");

    Shader Shader1(shader_location + used_shaders + std::string(".vert"),
                     shader_location + used_shaders + std::string(".frag")
                     );

    used_shaders = "shader2";
    Shader Shader2(shader_location + used_shaders + std::string(".vert"),
                     shader_location + used_shaders + std::string(".frag")
                     );

    used_shaders = "shader3";
    Shader Shader3(shader_location + used_shaders + std::string(".vert"),
                     shader_location + used_shaders + std::string(".frag")
                     );


    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    
    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    std::cout<<"Select a picture to draw from 1 to 4: ";
    int type;
    std::cin>>type;

    int resolution;

    if(type == 1){

        vertices.push_back(-0.8f); vertices.push_back(0.4f);  // top left
        vertices.push_back(-0.6f); vertices.push_back(0.4f);  // top right
        vertices.push_back(-0.8f); vertices.push_back(-0.4f); // bottom left
        vertices.push_back(-0.6f); vertices.push_back(-0.4f); // bottom right

        indices.push_back(0); indices.push_back(1); indices.push_back(2); // first triangle
        indices.push_back(1); indices.push_back(2); indices.push_back(3); // second triangle

        float xc = 0.1f;
        float yc = 0.0f;
        resolution = 100;
        float r1 = 0.5f;
        float r2 = 0.3f;
        float start_angle = 0.0f;
        float delta_angle = 2*M_PI/resolution;

        vertices.push_back(xc); vertices.push_back(yc);

        for(int i = 0; i < resolution + 1; i++)
        {
            float x = xc + r1 * cos(start_angle);
            float y = yc + r1 * sin(start_angle);
            vertices.push_back(x);
            vertices.push_back(y);
            
            start_angle += delta_angle;
        }

        vertices.push_back(xc); vertices.push_back(yc);

        for(int i = 0; i < resolution + 1; i++)
        {
            float x = xc + r2 * cos(start_angle);
            float y = yc + r2 * sin(start_angle);
            vertices.push_back(x);
            vertices.push_back(y);
            
            start_angle += delta_angle;
        }

    }
    else if(type == 2){
        float rectWidth = 1.8f;
        float rectHeight = 0.2f;
        float startX = -0.9f;
        float startY = 0.9f;
        float spacing = 0.2f;

        for (int i = 0; i < 5; ++i) {
            float x1 = startX;
            float y1 = startY - i * (rectHeight + spacing);
            float x2 = x1 + rectWidth;
            float y2 = y1 - rectHeight;

            vertices.push_back(x1); vertices.push_back(y1); // top left
            vertices.push_back(x2); vertices.push_back(y1); // top right
            vertices.push_back(x1); vertices.push_back(y2); // bottom left
            vertices.push_back(x2); vertices.push_back(y2); // bottom right

            // unsigned int offset = i * 4;
            // indices.push_back(offset); indices.push_back(offset + 1); indices.push_back(offset + 2); // first triangle
            // indices.push_back(offset + 1); indices.push_back(offset + 2); indices.push_back(offset + 3); // second triangle
        }
    }
    else if(type == 3){
        float xc = 0.0f;
        float yc = 0.0f;
        resolution = 100;
        float r1 = 0.6f;

        float start_angle = 0.0f;
        float delta_angle = 2*M_PI/resolution;

        vertices.push_back(xc); vertices.push_back(yc);

        for(int i = 0; i < resolution + 1; i++)
        {
            float x = xc + r1 * cos(start_angle);
            float y = yc + r1 * sin(start_angle);
            vertices.push_back(x);
            vertices.push_back(y);
            
            start_angle += delta_angle;
        }



        // vertices.push_back(-0.5f); vertices.push_back(0.5f);  // top left
        // vertices.push_back(0.5f); vertices.push_back(0.5f);   // top right
        // vertices.push_back(-0.5f); vertices.push_back(-0.5f); // bottom left
        // vertices.push_back(0.5f); vertices.push_back(-0.5f);  // bottom right

        // indices.push_back(0); indices.push_back(1); indices.push_back(2); // first triangle
        // indices.push_back(1); indices.push_back(2); indices.push_back(3); // second triangle

    }

    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), static_cast<void*>(nullptr));
    glEnableVertexAttribArray(0);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBindVertexArray(VAO);
        // render container
        if (type == 1){
            Shader1.use();
            Shader1.setFloat("R", 0.1f);
            Shader1.setFloat("G", 0.1f);
            Shader1.setFloat("B", 0.9f);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
            Shader1.setFloat("R", 0.0f);
            Shader1.setFloat("G", 0.5f);
            Shader1.setFloat("B", 0.9f);
            glDrawArrays(GL_TRIANGLE_FAN, 4, resolution + 4);
            Shader1.setFloat("R", 1.0f);
            Shader1.setFloat("G", 1.0f);
            Shader1.setFloat("B", 1.0f);
            glDrawArrays(GL_TRIANGLE_FAN, resolution + 4 + 3, resolution * 2 + 4);
        }
        else if(type == 2){
            Shader2.use();
            Shader2.setVec2("Resolution", SCR_WIDTH, SCR_HEIGHT);

            Shader2.setVec3("Color1", 0.0f, 0.0f, 1.0f);
            Shader2.setVec3("Color2", 1.0f, 1.0f, 0.0f);
            glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

            Shader2.setVec3("Color1", 1.0f, 0.0f, 1.0f);
            Shader2.setVec3("Color2", 1.0f, 0.5f, 0.0f);
            glDrawArrays(GL_TRIANGLE_STRIP, 4, 4);

            Shader2.setVec3("Color1", 1.0f, 0.0f, 0.0f);
            Shader2.setVec3("Color2", 0.0f, 1.0f, 0.0f);
            glDrawArrays(GL_TRIANGLE_STRIP, 8, 4);

            Shader2.setVec3("Color1", 0.0f, 0.6f, 0.8f);
            Shader2.setVec3("Color2", 0.0f, 0.0f, 1.0f);
            glDrawArrays(GL_TRIANGLE_STRIP, 12, 4);

            Shader2.setVec3("Color1", 0.0f, 0.6f, 0.8f);
            Shader2.setVec3("Color2", 1.0f, 0.5f, 0.0f);
            glDrawArrays(GL_TRIANGLE_STRIP, 16, 4);
        }
        else if(type == 3){
            Shader3.use();
            Shader3.setVec2("Resolution", SCR_WIDTH, SCR_HEIGHT);

            glDrawArrays(GL_TRIANGLE_FAN, 0, resolution + 2);
            // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
        }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS){
        glfwSetWindowShouldClose(window, true);
    }
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}