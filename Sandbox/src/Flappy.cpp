#include <Hazel.h>
#include <Hazel/Core/EntryPoint.h>

#include "GameLayer.h"

class Flappy : public Hazel::Application
{
public:
    Flappy()
    {
        PushLayer(new GameLayer());
    }

    ~Flappy()
    {
    }
};

Hazel::Application* Hazel::CreateApplication()
{
    return new Flappy();
}