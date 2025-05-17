#pragma once

#include <glad/gl.h>
#include <vector>


// Vertex Buffer
// -------------------------------------------------
class VertexBuffer{
private:
    unsigned int Renderer_ID;
public:
    VertexBuffer(const void *data, unsigned int size);
    ~VertexBuffer();
    
    void Bind() const;
    void Unbind() const;
};


// Index Buffer
// -------------------------------------------------
class IndexBuffer{
private:
    unsigned int Renderer_ID;
    unsigned int count;
public:
    IndexBuffer(const unsigned int *data, unsigned int _count);
    ~IndexBuffer();
    
    void Bind() const;
    void Unbind() const;
    
    inline unsigned int GetCount() const { return count; }
};


// Vertex Layout
// -------------------------------------------------
struct VertexBufferElement{
    unsigned int type;
    unsigned int count;
    unsigned char normalized;

    static unsigned int GetSizeOfType(unsigned int type){
        switch (type){
            case GL_FLOAT: return 4;
            case GL_UNSIGNED_INT: return 4;
            case GL_UNSIGNED_BYTE: return 1;
        }
        return 0;
    }
};


// Vertex Layout
// -------------------------------------------------
class VertexLayout{
private:
    std::vector<VertexBufferElement> Elements;
    unsigned int Stride;

public:

    VertexLayout() : Stride(0) {};
    ~VertexLayout();
    
    template<typename T>
    void Push(unsigned int count);
    
    inline const std::vector<VertexBufferElement> &GetElements() const { return Elements; }
    inline unsigned int GetStride() const { return Stride;}
};


// Vertex Array
// -------------------------------------------------
class VertexArray{
    private:
    unsigned int Renderer_ID;
    public:
    VertexArray();
    ~VertexArray();

    void Bind() const;
    void Unbind() const;

    void AddBuffer(const VertexBuffer &vb, const VertexLayout &layout);
};