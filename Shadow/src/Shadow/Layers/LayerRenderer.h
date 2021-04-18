#pragma once
#include "Layer.h"

#include "Shadow/Resources/ResourceProgram.h"
#include "Shadow/Renderer/Buffer.h"
#include "Shadow/Renderer/VertexArray.h"

#include "Shadow/Renderer/RendererAPI.h"
#include "Shadow/Renderer/Camera.h"

NAMESPACE_BEGAN
class Model;
class Texture;

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
	Camera camera;
	int projViewUniform = -1;
	std::shared_ptr<Program> defaultProgram;
	std::shared_ptr<VertexArray> vertexArray;
	Model* model = nullptr;
	Texture* tex = nullptr;
	static RendererAPI* rendererAPI;
};


NAMESPACE_END