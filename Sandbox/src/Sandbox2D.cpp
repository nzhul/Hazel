#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Platform/OpenGL/OpenGLShader.h"

Sandbox2D::Sandbox2D()
        : Layer("Sandbox2D"), _CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
    _SquareVA = Hazel::VertexArray::Create();

    float squareVertices[5 * 4] = {
        -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    Hazel::Ref<Hazel::VertexBuffer> squareVB;
    squareVB.reset(Hazel::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
    squareVB->SetLayout({
        { Hazel::ShaderDataType::Float3, "a_Position" }
    });
    _SquareVA->AddVertexBuffer(squareVB);

    uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
    Hazel::Ref<Hazel::IndexBuffer> squareIB;
    squareIB.reset(Hazel::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
    _SquareVA->SetIndexBuffer(squareIB);

    _FlatColorShader = Hazel::Shader::Create("assets/shaders/FlatColor.glsl");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Hazel::Timestep deltaTime)
{
    // Update
    _CameraController.OnUpdate(deltaTime);

    // Render
    Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
    Hazel::RenderCommand::Clear();

    Hazel::Renderer::BeginScene(_CameraController.GetCamera());


    std::dynamic_pointer_cast<Hazel::OpenGLShader>(_FlatColorShader)->Bind();
    std::dynamic_pointer_cast<Hazel::OpenGLShader>(_FlatColorShader)->UploadUniformFloat4("u_Color", _SquareColor);

    Hazel::Renderer::Submit(_FlatColorShader, _SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

    Hazel::Renderer::EndScene();
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(_SquareColor));
    ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
    _CameraController.OnEvent(e);
}
