#pragma once

#include "Core.h"
#include "Events/Event.h"
#include <Hazel/Window.h>

namespace Hazel
{
    class HAZEL_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();
    private:
        std::unique_ptr<Window> _Window;
        bool _Running = true;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}