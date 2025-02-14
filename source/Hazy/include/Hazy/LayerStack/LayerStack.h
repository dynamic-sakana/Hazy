#pragma once
#include <hazy_pch.h>
#include "Hazy/LayerStack/Layer.h"
namespace Hazy {

    class LayerStack {
    public:
        LayerStack();
        virtual ~LayerStack();

        virtual void pushLayer(Layer* layer);
        virtual void pushOverlay(Layer* overlay);
        virtual void popLayer(Layer* layer);
        virtual void popOverlay(Layer* overlay);

        inline void onEvent(Event& e) {
            for (Layer* layer : m_layers) {
                if (layer->isEnabled()) {
                    layer->onEvent(e);
                }
            }
        }

        inline std::vector<Layer*>::iterator begin() { return m_layers.begin(); }
        inline std::vector<Layer*>::iterator end()   { return m_layers.end(); }

    private:
        std::vector<Layer*> m_layers;
        std::vector<Layer*>::iterator m_layerInsert;    // 这个迭代器始终指向这个vector的第一个元素
    };
    
}
