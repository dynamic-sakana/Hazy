#include "Hazy/LayerStack/LayerStack.h"
#include <hazy_pch.h>

namespace Hazy {

    LayerStack::LayerStack() {
        m_layerInsert = m_layers.begin();
    }

    LayerStack::~LayerStack() {
        for (Layer* layer : m_layers) {
            delete layer;
        }
    }

    /**
     * @brief 往这一堆层中添加一个层，添加到最前面
     * @param layer 新的层
     */
    void LayerStack::pushLayer(Layer* layer) {
        m_layerInsert = m_layers.emplace(m_layerInsert, layer);
    }

    /**
     * @brief 往这一系列层中添加一个层，添加到最末尾
     * @param overlay 新的层
     */
    void LayerStack::pushOverlay(Layer* overlay) {
        m_layers.emplace_back(overlay);
    }
    
    /**
     * @brief 删除一个层
     * @param layer 需要删除的层
     * @note 如果你删除的是当前的插入点，那么插入点将会被移到上一个层
     */
    void LayerStack::popLayer(Layer* layer) {
        auto it = std::find(m_layers.begin(), m_layers.end(), layer);
        if (it != m_layers.end()) {
            m_layerInsert = m_layers.erase(it);
            m_layerInsert--;
        }
    }
    
    /**
     * @brief 删除一个层
     * @param overlay 需要删除的层
     */
    void LayerStack::popOverlay(Layer* overlay) {
        auto it = std::find(m_layers.begin(), m_layers.end(), overlay);
        if (it != m_layers.end()) {
            m_layers.erase(it);
        }
    }

}