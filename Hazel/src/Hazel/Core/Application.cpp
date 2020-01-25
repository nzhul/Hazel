#include "hzpch.h"
#include "Application.h"

#include "Hazel/Core/Log.h"
#include "Hazel/Renderer/Renderer.h"

#include "Input.h"

#include <glfw/glfw3.h>

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::_Instance = nullptr;

    Application::Application()
    {
        HZ_CORE_ASSERT(!_Instance, "Application already exists!");
        _Instance = this;

        _Window = std::unique_ptr<Window>(Window::Create());
        _Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

        Renderer::Init();

        _ImGuiLayer = new ImGuiLayer();
        PushOverlay(_ImGuiLayer);
    }

    void Application::PushLayer(Layer* layer)
    {
        _LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer* layer)
    {
        _LayerStack.PushOverlay(layer);
    }

    void Application::OnEvent(Event& e) 
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClosed));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));

        for (auto it = _LayerStack.end(); it != _LayerStack.begin(); )
        {
            (*--it)->OnEvent(e);
            if (e.Handled)
                break;
        }
    }

    void Application::Run()
    {
        while (_Running)
        {
            float time = (float)glfwGetTime(); // Platform::GetTime
            Timestep timestep = time - _LastFrameTime;
            _LastFrameTime = time;

            if (!_Minimized)
            {
                for (Layer* layer : _LayerStack)
                    layer->OnUpdate(timestep);
            }

            _ImGuiLayer->Begin();
            for (Layer* layer : _LayerStack)
                layer->OnImGuiRender();
            _ImGuiLayer->End();

            _Window->OnUpdate();
        }
    }

    bool Application::OnWindowClosed(WindowCloseEvent& e)
    {
        _Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            _Minimized = true;
            return false;
        }

        _Minimized = false;
        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return false;
    }
}