#include "swpch.h"
#include "LayerResourceManager.h"
#include "LayerRenderer.h"

#include "Shadow/Log.h"

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/cfileio.h"
#include "assimp/cimport.h"

#include "Platform/OpenGL/OpenGLTexture.h"


NAMESPACE_BEGAN

Resources* Resources::instance = new Resources;

Resources::Resources()
{
}

Resources::~Resources()
{
}

Model* Resources::LoadModel(std::string path)
{
	Model* resultModel = nullptr;
	unsigned flags = aiProcess_CalcTangentSpace | \
		aiProcess_GenSmoothNormals | \
		aiProcess_JoinIdenticalVertices | \
		aiProcess_ImproveCacheLocality | \
		aiProcess_LimitBoneWeights | \
		aiProcess_SplitLargeMeshes | \
		aiProcess_Triangulate | \
		aiProcess_GenUVCoords | \
		aiProcess_SortByPType | \
		aiProcess_FindDegenerates | \
		aiProcess_FindInvalidData | \
		aiProcess_FlipUVs | 
		0;

	const aiScene* scene = aiImportFile(path.c_str(), flags);

	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		SW_ERROR(path + " could not be imported");
		return resultModel;
	}

	resultModel = new Model();
	instance->Models.push_back(resultModel);

	resultModel->ProcessNode(scene->mRootNode, scene);

	return resultModel;
}

Texture* Resources::LoadTexture(std::string path)
{
	switch (Renderer::GetRendererAPI())
	{
		case RendererAPI::RenderAPIType::NONE:	SW_CORE_ASSERT(false, "not render api using"); return nullptr; break;
		case RendererAPI::RenderAPIType::OPENGL: return new OpenGLTexture(path); break;
	}

	return nullptr;
}

NAMESPACE_END


