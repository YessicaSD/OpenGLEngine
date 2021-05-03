#pragma once
#include "Resource.h"
#include "ResourceMesh.h"

struct aiNode;
struct aiScene;
struct aiMesh;

NAMESPACE_BEGAN
class Model : public Resource
{
public:
	void Draw();
	inline int GetNumberOfMeshes() { return meshes.size(); };
	std::vector<Mesh> GetMeshes() { return meshes; };
	void AddMesh(Mesh mesh) { meshes.push_back(mesh); };
private:
	std::vector<Mesh> meshes;
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	friend class Resources;
};

NAMESPACE_END