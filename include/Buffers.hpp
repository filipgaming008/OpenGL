#pragma once

class VertexBuffer{
private:
    unsigned int Renderer_ID;
public:
    VertexBuffer(const void *data, unsigned int size);
    ~VertexBuffer();
    
    void Bind() const;
    void Unbind() const;
};

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