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

    Hazel::Renderer2D::BeginScene(_CameraController.GetCamera());
    Hazel::Renderer2D::DrawQuad({0.0f, 0.0f}, {1.0f, 1.0f}, {0.8f, 0.2f, 0.3f, 1.0f});
    Hazel::Renderer2D::EndScene();

    //std::dynamic_pointer_cast<Hazel::OpenGLShader>(_FlatColorShader)->Bind();
    //std::dynamic_pointer_cast<Hazel::OpenGLShader>(_FlatColorShader)->UploadUniformFloat4("u_Color", _SquareColor);
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