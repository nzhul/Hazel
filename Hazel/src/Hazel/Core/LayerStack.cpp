#include "hzpch.h"
#include "LayerStack.h"

namespace Hazel {

    LayerStack::LayerStack()
    {
    }

    LayerStack::~LayerStack()
    {
        for (Layer* layer : _Layers)
            delete layer;
    }

    void LayerStack::PushLayer(Layer* layer)
    {
        _Layers.emplace(_Layers.begin() + _LayerInsertIndex, layer);
        _LayerInsertIndex++;
        layer->OnAttach();
    }

    void LayerStack::PushOverlay(Layer* overlay)
    {
        _Layers.emplace_back(overlay);
        overlay->OnAttach();
    }

    void LayerStack::PopLayer(Layer* layer)
    {
        auto it = std::find(_Layers.begin(), _Layers.end(), layer);
        if (it != _Layers.end())
        {
            _Layers.erase(it);
            _LayerInsertIndex--;
            layer->OnDetach();
        }
    }

    void LayerStack::PopOverlay(Layer* overlay)
    {
        auto it = std::find(_Layers.begin(), _Layers.end(), overlay);
        if (it != _Layers.end())
        {
            _Layers.erase(it);
            overlay->OnDetach();
        }
    }
}