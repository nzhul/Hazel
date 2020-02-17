#include "GameLayer.h"
#include <imgui/imgui.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Hazel;

GameLayer::GameLayer()
    : Layer("GameLayer")
{
    auto& window = Application::Get().GetWindow();
    CreateCamera(window.GetWidth(), window.GetHeight());

    //Random::Init();
}

void GameLayer::OnAttach()
{
    _Level.Init();
}

void GameLayer::OnDetach()
{
}

void GameLayer::OnUpdate(Hazel::Timestep ts)
{

}

void GameLayer::OnImGuiRender()
{

}

void GameLayer::OnEvent(Hazel::Event& e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowResizeEvent>(HZ_BIND_EVENT_FN(GameLayer::OnWindowResize));
    dispatcher.Dispatch<MouseButtonPressedEvent>(HZ_BIND_EVENT_FN(GameLayer::OnMouseButtonPressed));
}

bool GameLayer::OnMouseButtonPressed(Hazel::MouseButtonPressedEvent& e)
{
    return false;
}

bool GameLayer::OnWindowResize(Hazel::WindowResizeEvent& e)
{
    CreateCamera(e.GetWidth(), e.GetHeight());
    return false;
}

void GameLayer::CreateCamera(uint32_t width, uint32_t height)
{
    float aspectRatio = (float)width / (float)height;

    float camWidth = 8.0f;
    float bottom = -camWidth;
    float top = camWidth;
    float left = bottom * aspectRatio;
    float right = top * aspectRatio;
    m_Camera = CreateScope<OrthographicCamera>(left, right, bottom, top);
}
