#pragma once

#include "Hazel/Renderer/VertexArray.h"

namespace Hazel
{
    class OpenGLVertexArray : public VertexArray
    {
    public:
        OpenGLVertexArray();
        virtual ~OpenGLVertexArray();

        virtual void Bind() const override;
        virtual void UnBind() const override;

        virtual void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer) override;
        virtual void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer) override;

        virtual const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return _VertexBuffers; }
        virtual const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return _IndexBuffer; }

    private:
        uint32_t _RendererId;
        std::vector<std::shared_ptr<VertexBuffer>> _VertexBuffers;
        std::shared_ptr<IndexBuffer> _IndexBuffer;
    };
}