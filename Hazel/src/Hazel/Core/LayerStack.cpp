#include "hzpch.h"
#include "Hazel/Core/LayerStack.h"

namespace Hazel {

    LayerStack::~LayerStack()
    {
        for (Layer* layer : _Layers)
        {
            layer->OnDetach();
            delete layer;
        }

    }

    void LayerStack::PushLayer(Layer* layer)
    {
        _Layers.emplace(_Layers.begin() + _LayerInsertIndex, layer);
        _LayerInsertIndex++;
    }

    void LayerStack::PushOverlay(Layer* overlay)
    {
        _Layers.emplace_back(overlay);
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(_Layers.begin(), _Layers.begin() + _LayerInsertIndex, layer);
        if (it != _Layers.begin() + _LayerInsertIndex)
        {
            layer->OnDetach();
            _Layers.erase(it);
            _LayerInsertIndex--;
        }
    }

    void LayerStack::PopOverlay(Layer* overlay)
    {
        auto it = std::find(_Layers.begin() + _LayerInsertIndex, _Layers.end(), overlay);
        if (it != _Layers.end())
        {
            overlay->OnDetach();
            _Layers.erase(it);
        }
    }
}