#include "Hazy/Renderer/Model.h"
#include "Hazy/Util/Log.h"
#include "Hazy/Renderer/Texture.h"
#include "Hazy/Renderer/Buffer.h"
#include "Hazy/Renderer/VertexArray.h"
#include "Hazy/Renderer/Context.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Hazy {
    /**
     * @brief 解析模型数据
     * @param context 上下文（此VertexBuffer属于哪一个上下文）
     * @param ai_mesh assimp模型数据
     * @param name VertexBuffer的名字
     * @return VertexArray&
     */
    VertexArray& ParseArray(Context& context, aiMesh* ai_mesh, const std::string& name);
    
    /**
     * @brief 解析模型数据
     * @param context 上下文（此材质属于哪一个上下文）
     * @param ai_mesh assimp模型数据
     * @param ai_scene assimp模型数据
     * @param name texture系列的名字
     * @return Material
     */
    Material ParseMaterial(Context& context, aiMesh* ai_mesh, const aiScene* ai_scene, const std::string& name);
    
    /**
     * @brief 加载纹理数据
     * @param context 上下文（此纹理属于哪一个上下文）
     * @param path 纹理文件路径
     * @param type 纹理数据类型
     * @param name 材质的名字
     * @return Texture2D& 加载出来的纹理
     */
    inline Texture2D& LoadTexture2D(Context& context, const std::string& path, TextureType type, const std::string& name);

    /**
     * @brief 解析网格数据
     * @param context 上下文（此网格属于哪一个上下文）
     * @param ai_mesh assimp网格数据
     * @param ai_scene assimp场景数据
     * @param name 网格的名字
     * @return Mesh& 解析出来的网格
     */
    inline Mesh& ParseMesh(Context& context, aiMesh* ai_mesh, const aiScene* ai_scene, const std::string& name);



    Model::Model(Context& context, const std::string& name, const std::string& path) {
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenNormals);
        if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
            Logger::LogError("Failed to load model: {}, because: {}", path, importer.GetErrorString());
            return;
        }
        processNode(context, scene->mRootNode, scene, name);
    }

    void Model::processNode(Context& context, aiNode* node, const aiScene* scene, const std::string& name) {
        for (size_t i = 0; i < node->mNumMeshes; i++) {
            aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
            m_meshes.push_back(ParseMesh(context, mesh, scene, name));
        }
        for (size_t i = 0; i < node->mNumChildren; i++) {
            processNode(context, node->mChildren[i], scene, name);
        }
    }


    Mesh& ParseMesh(Context& context, aiMesh* ai_mesh, const aiScene* ai_scene, const std::string& name) {
        return context.create<Mesh>(name, ParseArray(context, ai_mesh, name), ParseMaterial(context, ai_mesh, ai_scene, name));
    }

    VertexArray& ParseArray(Context& context, aiMesh* ai_mesh, const std::string& name) {
        VertexBufferLayout layout;
        if (ai_mesh->HasPositions()) {
            layout.addElement({ DataType::Float, 3, "a_Position" });
        }
        if (ai_mesh->HasNormals()) {
            layout.addElement({ DataType::Float, 3, "a_Normal" });
        }
        if (ai_mesh->HasTextureCoords(0)) {
            layout.addElement({ DataType::Float, 2, "a_TexCoord" });
        }

        uint32_t vertexLength = ai_mesh->mNumVertices * layout.getStride();
        float* vertices = new float[vertexLength];
        uint32_t indexLength = ai_mesh->mNumFaces * 3;
        uint32_t* indices = new uint32_t[indexLength];
        for (auto& element : layout.getElements()) {
            if (element.name == "a_Position" && ai_mesh->HasPositions()) {
                uint32_t offset = layout.getOffsetOf("a_Position");
                for (uint32_t i = 0; i < ai_mesh->mNumVertices; i++) {
                    vertices[i * layout.getStride() + offset + 0] = ai_mesh->mVertices[i].x;
                    vertices[i * layout.getStride() + offset + 1] = ai_mesh->mVertices[i].y;
                    vertices[i * layout.getStride() + offset + 2] = ai_mesh->mVertices[i].z;
                }
            }
            else if (element.name == "a_Normal" && ai_mesh->HasNormals()) {
                uint32_t offset = layout.getOffsetOf("a_Normal");
                for (uint32_t i = 0; i < ai_mesh->mNumVertices; i++) {
                    vertices[i * layout.getStride() + offset + 0] = ai_mesh->mNormals[i].x;
                    vertices[i * layout.getStride() + offset + 1] = ai_mesh->mNormals[i].y;
                    vertices[i * layout.getStride() + offset + 2] = ai_mesh->mNormals[i].z;
                }
            }
            else if (element.name == "a_TexCoord") {
                uint32_t offset = layout.getOffsetOf("a_TexCoord");
                for (uint32_t i = 0; i < ai_mesh->mNumVertices; i++) {
                    vertices[i * layout.getStride() + offset + 0] = ai_mesh->mTextureCoords[0][i].x;
                    vertices[i * layout.getStride() + offset + 1] = ai_mesh->mTextureCoords[0][i].y;
                }
            }
        }

        for (uint32_t i = 0; i < ai_mesh->mNumFaces; i++) {
            indices[i * 3 + 0] = ai_mesh->mFaces[i].mIndices[0];
            indices[i * 3 + 1] = ai_mesh->mFaces[i].mIndices[1];
            indices[i * 3 + 2] = ai_mesh->mFaces[i].mIndices[2];
        }

        context.create<VertexArray>(name)
            .addVertexBuffer(context.create<VertexBuffer>(name, vertices, vertexLength, layout, BufferUsage::StaticDraw))
            .setIndexBuffer(context.create<IndexBuffer>(name, indices, indexLength, BufferUsage::StaticDraw));

        delete[] vertices;
        delete[] indices;
        return context.get<VertexArray>(name);
    }


    Texture2D& LoadTexture2D(Context& context, const std::string& path, TextureType type, const std::string& name) {
        try {
            return context.get<Texture2D>(name);
        }
        catch (const std::out_of_range&) {
            return context.create<Texture2D>(name, path, type)
                .setFilter(TextureFilter::LinearMipmapLinear)
                .generateMipMap();
        }
    }

    Material ParseMaterial(Context& context, aiMesh* ai_mesh, const aiScene* ai_scene, const std::string& name) {
        aiMaterial* ai_material = ai_scene->mMaterials[ai_mesh->mMaterialIndex];
        size_t ambCount = ai_material->GetTextureCount(aiTextureType_AMBIENT);
        size_t difCount = ai_material->GetTextureCount(aiTextureType_DIFFUSE);
        size_t speCount = ai_material->GetTextureCount(aiTextureType_SPECULAR);
        size_t norCount = ai_material->GetTextureCount(aiTextureType_NORMALS);
        Material result;
        result.ambientMaps.reserve(ambCount);
        result.diffuseMaps.reserve(difCount);
        result.specularMaps.reserve(speCount);
        result.normalMaps.reserve(norCount);

        // 纹理的名字生成规则：<材质名字>_<纹理类型><序号>
        // 如：材质名字为"material1"，纹理类型为"diffuse"，序号为1，则纹理名字为"material1_dif1"
        auto generateTextureName =
            [](const std::string& name, aiTextureType type, uint32_t index) {
                std::stringstream ss;
                ss << name << "_";
                switch (type) {
                    case aiTextureType_DIFFUSE: ss << "dif"; break;
                    case aiTextureType_SPECULAR: ss << "spe"; break;
                    case aiTextureType_NORMALS: ss << "nor"; break;
                    case aiTextureType_AMBIENT: ss << "amb"; break;
                    default: ss << "unk"; break;
                }
                ss << index;
                return ss.str();
            };
            
        for (size_t i = 0; i < ambCount; i++) {
            aiString path;
            std::string textureName = generateTextureName(name, aiTextureType_AMBIENT, i);
            ai_material->GetTexture(aiTextureType_AMBIENT, i, &path);
            result.ambientMaps.push_back(&LoadTexture2D(context, path.C_Str(), TextureType::Ambient, textureName));
        }

        for (size_t i = 0; i < difCount; i++) {
            aiString path;
            std::string textureName = generateTextureName(name, aiTextureType_DIFFUSE, i);
            ai_material->GetTexture(aiTextureType_DIFFUSE, i, &path);
            result.diffuseMaps.push_back(&LoadTexture2D(context, path.C_Str(), TextureType::Diffuse, textureName));
        }

        for (size_t i = 0; i < speCount; i++) {
            aiString path;
            std::string textureName = generateTextureName(name, aiTextureType_SPECULAR, i);
            ai_material->GetTexture(aiTextureType_SPECULAR, i, &path);
            result.specularMaps.push_back(&LoadTexture2D(context, path.C_Str(), TextureType::Specular, textureName));
        }

        for (size_t i = 0; i < norCount; i++) {
            aiString path;
            std::string textureName = generateTextureName(name, aiTextureType_NORMALS, i);
            ai_material->GetTexture(aiTextureType_NORMALS, i, &path);
            result.normalMaps.push_back(&LoadTexture2D(context, path.C_Str(), TextureType::Normal, textureName));
        }

        return result;
    }

    
}