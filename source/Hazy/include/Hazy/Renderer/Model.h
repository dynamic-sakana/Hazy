#pragma once
#include <hazy_pch.h>

struct aiNode;
struct aiScene;

namespace Hazy {

    class VertexArray;
    class Context;

    template<int dimension>
    class Texture;

    using Texture2D = Texture<2>;
    using Texture3D = Texture<3>;

    struct Material {
        std::vector<Texture2D*> ambientMaps;
        std::vector<Texture2D*> diffuseMaps;
        std::vector<Texture2D*> specularMaps;
        std::vector<Texture2D*> normalMaps;
    };

    struct Mesh {
        Mesh(VertexArray& vertexArray, const Material& material)
            : vertexArray(&vertexArray), material(material) { }
        ~Mesh() = default;

        VertexArray* vertexArray;
        Material material;
    };

    /**
     * @brief 模型类，用于加载模型文件
     * @note 由于在创建它的时候必须要创建VertexBuffer等其他 GPU 资源，所以构造函数的参数中有一个 Context，表示这个模型属于哪一个上下文
     * @warning 请勿直接new一个Model，而是使用Context的Create方法加载模型
     */
    class Model {
    public:
        Model(Context& context, const std::string& name, const std::string& path);
        ~Model() = default;

        inline const std::vector<Mesh>& getMeshes() const { return m_meshes; }

    private:
        void processNode(Context& context, aiNode* node, const aiScene* scene, const std::string& name);
        
        std::vector<Mesh> m_meshes;
    };

}