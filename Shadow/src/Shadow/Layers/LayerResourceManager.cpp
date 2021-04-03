#include "swpch.h"
#include "LayerResourceManager.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/cfileio.h"
#include "assimp/cimport.h"

NAMESPACE_BEGAN
LayerResourceManager::LayerResourceManager()
{
	LoadScene();
}

LayerResourceManager::~LayerResourceManager()
{
}

void LayerResourceManager::LoadScene()
{
	const aiScene* scene = aiImportFile("E:/Documents/GitHub/OpenGLEngine/Sandbox/Assets/warrior/warrior.fbx", aiProcessPreset_TargetRealtime_MaxQuality);

}

NAMESPACE_END


