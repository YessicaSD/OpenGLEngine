#include "swpch.h"
#include "ResourceModel.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/cfileio.h"
#include "assimp/cimport.h"
#include "Shadow/Layers/LayerRenderer.h"

NAMESPACE_BEGAN

void Model::Draw()
{
    for (auto mesh = meshes.begin(); mesh != meshes.end(); mesh++)
    {
        Renderer::Submit(mesh->vertexArray);
    }
}

void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
       
        Mesh rmesh = ProcessMesh(mesh, scene);
        aiMatrix4x4 transform = node->mTransformation;
        rmesh.transform = glm::mat4(transform.a1, transform.a2, transform.a3, transform.a4,
                                    transform.b1, transform.b2, transform.b3, transform.b4,
                                    transform.c1, transform.c2, transform.c3, transform.c4, 
                                    transform.d1, transform.d2, transform.d3, transform.d4) ;
        
        meshes.push_back(rmesh);
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        ProcessNode(node->mChildren[i], scene);
    }
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        vertex.position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        vertex.normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        vertex.tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
        vertex.bitangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);

        if (mesh->mTextureCoords[0])
            vertex.texCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);

        vertices.push_back(vertex);
    }

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }
    Mesh rmesh = Mesh(vertices, indices);
    rmesh.name = mesh->mName.C_Str();
    return rmesh;

}


NAMESPACE_END

