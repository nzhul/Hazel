#include "hzpch.h"
#include "Platform/OpenGL/OpenGLBuffer.h"

#include <glad/glad.h>

namespace Hazel 
{
    ///////////////////////////////////////////////////////////
    ////////////////////// Vertex buffer //////////////////////
    ///////////////////////////////////////////////////////////

    OpenGLVertexBuffer::OpenGLVertexBuffer(float* vertices, uint32_t size)
    {
        HZ_PROFILE_FUNCTION();

        glCreateBuffers(1, &_RendererId);
        glBindBuffer(GL_ARRAY_BUFFER, _RendererId);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        HZ_PROFILE_FUNCTION();

        glDeleteBuffers(1, &_RendererId);
    }

    void OpenGLVertexBuffer::Bind() const
    {
        HZ_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, _RendererId);
    }

    void OpenGLVertexBuffer::UnBind() const
    {
        HZ_PROFILE_FUNCTION();

        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    ///////////////////////////////////////////////////////////
    ////////////////////// Index buffer ///////////////////////
    ///////////////////////////////////////////////////////////

    OpenGLIndexBuffer::OpenGLIndexBuffer(uint32_t* indices, uint32_t count)
        : _Count(count)
    {
        HZ_PROFILE_FUNCTION();

        glCreateBuffers(1, &_RendererId);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererId);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        HZ_PROFILE_FUNCTION();

        glDeleteBuffers(1, &_RendererId);
    }

    void OpenGLIndexBuffer::Bind() const
    {
        HZ_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _RendererId);
    }

    void OpenGLIndexBuffer::UnBind() const
    {
        HZ_PROFILE_FUNCTION();

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}