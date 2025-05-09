#include <cstdint>

#include "Buffers.hpp"
#include "Renderer.hpp"


// VertexBuffer
// -------------------------------------------------
VertexBuffer::VertexBuffer(const void* data, unsigned int size){
    GLCall(glGenBuffers(1, &Renderer_ID));
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, Renderer_ID));
    GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
}
VertexBuffer::~VertexBuffer(){
    GLCall(glDeleteBuffers(1, &Renderer_ID));
}

void VertexBuffer::Bind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, Renderer_ID));
}

void VertexBuffer::Unbind() const{
    GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}


// IndexBuffer
// -------------------------------------------------
IndexBuffer::IndexBuffer(const unsigned int *data, unsigned int _count) : count(_count){
    GLCall(glGenBuffers(1, &Renderer_ID));
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer_ID));
    GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, _count * sizeof(unsigned int), data, GL_STATIC_DRAW));
}

IndexBuffer::~IndexBuffer(){
    GLCall(glDeleteBuffers(1, &Renderer_ID));
}

void IndexBuffer::Bind() const{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Renderer_ID));
}

void IndexBuffer::Unbind() const{
    GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}


// VertexLayout
// -------------------------------------------------
VertexLayout::~VertexLayout(){
    Elements.clear();
}

template<>
void VertexLayout::Push<float>(unsigned int count){
    Elements.push_back({GL_FLOAT, count, GL_FALSE});
    Stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
};

template<>
void VertexLayout::Push<unsigned int>(unsigned int count){
    Elements.push_back({GL_UNSIGNED_INT, count, GL_FALSE});
    Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
};

template<>
void VertexLayout::Push<unsigned char>(unsigned int count){
    Elements.push_back({GL_BYTE, count, GL_TRUE});
    Stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
};


// VertexArray
// -------------------------------------------------
VertexArray::VertexArray(){
    GLCall(glGenVertexArrays(1, &Renderer_ID));
    GLCall(glBindVertexArray(Renderer_ID));
}

VertexArray::~VertexArray(){
    GLCall(glDeleteVertexArrays(1, &Renderer_ID));

}

void VertexArray::Bind() const{
    GLCall(glBindVertexArray(Renderer_ID));
}

void VertexArray::Unbind() const{
    GLCall(glBindVertexArray(0));
}

void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexLayout& layout){
    Bind();
    vb.Bind();
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;
    for (unsigned int i = 0; i < elements.size(); i++){
        const auto& element = elements[i];
        GLCall(glEnableVertexAttribArray(i));
        GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*)(uintptr_t)offset));
        offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
    }
}