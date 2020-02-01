#pragma once
#include "Hazel.h"

class Sandbox2D : public Hazel::Layer
{
public:
    Sandbox2D();
    virtual ~Sandbox2D() = default;
    virtual void OnAttach() override;
    virtual void OnDetach() override;

    void OnUpdate(Hazel::Timestep deltaTime) override;
    virtual void OnImGuiRender() override;
    void OnEvent(Hazel::Event& e) override;

private:
        Hazel::OrthographicCameraController _CameraController;

        // Temp
        Hazel::Ref<Hazel::VertexArray> _SquareVA;
        Hazel::Ref<Hazel::Shader> _FlatColorShader;
        Hazel::Ref<Hazel::Texture2D> _CheckerboardTexture;

        struct ProfileResult
        {
            const char* Name;
            float Time;
        };

        std::vector<ProfileResult> _ProfileResults;

        glm::vec4 _SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
};