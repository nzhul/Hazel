#include "hzpch.h"
#include "Application.h"

#include "Hazel/Log.h"
#include "Hazel/Renderer/Renderer.h"

#include "Input.h"

namespace Hazel {

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application* Application::_Instance = nullptr;

    Application::Application()
    {
        HZ_CORE_ASSERT(!_Instance, "Application already exists!");
        _Instance = this;

        _Window = std::unique_ptr<Window>(Window::Create());
        _Window->SetEventCallback(BIND_EVENT_FN(OnEvent));

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
            for (Layer* layer : _LayerStack)
                layer->OnUpdate();

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
}