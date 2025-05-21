#include "Model.hpp"

#include "glm/gtc/constants.hpp"
#include "glm/gtc/type_ptr.hpp"

Model::Model(enum ModelType type)
{
    model = glm::mat4(1.0f);
    switch (type){
        case CUBE: Cube(); break;
        case PLANE: Plane(); break;
        case SPHERE: Sphere(); break;
    }
}

Model::~Model()
{
    delete layout;
    delete VAO;
    delete VBO;
    delete IBO;
}

void Model::Cube()
{
    std::vector<float> vertices = {
        // Front face
        // Position          Normal            UV
        -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // 0
        0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f, // 1
        0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // 2
        -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, // 3

        // Back face
        0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // 4
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // 5
        -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // 6
        0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, // 7

        // Left face
        -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 8
        -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 9
        -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 10
        -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 11

        // Right face
        0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // 12
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // 13
        0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // 14
        0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // 15

        // Top face
        -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, // 16
        0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // 17
        0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // 18
        -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, // 19

        // Bottom face
        -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // 20
        0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // 21
        0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // 22
        -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f  // 23
    };

    std::vector<unsigned int> indices = {
        // Front face
        0, 1, 2,
        0, 2, 3,
        
        // Back face
        4, 5, 6,
        4, 6, 7,
        
        // Left face
        8, 9, 10,
        8, 10, 11,
        
        // Right face
        12, 13, 14,
        12, 14, 15,
        
        // Top face
        16, 17, 18,
        16, 18, 19,
        
        // Bottom face
        20, 21, 22,
        20, 22, 23
    };

    layout = new VertexLayout();
    layout->Push<float>(3); // Position
    layout->Push<float>(3); // Normal
    layout->Push<float>(2); // UV

    VBO = new VertexBuffer(vertices.data(), (unsigned int)vertices.size() * sizeof(float));
    VAO = new VertexArray();
    VAO->AddBuffer(*VBO, *layout);
    IBO = new IndexBuffer(indices.data(), (unsigned int)indices.size());
}

void Model::Plane()
{
    std::vector<float> vertices = {
        // Positions           // Normals             // UVs      // Tangent
         0.5f,  0.0f,  0.5f,    0.0f, -1.0f, 0.0f,   1.0f, 1.0f,  1.0f, 0.0f, 0.0f, // 0
         0.5f,  0.0f, -0.5f,    0.0f, -1.0f, 0.0f,   1.0f, 0.0f,  1.0f, 0.0f, 0.0f, // 1
        -0.5f,  0.0f, -0.5f,    0.0f, -1.0f, 0.0f,   0.0f, 0.0f,  1.0f, 0.0f, 0.0f, // 2
        -0.5f,  0.0f,  0.5f,    0.0f, -1.0f, 0.0f,   0.0f, 1.0f,  1.0f, 0.0f, 0.0f, // 3
    };

    std::vector<unsigned int> indices = {
        0, 2, 1,
        0, 3, 2
    };

    layout = new VertexLayout();
    layout->Push<float>(3); // Position
    layout->Push<float>(3); // Normal
    layout->Push<float>(2); // UV
    layout->Push<float>(3); // Tangent

    VBO = new VertexBuffer(vertices.data(), (unsigned int)vertices.size() * sizeof(float));
    VAO = new VertexArray();
    VAO->AddBuffer(*VBO, *layout);
    IBO = new IndexBuffer(indices.data(), (unsigned int)indices.size());
}

void Model::Sphere()
{
    const unsigned int sectorCount = 72;  // Longitude segments
    const unsigned int stackCount = 36;   // Latitude segments
    const float radius = 0.5f;
    const float PI = glm::pi<float>();

    // Pre-calculate trigonometric values for optimization
    std::vector<float> sinPhi(stackCount + 1);
    std::vector<float> cosPhi(stackCount + 1);
    for (unsigned int i = 0; i <= stackCount; ++i) {
        float phi = PI / 2.0f - i * PI / stackCount;  // Vertical angle
        sinPhi[i] = sinf(phi);
        cosPhi[i] = cosf(phi);
    }

    std::vector<float> sinTheta(sectorCount + 1);
    std::vector<float> cosTheta(sectorCount + 1);
    for (unsigned int j = 0; j <= sectorCount; ++j) {
        float theta = 2.0f * PI * j / sectorCount;  // Horizontal angle
        sinTheta[j] = sinf(theta);
        cosTheta[j] = cosf(theta);
    }

    // Reserve space for vertices and indices
    std::vector<float> vertices;
    vertices.reserve((stackCount + 1) * (sectorCount + 1) * 8);  // 8 components per vertex
    std::vector<unsigned int> indices;
    indices.reserve(stackCount * sectorCount * 6);  // 6 indices per quad

    // Generate vertices
    for (unsigned int i = 0; i <= stackCount; ++i) {
        float xy = radius * cosPhi[i];  // Radius on xy-plane
        float z = radius * sinPhi[i];

        for (unsigned int j = 0; j <= sectorCount; ++j) {
            // Position (XYZ)
            float x = xy * cosTheta[j];
            float y = xy * sinTheta[j];
            vertices.insert(vertices.end(), {x, y, z});

            // Normal (unit vector from sphere center)
            float nx = cosPhi[i] * cosTheta[j];
            float ny = cosPhi[i] * sinTheta[j];
            float nz = sinPhi[i];
            vertices.insert(vertices.end(), {nx, ny, nz});

            // UV coordinates (flipped V for OpenGL texture origin)
            float u = static_cast<float>(j) / sectorCount;
            float v = 1.0f - static_cast<float>(i) / stackCount;
            vertices.insert(vertices.end(), {u, v});
        }
    }

    // Generate indices with CCW winding
    for (unsigned int i = 0; i < stackCount; ++i) {
        for (unsigned int j = 0; j < sectorCount; ++j) {
            unsigned int current = i * (sectorCount + 1) + j;
            unsigned int next = current + sectorCount + 1;

            // Two triangles per sector except at poles
            if (i != 0) {
                indices.insert(indices.end(), {current, next, current + 1});
            }
            if (i != stackCount - 1) {
                indices.insert(indices.end(), {current + 1, next, next + 1});
            }
        }
    }

    // UV seam fix - duplicate vertices at texture seam
    // No UV seam fix needed; OpenGL can handle the seam with proper UVs and duplicated vertices at the seam if required.
    // Create GPU buffers
    layout = new VertexLayout();
    layout->Push<float>(3);  // Position
    layout->Push<float>(3);  // Normal
    layout->Push<float>(2);  // UV

    VBO = new VertexBuffer(vertices.data(), static_cast<unsigned int>(vertices.size() * sizeof(float)));
    VAO = new VertexArray();
    VAO->AddBuffer(*VBO, *layout);
    IBO = new IndexBuffer(indices.data(), static_cast<unsigned int>(indices.size()));
}

void Model::Draw(Shader &shader, const Camera &camera)
{
    VAO->Bind();
    IBO->Bind();
    shader.use();
    shader.setMVPMatrices(glm::value_ptr(model), glm::value_ptr(camera.GetViewMatrix()), glm::value_ptr(camera.GetProjectionMatrix()));
    glDrawElements(GL_TRIANGLES, (GLsizei)IBO->GetCount(), GL_UNSIGNED_INT, nullptr);
}