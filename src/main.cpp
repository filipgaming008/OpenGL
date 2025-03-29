#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cmath>

#include <iostream>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

double PI = 3.14159265358979323846;

static const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
static const char *fragmentShaderSourceBF = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0, 0.0, 1.0, 1.0);\n"
    "}\n\0";
static const char *fragmentShaderSourceRF = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0, 0.0, 0.0, 1.0);\n"
    "}\n\0";
static const char *fragmentShaderSourceBB = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0, 0.0, 0.5, 1.0);\n"
    "}\n\0";
static const char *fragmentShaderSourceRB = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.5, 0.0, 0.0, 1.0);\n"
    "}\n\0";
static const char *fragmentShaderSourceDoor = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.25, 0.15, 0.1, 1.0);\n"
    "}\n\0";
static const char *fragmentShaderSourceWindow = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0, 1.0, 1.0, 1.0);\n"
    "}\n\0";
int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", nullptr, nullptr);
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
    if (!gladLoadGLLoader(GLADloadproc(glfwGetProcAddress)))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    // build and compile our shader program
    // ------------------------------------
    // vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    // check for shader compile errors
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    GLuint fragmentShaderBF = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderBF, 1, &fragmentShaderSourceBF, nullptr);
    glCompileShader(fragmentShaderBF);
    // check for shader compile errors
    glGetShaderiv(fragmentShaderBF, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderBF, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    GLuint fragmentShaderRF = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderRF, 1, &fragmentShaderSourceRF, nullptr);
    glCompileShader(fragmentShaderRF);
    // check for shader compile errors
    glGetShaderiv(fragmentShaderRF, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderRF, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    GLuint fragmentShaderBB = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderBB, 1, &fragmentShaderSourceBB, nullptr);
    glCompileShader(fragmentShaderBB);
    // check for shader compile errors
    glGetShaderiv(fragmentShaderBB, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderBB, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    GLuint fragmentShaderRB = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderRB, 1, &fragmentShaderSourceRB, nullptr);
    glCompileShader(fragmentShaderRB);
    // check for shader compile errors
    glGetShaderiv(fragmentShaderRB, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderRB, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    GLuint fragmentShaderDoor = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderDoor, 1, &fragmentShaderSourceDoor, nullptr);
    glCompileShader(fragmentShaderDoor);
    // check for shader compile errors
    glGetShaderiv(fragmentShaderDoor, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderDoor, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    // fragment shader
    GLuint fragmentShaderWindow = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderWindow, 1, &fragmentShaderSourceWindow, nullptr);
    glCompileShader(fragmentShaderWindow);
    // check for shader compile errors
    glGetShaderiv(fragmentShaderWindow, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderWindow, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    // link shaders
    GLuint shaderProgram1 = glCreateProgram();
    glAttachShader(shaderProgram1, vertexShader);
    glAttachShader(shaderProgram1, fragmentShaderBF);
    glLinkProgram(shaderProgram1);
    // check for linking errors
    glGetProgramiv(shaderProgram1, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram1, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(fragmentShaderBF);
    // link shaders
    GLuint shaderProgram2 = glCreateProgram();
    glAttachShader(shaderProgram2, vertexShader);
    glAttachShader(shaderProgram2, fragmentShaderRF);
    glLinkProgram(shaderProgram2);
    // check for linking errors
    glGetProgramiv(shaderProgram2, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram2, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(fragmentShaderRF);
    // link shaders
    GLuint shaderProgram3 = glCreateProgram();
    glAttachShader(shaderProgram3, vertexShader);
    glAttachShader(shaderProgram3, fragmentShaderRB);
    glLinkProgram(shaderProgram3);
    // check for linking errors
    glGetProgramiv(shaderProgram3, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram3, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(fragmentShaderRB);
    // link shaders
    GLuint shaderProgram4 = glCreateProgram();
    glAttachShader(shaderProgram4, vertexShader);
    glAttachShader(shaderProgram4, fragmentShaderBB);
    glLinkProgram(shaderProgram4);
    // check for linking errors
    glGetProgramiv(shaderProgram4, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram4, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(fragmentShaderBB);
    // link shaders
    GLuint shaderProgram5 = glCreateProgram();
    glAttachShader(shaderProgram5, vertexShader);
    glAttachShader(shaderProgram5, fragmentShaderDoor);
    glLinkProgram(shaderProgram5);
    // check for linking errors
    glGetProgramiv(shaderProgram5, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram5, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(fragmentShaderDoor);
    // link shaders
    GLuint shaderProgram6 = glCreateProgram();
    glAttachShader(shaderProgram6, vertexShader);
    glAttachShader(shaderProgram6, fragmentShaderWindow);
    glLinkProgram(shaderProgram6);
    // check for linking error
    glGetProgramiv(shaderProgram6, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram6, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(fragmentShaderDoor);
    glDeleteShader(vertexShader);
    

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    double r = 0.65f;
    double rw = 0.2f;
    double angle = 45.0f * (PI / 180.0f);

    std::vector<float> vertices;
    // Triangle 1
    // v1 Bright
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    // v2 Bleft
    vertices.push_back(-0.8f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    // v3 Tright
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    // Triangle 2
    // v1 Bleft
    vertices.push_back(-0.8f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    // v2 Tright
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    // v3 Tleft
    vertices.push_back(-0.8f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    // Triangle 3
    // v1 Bright
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    // v2 Bleft
    vertices.push_back(-0.8f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    // v3 top
    vertices.push_back(-0.4f);
    vertices.push_back(0.5f);
    vertices.push_back(0.0f);
    // Triangle 4
    // v1 bottom
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    // v2 right
    vertices.push_back(r*cos(angle));
    vertices.push_back(r*sin(angle) - 1.0f);
    vertices.push_back(0.0f);
    // v3 top
    vertices.push_back(r*cos(angle));
    vertices.push_back(r*sin(angle));
    vertices.push_back(0.0f);
    // Triangle 5
    // v1 bottom
    vertices.push_back(0.0f);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    // v2 top
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    // v3 right
    vertices.push_back(r*cos(angle));
    vertices.push_back(r*sin(angle));
    vertices.push_back(0.0f);
    // Triangle 6
    // v1 bottom
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    // v2 top
    vertices.push_back(-0.4f);
    vertices.push_back(0.5f);
    vertices.push_back(0.0f);
    // v3 right
    vertices.push_back(r*cos(angle));
    vertices.push_back(r*sin(angle));
    vertices.push_back(0.0f);
    // Triangle 7
    // v1 right
    vertices.push_back(r*cos(angle));
    vertices.push_back(r*sin(angle));
    vertices.push_back(0.0f);
    // v2 left
    vertices.push_back(-0.4f);
    vertices.push_back(0.5f);
    vertices.push_back(0.0f);
    // v3 Tright
    vertices.push_back(r*cos(angle) - 0.4f);
    vertices.push_back(r*sin(angle) + 0.5f);
    vertices.push_back(0.0f);
    // Triangle 8 Door
    // v1 left
    vertices.push_back((-0.8f + -0.5) / 2);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    // v2 right
    vertices.push_back(-0.8f / 4);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    // v3 Tright
    vertices.push_back(-0.8f / 4);
    vertices.push_back(-0.3f);
    vertices.push_back(0.0f);
    // Triangle 9 Door
    // v1 left
    vertices.push_back((-0.8f + -0.5) / 2);
    vertices.push_back(-1.0f);
    vertices.push_back(0.0f);
    // v2 top
    vertices.push_back((-0.8f + -0.5) / 2);
    vertices.push_back(-0.3f);
    vertices.push_back(0.0f);
    // v3 TRight
    vertices.push_back(-0.8f / 4);
    vertices.push_back(-0.3f);
    vertices.push_back(0.0f);
    // Triangle 10 Window 1
    // v1 Bleft
    vertices.push_back(-0.8f / 4 + 0.30f);
    vertices.push_back(-0.3f);
    vertices.push_back(0.0f);
    // v2 top
    vertices.push_back(-0.8f / 4 + 0.30f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    // v3 Bright
    vertices.push_back(rw*cos(angle) + -0.8f / 4 + 0.30f);
    vertices.push_back(rw*sin(angle) + -0.3f);
    vertices.push_back(0.0f);
    // Triangle 11 Window 1
    // v1 Top
    vertices.push_back(-0.8f / 4 + 0.30f);
    vertices.push_back(0.0f);
    vertices.push_back(0.0f);
    // v2 BRight
    vertices.push_back(rw*cos(angle) + -0.8f / 4 + 0.30f);
    vertices.push_back(rw*sin(angle) + -0.3f);
    vertices.push_back(0.0f);
    // v3 TRight
    vertices.push_back(rw*cos(angle) + -0.8f / 4 + 0.30f);
    vertices.push_back(rw*sin(angle));
    vertices.push_back(0.0f);
    // Triangle 12 Window 2
    // v1 Bleft
    vertices.push_back(rw*cos(angle) + -0.8f / 4 + 0.30f + 0.05f);
    vertices.push_back(rw*sin(angle) + -0.3f + 0.05f);
    vertices.push_back(0.0f);
    // v2 Tleft
    vertices.push_back(rw*cos(angle) + -0.8f / 4 + 0.30f + 0.05f);
    vertices.push_back(rw*sin(angle) + -0.3f + 0.05f + 0.30f);
    vertices.push_back(0.0f);
    // v3 Bright
    vertices.push_back(rw*cos(angle) + -0.8f / 4 + 0.30f + 0.05f + (rw*cos(angle)));
    vertices.push_back(rw*sin(angle) + -0.3f + 0.05f + (rw*sin(angle)));
    vertices.push_back(0.0f);
    // Triangle 13 Window 2
    // v1
    vertices.push_back(rw*cos(angle) + -0.8f / 4 + 0.30f + 0.05f);
    vertices.push_back(rw*sin(angle) + -0.3f + 0.05f + 0.30f);
    vertices.push_back(0.0f);
    // v2
    vertices.push_back(rw*cos(angle) + -0.8f / 4 + 0.30f + 0.05f + (rw*cos(angle)));
    vertices.push_back(rw*sin(angle) + -0.3f + 0.05f + (rw*sin(angle)));
    vertices.push_back(0.0f);
    // v3 Tright
    vertices.push_back(rw*cos(angle) + -0.8f / 4 + 0.30f + 0.05f + (rw*cos(angle)));
    vertices.push_back(rw*sin(angle) + -0.3f + 0.05f + 0.30f + (rw*sin(angle)));
    vertices.push_back(0.0f);

    // vertices.push_back(rw*cos(angle) + -0.8f / 4 + 0.30f);
    // vertices.push_back(rw*sin(angle));
    // vertices.push_back(0.0f);
   

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), &vertices[0], GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's
    // bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO,
    // but this rarely happens. Modifying other VAOs requires a call to glBindVertexArray anyways
    // so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
    glBindVertexArray(0);


    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glUseProgram(shaderProgram1);

        // seeing as we only have a single VAO there's no need to bind it every time,
        // but we'll do so to keep things a bit more organized
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        glUseProgram(shaderProgram2);
        glDrawArrays(GL_TRIANGLES, 6, 9);
    
        glUseProgram(shaderProgram4);
        glDrawArrays(GL_TRIANGLES, 9, 15);

        glUseProgram(shaderProgram3);
        glDrawArrays(GL_TRIANGLES, 15, 21);

        glUseProgram(shaderProgram5);
        glDrawArrays(GL_TRIANGLES, 21, 27);

        glUseProgram(shaderProgram6);
        glDrawArrays(GL_TRIANGLES, 27, 39);

        // glBindVertexArray(0); // no need to unbind it every time

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}
