#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

template<typename Fn>

class Timer
{
public:
    Timer(const char* name, Fn&& func)
        : _Name(name), _Func(func), _Stopped(false)
    {
        _StartTimepoint = std::chrono::high_resolution_clock::now();
    }

    ~Timer()
    {
        if (!_Stopped)
        {
            Stop();
        }
    }

    void Stop()
    {
        auto endTimepoint = std::chrono::high_resolution_clock::now();

        long long start = std::chrono::time_point_cast<std::chrono::microseconds>(_StartTimepoint).time_since_epoch().count();
        long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

        _Stopped = true;

        float duration = (end - start) * 0.001f;

        _Func({_Name, duration});
    }

private:
    const char* _Name;
    Fn _Func;
    std::chrono::time_point<std::chrono::steady_clock> _StartTimepoint;
    bool _Stopped;
};

#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { _ProfileResults.push_back(profileResult); })

Sandbox2D::Sandbox2D()
        : Layer("Sandbox2D"), _CameraController(1280.0f / 720.0f, true)
{
}

void Sandbox2D::OnAttach()
{
    _CheckerboardTexture = Hazel::Texture2D::Create("assets/textures/Checkerboard.png");
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Hazel::Timestep deltaTime)
{
    PROFILE_SCOPE("Sandbox2D::OnUpdate");

    // Update
    {
        PROFILE_SCOPE("CameraController::OnUpdate");
        _CameraController.OnUpdate(deltaTime);
    }

    // Render
    {
        PROFILE_SCOPE("Renderer Prep");
        Hazel::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
        Hazel::RenderCommand::Clear();
    }

    {
        PROFILE_SCOPE("Renderer Draw");
        Hazel::Renderer2D::BeginScene(_CameraController.GetCamera());
        Hazel::Renderer2D::DrawQuad({ -1.0f, 0.0f}, {0.8f, 0.8f}, {0.8f, 0.2f, 0.3f, 1.0f});
        Hazel::Renderer2D::DrawQuad({ 0.5f, -0.5f}, {0.5f, 0.75f}, {0.2f, 0.3f, 0.8f, 1.0f});
        Hazel::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, {10.0f, 10.0f}, _CheckerboardTexture);
        Hazel::Renderer2D::EndScene();
    }
}

void Sandbox2D::OnImGuiRender()
{
    ImGui::Begin("Settings");
    ImGui::ColorEdit4("Square Color", glm::value_ptr(_SquareColor));

    for (auto& result : _ProfileResults)
    {
        char label[50];
        strcpy(label, "%.3fms  ");
        strcat(label, result.Name);
        ImGui::Text(label, result.Time);
    }

    _ProfileResults.clear();

    ImGui::End();
}

void Sandbox2D::OnEvent(Hazel::Event& e)
{
    _CameraController.OnEvent(e);
}
