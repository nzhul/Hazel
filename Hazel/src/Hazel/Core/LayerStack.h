#pragma once

#include "Hazel/Core/Core.h"
#include "Hazel/Core/Layer.h"

#include <vector>

namespace Hazel {

    class LayerStack
    {
    public:
        LayerStack() = default;
        ~LayerStack();

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);
        void PopLayer(Layer* overlay);
        void PopOverlay(Layer* overlay);

        std::vector<Layer*>::iterator begin() { return _Layers.begin(); }
        std::vector<Layer*>::iterator end() { return _Layers.end(); }

    private:
        std::vector<Layer*> _Layers;
        unsigned int _LayerInsertIndex = 0;
    };
}