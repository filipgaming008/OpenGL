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

    glfwSetFramebufferSizeCallback(Window, framebuffer_size_callback);

    std::cout << "OpenGL " << glGetString(GL_VERSION) << std::endl;
    std::cout << "GL Renderer: " << glGetString(GL_RENDERER) << std::endl;
    std::cout << "GLSL " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;

    // Start Scope
    {
    // Shaders init
    std::string shaders_location("../res/shaders/");
    std::string shader_name("shader");

    Shader shader1(shaders_location + shader_name + ".vert", shaders_location + shader_name + ".frag");

    Shader lightShader(shaders_location + "light.vert", shaders_location + "light.frag");

    Shader PBRShader(shaders_location + "pbr.vert", shaders_location + "pbr.frag");

    Shader bloomShader(shaders_location + "bloom.vert", shaders_location + "bloom.frag");
    
    Shader gaussianBlurShader(shaders_location + "gaussianBlur.vert", shaders_location + "gaussianBlur.frag");

    Shader toneMap(shaders_location + "toneMap.vert", shaders_location + "toneMap.frag");

    Shader sampleTest(shaders_location + "sampleTest.vert", shaders_location + "sampleTest.frag");

    // Vertex Data for a Plane with Positions (X, Y, Z) and UV Coordinates
    std::vector<float> vertices = {
        // Positions           // UV Coordinates
        -1.0f, -1.0f, 0.0f,    0.0f, 0.0f, // Bottom-left
         1.0f, -1.0f, 0.0f,    1.0f, 0.0f, // Bottom-right
         1.0f,  1.0f, 0.0f,    1.0f, 1.0f, // Top-right
        -1.0f,  1.0f, 0.0f,    0.0f, 1.0f  // Top-left
    };

    std::vector<unsigned int> indices = {
        0, 1, 2,
        0, 2, 3
    };


    // Buffers
    VertexLayout *layout = new VertexLayout();
    layout->Push<float>(3); // Position
    layout->Push<float>(2); // UV
    
    VertexBuffer *VBO = new VertexBuffer(vertices.data(), (unsigned int)vertices.size() * sizeof(float));
    
    VertexArray *VAO = new VertexArray();
    VAO->AddBuffer(*VBO, *layout);

    IndexBuffer *IBO = new IndexBuffer(indices.data(), (unsigned int)indices.size());


    // Models, Light and Materials
    //-------------------------------------------------------------
    Model *model1 = new Model(SPHERE);
    Model *model2 = new Model(CUBE);
    Model *lightModel = new Model(SPHERE);

    Light light;
    light.pos = glm::vec3(0.0f, 0.0f, 0.0f);
    light.ambient = glm::vec3(0.2f, 0.2f, 0.2f);
    light.diffuse = glm::vec3(0.7f, 0.7f, 0.7f);
    light.specular = glm::vec3(1.0f, 1.0f, 1.0f);

    Light light2;
    light2.pos = glm::vec3(0.0f, -3.0f, 0.0f);
    light2.ambient = glm::vec3(35.0f, 32.0f, 33.0f);

    Material material1;
    material1.ambient = glm::vec4(0.8f, 0.2f, 0.2f, 1.0f);
    material1.diffuse = glm::vec3(0.8f, 0.1f, 0.1f);
    material1.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    material1.shine = 32.0f;

    Material material2;
    material2.ambient = glm::vec4(0.2f, 0.2f, 0.8f, 1.0f);
    material2.diffuse = glm::vec3(0.2f, 0.2f, 0.8f);
    material2.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    material2.shine = 32.0f;

    Material material3;
    material3.ambient = glm::vec4(0.1f, 0.8f, 0.3f, 0.5f);
    material3.diffuse = glm::vec3(0.8f, 0.8f, 0.8f);
    material3.specular = glm::vec3(1.0f, 1.0f, 1.0f);
    material3.shine = 32.0f;

    Model *plane = new Model(PLANE);
    plane->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 0.0f, 0.0f))
    * glm::scale(glm::mat4(1.0f), glm::vec3(5.0f)));

    // Textures
    //-------------------------------------------------------------

    std::string textures_location("../res/textures/");

    Texture pbrAlbedo;
    pbrAlbedo.loadTexture(textures_location + "base.png");

    Texture pbrNormal;
    pbrNormal.loadTexture(textures_location + "normal.png");

    Texture pbrMetallic;
    pbrMetallic.loadTexture(textures_location + "metallic.png");

    Texture pbrRoughness;
    pbrRoughness.loadTexture(textures_location + "roughness.png");

    Texture pbrAO;
    pbrAO.loadTexture(textures_location + "ambient.png");

    Texture pbrHeight;
    pbrHeight.loadTexture(textures_location + "height.png");

    
    // Depth Buffer
    FrameBuffer HDRFBO;
    HDRFBO.Bind(GL_FRAMEBUFFER);
    Texture HDRTexture;
    HDRTexture.genTextureHDR(SCR_WIDTH, SCR_HEIGHT);

    DepthBuffer depthBuffer;
    depthBuffer.Bind();

    // Frame Buffers and Textures

    FrameBuffer BloomFBO;
    BloomFBO.Bind(GL_FRAMEBUFFER);
    Texture BloomTexture;
    BloomTexture.genTextureHDR(SCR_WIDTH, SCR_HEIGHT);

    std::vector<FrameBuffer> scaledFBOs(2);
    std::vector<Texture> scaledTextures(2);
    for (int i = 0; i < 2; i++) {
        scaledFBOs[i].Bind(GL_FRAMEBUFFER);
        scaledTextures[i].bindTexture();
        scaledTextures[i].genTextureHDR(SCR_WIDTH / 4, SCR_HEIGHT / 4);
    }


    // Rendering Loop
    while (glfwWindowShouldClose(Window) == false) {
        
        DeltaTime();
        // Process Input
        processInput(Window);
        cursor_position_callback(Window, cursorPos.x, cursorPos.y);
        
        camera.MouseUpdate(cursorPos);
        camera.Update();
        
        // Background Fill Color
        
        HDRFBO.Bind(GL_FRAMEBUFFER);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LESS);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glCullFace(GL_BACK);
        glEnable(GL_CULL_FACE);
        
        // Drawing
        //-------------------------------------------------------------
        
        // Set light position and draw the light source
        light.pos.x = (float)sin(glfwGetTime() * 0.3f) * 2.0f;
        light.pos.y = (float)cos(glfwGetTime() * 0.3f) * 2.0f;
        light2.pos.x = (float)sin(glfwGetTime() * 0.3f) * 3.0f + 10.0f;
        light2.pos.z = (float)cos(glfwGetTime() * 0.3f) * 3.0f;

        lightModel->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(light.pos.x, light.pos.y, light.pos.z)));
        lightModel->SetModelMatrix(glm::scale(lightModel->GetModelMatrix(), glm::vec3(0.2f)));
        lightModel->Draw(lightShader, camera);

        lightModel->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(light2.pos.x, light2.pos.y, light2.pos.z)));
        lightModel->SetModelMatrix(glm::scale(lightModel->GetModelMatrix(), glm::vec3(0.2f)));
        lightModel->Draw(lightShader, camera);

        shader1.use();
        shader1.setVec3f("viewPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        
        // Draw opaque objects first
        glDisable(GL_BLEND);

        
        PBRShader.use();
        pbrAlbedo.bindTexture(0);
        pbrNormal.bindTexture(1);
        pbrMetallic.bindTexture(2);
        pbrRoughness.bindTexture(3);
        pbrAO.bindTexture(4);
        pbrHeight.bindTexture(5);
        PBRShader.setInt("material.albedoMap", 0);
        PBRShader.setInt("material.normalMap", 1);
        PBRShader.setInt("material.metallicMap", 2);
        PBRShader.setInt("material.roughnessMap", 3);
        PBRShader.setInt("material.aoMap", 4);
        PBRShader.setInt("material.heightMap", 5);
        PBRShader.setFloat("material.heightScale", 0.15f);
        PBRShader.setVec3f("viewPos", camera.GetPosition().x, camera.GetPosition().y, camera.GetPosition().z);
        PBRShader.setVec3f("lightPos", light2.pos.x, light2.pos.y, light2.pos.z);
        PBRShader.setVec3f("lightColor", light2.ambient.r, light2.ambient.g, light2.ambient.b);
        plane->Draw(PBRShader, camera);

        shader1.use();
        shader1.setMaterial("material", &material1.ambient[0], &material1.diffuse[0], &material1.specular[0], material1.shine);
        shader1.setLight("light", &light.pos[0], &light.ambient[0], &light.diffuse[0], &light.specular[0]);
        model2->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 3.0f)) 
        * glm::rotate(glm::mat4(1.0f), (float)glfwGetTime() * 0.5f, glm::normalize(-glm::vec3(1.0f, 1.0f, 0.0f))));
        model2->Draw(shader1, camera);
        
        shader1.use();
        shader1.setMaterial("material", &material2.ambient[0], &material2.diffuse[0], &material2.specular[0], material2.shine);
        shader1.setLight("light", &light.pos[0], &light.ambient[0], &light.diffuse[0], &light.specular[0]);
        model2->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, 0.0f, 3.0f)));
        model2->Draw(shader1, camera);

        // Draw transparent objects
        glEnable(GL_BLEND);
        shader1.use();
        shader1.setMaterial("material", &material3.ambient[0], &material3.diffuse[0], &material3.specular[0], material3.shine);
        shader1.setLight("light", &light.pos[0], &light.ambient[0], &light.diffuse[0], &light.specular[0]);
        model1->SetModelMatrix(glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 2.0f)) 
        * glm::scale(glm::mat4(1.0f), glm::vec3((float)sin(glfwGetTime()) * 0.5f + 1.0f)));
        model1->Draw(shader1, camera);
        
        // Post processing
        glDisable(GL_CULL_FACE);
        glDisable(GL_DEPTH_TEST);
        glDisable(GL_BLEND);

        VAO->Bind();
        VBO->Bind();
        IBO->Bind();
        BloomFBO.Bind(GL_FRAMEBUFFER);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        bloomShader.use();
        HDRTexture.bindTexture();
        bloomShader.setInt("scene", 0);
        glDrawElements(GL_TRIANGLES, IBO->GetCount(), GL_UNSIGNED_INT, nullptr);
        
        // Down Sampling

        BloomFBO.Bind(GL_READ_FRAMEBUFFER);
        scaledFBOs[0].Bind(GL_DRAW_FRAMEBUFFER);
        glBlitFramebuffer(0, 0, SCR_WIDTH, SCR_HEIGHT, 0, 0, SCR_WIDTH / 4, SCR_HEIGHT / 4, GL_COLOR_BUFFER_BIT, GL_LINEAR);

        // Blur Passes

        int blurPasses = 8;
        bool horizontal = true;
        for (int i = 0; i < blurPasses; i++) {
            scaledFBOs[horizontal].Bind(GL_FRAMEBUFFER);
            glViewport(0, 0, SCR_WIDTH / 4, SCR_HEIGHT / 4);
            gaussianBlurShader.use();
            gaussianBlurShader.setBool("horizontal", horizontal);

            if (i == 0) {
                scaledTextures[0].bindTexture();
            } else {
                scaledTextures[!horizontal].bindTexture();
            }
            
            gaussianBlurShader.setInt("image", 0);
            glDrawElements(GL_TRIANGLES, IBO->GetCount(), GL_UNSIGNED_INT, nullptr);
            horizontal = !horizontal;  // Switch direction
        }

        
        // // Up Sampling
        scaledFBOs[1].Bind(GL_READ_FRAMEBUFFER);
        BloomFBO.Bind(GL_DRAW_FRAMEBUFFER);
        glBlitFramebuffer(0, 0, SCR_WIDTH / 4, SCR_HEIGHT / 4, 0, 0, SCR_WIDTH, SCR_HEIGHT, GL_COLOR_BUFFER_BIT, GL_LINEAR);
        
        // // Draw Test
        // glBindFramebuffer(GL_FRAMEBUFFER, 0);
        // glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        // sampleTest.use();
        // HDRTexture.bindTexture(0);
        // sampleTest.setInt("scene", 0);
        // glDrawElements(GL_TRIANGLES, IBO->GetCount(), GL_UNSIGNED_INT, nullptr);
        
        // Draw with Bloom
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
        toneMap.use();
        HDRTexture.bindTexture(0);
        BloomTexture.bindTexture(1);
        toneMap.setInt("hdrBuffer", 0);
        toneMap.setInt("bloomBuffer", 1);
        glDrawElements(GL_TRIANGLES, IBO->GetCount(), GL_UNSIGNED_INT, nullptr);

        // Flip Buffers and Draw
        glfwSwapBuffers(Window);
        glfwPollEvents();
    }   
    
    // Cleanup
    delete model1;
    delete model2;
    delete plane;
    delete lightModel;
    // delete layout;
    // delete VAO;
    // delete VBO;
    // delete IBO;

    // End Scope
    }
    
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
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
        camera.Reset();
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