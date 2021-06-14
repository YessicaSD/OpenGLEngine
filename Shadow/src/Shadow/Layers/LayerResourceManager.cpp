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
#include "Platform/OpenGL/OpenGLPrimitive.h"
#include "Platform/OpenGL/OpenGLProgram.h"
#include "Platform/OpenGL/OpenGLTextureCube.h"
#include "Platform/OpenGL/AllBuffers/OpenGLFBO.h"
#include "Platform/OpenGL/AllBuffers/OpenGLRBO.h"

#include "imgui.h"

NAMESPACE_BEGAN

Resources* Resources::instance = new Resources;

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
		aiProcess_FlipUVs | \
		0;

	const aiScene* scene = aiImportFile(path.c_str(), flags);

	if (scene == nullptr || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		SW_LOG_ERROR(path + " could not be imported");
		return resultModel;
	}

	resultModel = new Model();
	instance->Models.push_back(resultModel);

	resultModel->ProcessNode(scene->mRootNode, scene);

	return resultModel;
}

Model* Resources::GetQuad()
{
	switch (Renderer::GetRendererAPI())
	{
		case RendererAPI::RenderAPIType::NONE:	SW_CORE_ASSERT(false, "not render api using"); return nullptr; break;
		case RendererAPI::RenderAPIType::OPENGL: return OpenGLPrimitive::GetQuad();
	}

	return nullptr;
}

std::shared_ptr<Model> Resources::GetCube()
{
	if (instance->cubeModel.use_count() != 0)
		return instance->cubeModel;

	switch (Renderer::GetRendererAPI())
	{
		case RendererAPI::RenderAPIType::NONE:	SW_CORE_ASSERT(false, "not render api using"); return nullptr; break;
		case RendererAPI::RenderAPIType::OPENGL: instance->cubeModel.reset(OpenGLPrimitive::GetCube());
	}

	return nullptr;
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

Texture* Resources::CreateTextureFromArray(unsigned int* array, int width, int height, int layerCount, int mipLevelCount)
{
	switch (Renderer::GetRendererAPI())
	{
		case RendererAPI::RenderAPIType::NONE:	SW_CORE_ASSERT(false, "not render api using"); return nullptr; break;
		case RendererAPI::RenderAPIType::OPENGL: return new OpenGLTexture(array, width, height, layerCount, mipLevelCount); break;
	}

	return nullptr;
}

Texture* Resources::CreateTextureFromArray(std::vector<glm::vec3>& array, int width, int height, int layerCount, int mipLevelCount)
{
	switch (Renderer::GetRendererAPI())
	{
		case RendererAPI::RenderAPIType::NONE:	SW_CORE_ASSERT(false, "not render api using"); return nullptr; break;
		case RendererAPI::RenderAPIType::OPENGL: return new OpenGLTexture(array, width, height, layerCount, mipLevelCount); break;
	}

	return nullptr;
}

Texture* Resources::CreateEmptyTexture(int width, int height, int internalFormat, int format, int type)
{
	switch (Renderer::GetRendererAPI())
	{
		case RendererAPI::RenderAPIType::NONE:	SW_CORE_ASSERT(false, "not render api using"); return nullptr; break;
		case RendererAPI::RenderAPIType::OPENGL: return new OpenGLTexture(width, height, internalFormat, format, type); break;
	}

	return nullptr;
	
}

Program* Resources::CreateShader(std::string& vertexSource, std::string& fragmentSource)
{

	switch (Renderer::GetRendererAPI())
	{
	case RendererAPI::RenderAPIType::NONE:
		SW_CORE_ASSERT(false, "not render api using");
		return nullptr;
		break;

	case RendererAPI::RenderAPIType::OPENGL:
		return new OpenGLProgram(vertexSource, fragmentSource);
		break;

	default:
		return nullptr;
		break;
	}
}


Cubemap* Resources::CreateCubemap()
{
	switch (Renderer::GetRendererAPI())
	{
	case RendererAPI::RenderAPIType::NONE:	SW_CORE_ASSERT(false, "not render api using"); return nullptr; break;
	case RendererAPI::RenderAPIType::OPENGL: return new OpenGLTextureCube(); break;
	}

	return nullptr;
}

Cubemap* Resources::CreateCubemapFromTexture(Texture* texture)
{
	switch (Renderer::GetRendererAPI())
	{
		case RendererAPI::RenderAPIType::NONE:	SW_CORE_ASSERT(false, "not render api using"); return nullptr; break;
		case RendererAPI::RenderAPIType::OPENGL: return new OpenGLTextureCube(texture); break;
	}

	return nullptr;
}

FBO* Resources::CreateFBO()
{
	switch (Renderer::GetRendererAPI())
	{
		case RendererAPI::RenderAPIType::NONE:	SW_CORE_ASSERT(false, "not render api using"); return nullptr; break;
		case RendererAPI::RenderAPIType::OPENGL: return new OpenGLFBO(); break;
	}

	return nullptr;
}

RBO* Resources::CreateRBO()
{
	switch (Renderer::GetRendererAPI())
	{
		case RendererAPI::RenderAPIType::NONE:	SW_CORE_ASSERT(false, "not render api using"); return nullptr; break;
		case RendererAPI::RenderAPIType::OPENGL: return new OpenGLRBO(); break;
	}

	return nullptr;
}

std::shared_ptr<Texture> Resources::GetNoTextureTexture()
{
	return instance->noTextureTex;
}

void Resources::OnImGuiRender()
{
	ImGui::Begin("Resources");
	ImGui::End();
}

void Resources::OnMainTopBar()
{
	static bool open = true;
	if (ImGui::Button("Hey"))
	{

	}
	//ImGui::BeginTabItem("Resources", &open);
}

void Resources::Init()
{
	CreateNoTextureTexture();
	instance->irradianceProgram.reset(LoadProgram("Assets/Programs/Irradiance.glsl"));
	instance->cubeToTexture.reset(LoadProgram("Assets/Programs/BuildCubeMap.glsl"));
	instance->cubeToTexture->Bind();
	instance->cubeToTexture->UploadUniformInt("equirectangularMap", 0);
	GetCube();
	instance->bakeFBO.reset(CreateFBO());
	instance->bakeRBO.reset(CreateRBO());

}

void Resources::CreateNoTextureTexture()
{
	unsigned int value = 0;
	instance->noTextureTex.reset(CreateTextureFromArray(&value, 1, 1));
}

NAMESPACE_END


