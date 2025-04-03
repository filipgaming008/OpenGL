#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include <cmath>
#include <math.h>

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

static const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";
static const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

static const char *fragmentShaderSourceBlack = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "   FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);\n"
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
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    // check for shader compile errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    // fragment shader
    GLuint fragmentShaderBlack = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShaderBlack, 1, &fragmentShaderSourceBlack, nullptr);
    glCompileShader(fragmentShaderBlack);
    // check for shader compile errors
    glGetShaderiv(fragmentShaderBlack, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragmentShaderBlack, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    
    // link shaders
    GLuint shaderProgramB = glCreateProgram();
    glAttachShader(shaderProgramB, vertexShader);
    glAttachShader(shaderProgramB, fragmentShaderBlack);
    glLinkProgram(shaderProgramB);
    // check for linking errors
    glGetProgramiv(shaderProgramB, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgramB, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }

    // link shaders
    GLuint shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
    }
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    glDeleteShader(fragmentShaderBlack);

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    std::vector<float> vertices;

    int type;
    std::cout<<"Select the type of shape to draw: ";
    std::cin>>type;

    int n;
    int circle;
    int radiationthing;
    int radiationthingstart;
    if(type == 1){
        n = 8;
        float r = 0.5f;
        float xc=0.0f, yc=0.0f, zc=0.0f;
        float angle_begin = M_PI/4;
        float delta_angle = 2*M_PI/n;
        //angle += delta_angle/2.0f;
    
        // vertices.push_back(xc);
        // vertices.push_back(yc);
        // vertices.push_back(zc);
    
        int x = 0;  
        glBegin(GL_TRIANGLES);
        for (int i=0; i<n+1; i++) {
            vertices.push_back(xc+r*cos(angle_begin));
            vertices.push_back(yc+r*sin(angle_begin));
            vertices.push_back(zc);
            x++;
            if(x%3 == 0){
                if(i == n+1){
                    glEnd();
                    break;
                }
                glEnd();
                glBegin(GL_TRIANGLES);
                vertices.push_back(xc+r*cos(angle_begin));
                vertices.push_back(yc+r*sin(angle_begin));
                vertices.push_back(zc);
                x=1;
            }
            angle_begin+=delta_angle;
        }
        n = n + 4;
        
        // x = 12;
        // angle_begin = 0.0f;
        // delta_angle = 2*M_PI / x;
        // r = 0.6f;
        // for(int i = 0; i<x+2; i++){
        //     vertices.push_back(xc+r*cos(angle_begin));
        //     vertices.push_back(yc+r*sin(angle_begin));
        //     vertices.push_back(zc);
        //     angle_begin+=delta_angle;
        // }
    }
    else if(type == 3){
        int resolution = 100;
        float r = 0.2f;
        float xc=0.0f, yc=0.0f, zc=0.0f;
        float angle_begin = 0.0f;
        float delta_angle = 2*M_PI/resolution;
        //angle += delta_angle/2.0f;
        
        vertices.push_back(xc);
        vertices.push_back(yc);
        vertices.push_back(zc);
        
        for (int i=0; i<resolution+1; i++) {
            vertices.push_back(xc+r*cos(angle_begin));
            vertices.push_back(yc+r*sin(angle_begin));
            vertices.push_back(zc);
            angle_begin+=delta_angle;
        }

        double r1 = 0.3f;
        double r2 = 0.8f;
        angle_begin = 0.0f;
        delta_angle = 2*M_PI/ 6 / resolution;
        double shift = 2*M_PI / 3;
        for(int j=0; j<3; j++){
            angle_begin = shift * j;
            for(int i=0; i<resolution; i++) {
                vertices.push_back(xc+r1*cos(angle_begin));
                vertices.push_back(yc+r1*sin(angle_begin));
                vertices.push_back(zc);
                
                vertices.push_back(xc+r2*cos(angle_begin));
                vertices.push_back(yc+r2*sin(angle_begin));
                vertices.push_back(zc);
                
                angle_begin+=delta_angle;
            }
        }
        circle = resolution + 2;
        radiationthing = resolution;
    }
    else if(type == 2){

    }
    

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // bind the Vertex Array Object first, then bind and set vertex buffer(s), and then configure vertex attributes(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), &vertices[0], GL_STATIC_DRAW);
    //glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_a), vertices_a, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);
    glEnableVertexAttribArray(0);

    // note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
    // VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
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
        if(type == 3){
            glClearColor(1.0f, 1.0f, 0.0f, 0.0f);
        }else{
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);

        }

        glClear(GL_COLOR_BUFFER_BIT);

        // draw our first triangle
        glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
        
        if(type == 3){
            glUseProgram(shaderProgramB);
            glDrawArrays(GL_TRIANGLE_FAN, 0, circle);
            glDrawArrays(GL_TRIANGLE_STRIP, radiationthing + 2, radiationthing * 2);
            glDrawArrays(GL_TRIANGLE_STRIP, radiationthing * 3 + 2, radiationthing * 2);
            glDrawArrays(GL_TRIANGLE_STRIP, radiationthing * 5 + 2, radiationthing * 2);
        }else if(type == 1){
            glUseProgram(shaderProgram);
            // glDrawArrays(GL_LINE_STRIP, n + 1, 6);
            glDrawArrays(GL_TRIANGLES, 0, n);
        }
        
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