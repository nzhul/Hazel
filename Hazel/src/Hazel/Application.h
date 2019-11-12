#pragma once

#include "Core.h"
#include "Events/Event.h"
#include "Hazel/Events/ApplicationEvent.h"

#include <Hazel/Window.h>

namespace Hazel
{
    class HAZEL_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);
    private:
        bool OnWindowClosed(WindowCloseEvent& e);

        std::unique_ptr<Window> _Window;
        bool _Running = true;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}