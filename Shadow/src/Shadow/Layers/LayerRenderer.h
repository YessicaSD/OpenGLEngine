#pragma once
#include "Layer.h"

#include "Shadow/Resources/ResourceProgram.h"
#include "Shadow/Renderer/Buffer.h"
#include "Shadow/Renderer/VertexArray.h"

#include "Shadow/Renderer/RendererAPI.h"
#include "Shadow/Renderer/Camera.h"
#include "Shadow/Resources/ResourceMaterial.h"


NAMESPACE_BEGAN
class Model;
class Texture;
class Cubemap;

class Renderer: public Layer
{
public:
	Renderer();
	~Renderer();
	static void BeginScene();
	static void EndScene();

	static inline RendererAPI::RenderAPIType GetRendererAPI() { return RendererAPI::GetAPI(); };
	inline static void SetClearColor(const glm::vec4& color) { rendererAPI->SetClearColor(color); };
	inline static void ClearScreen() { rendererAPI->ClearScreen(); };
	static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
	virtual void OnImGuiRender() override;

	virtual void OnUpdate() override ;
private:
	void CameraUpdate();
private:
	Camera camera;
	int projViewUniform = -1;
	std::shared_ptr<Program> defaultProgram;
	std::shared_ptr<Program> skyProgram;
	std::unique_ptr<Program> deferredProgram;
	std::shared_ptr<VertexArray> vertexArray;
	std::unique_ptr<Material> material;

	Model* model = nullptr;
	Model* cube = nullptr;
	std::unique_ptr<Model> renderQuad = nullptr;

	Texture* tex = nullptr;
	Cubemap* skybox = nullptr;
	static RendererAPI* rendererAPI;

	unsigned int gBuffer;
	unsigned int gPosition, gNormal, gAlbedoSpec, gDepth;
};


NAMESPACE_END