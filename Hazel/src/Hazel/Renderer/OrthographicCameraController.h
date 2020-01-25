#pragma once
#include "Hazel/Renderer/OrthographicCamera.h"
#include "Hazel/Core/Timestep.h"

#include "Hazel/Events/ApplicationEvent.h"
#include "Hazel/Events/MouseEvent.h"

namespace Hazel
{
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio, bool rotation = false);

        void OnUpdate(Timestep deltaTime);
        void OnEvent(Event& e);

        OrthographicCamera& GetCamera() { return _Camera; }
        const OrthographicCamera& GetCamera() const { return _Camera; }

        float GetZoomLevel() const { return _ZoomLevel; };
        void SetZoomLevel(float level) { _ZoomLevel = level; };
    private:
        bool OnMouseScrolled(MouseScrolledEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
    private:
        float _AspectRatio;
        float _ZoomLevel = 1.0f;
        OrthographicCamera _Camera;

        bool _Rotation = false;

        glm::vec3 _CameraPosition = { 0.0f, 0.0f, 0.0f };
        float _CameraRotation = 0.0f;
        float _CameraTranslationSpeed = 5.0f, _CameraRotationSpeed = 180.0f;
    };
}