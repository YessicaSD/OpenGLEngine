#pragma once
#include "Layer.h"

#include "Shadow/Resources/ResourceProgram.h"
#include "Shadow/Renderer/Buffer.h"
#include "Shadow/Renderer/VertexArray.h"

#include "Shadow/Renderer/RendererAPI.h"
#include "Shadow/Renderer/Camera.h"
#include "Shadow/Resources/ResourceMaterial.h"
#include "Shadow/Renderer/Environment.h"
#include "Shadow/Renderer/FrameBufferObject.h"
#include "Shadow/Renderer/Light.h"

NAMESPACE_BEGAN
class Model;
class Texture;
class Cubemap;

enum RenderMethod
{
	FORWARD,
	DEFERRED,
};
enum RenderMode
{
	NORMAL = 0,
	ALBEDO,
	POSITION,
	DEPTH,
	FINAL,
};
class Renderer: public Layer
{
public:
	Renderer();
	~Renderer();

	static void BeginScene() ;
	static void EndScene();

	static inline RendererAPI::RenderAPIType GetRendererAPI() { return RendererAPI::GetAPI(); };
	inline static void SetClearColor(const glm::vec4& color) { rendererAPI->SetClearColor(color); };
	inline static void ClearScreen() { rendererAPI->ClearScreen(); };
	static void SetViewPort(int x, int y, int width, int height);
	static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
	virtual void OnImGuiRender() override;



public:
	virtual void Init() override;
	virtual void OnUpdate() override ;

private:
	//Init ==
	void InitSSAO();
	void InitDeferredProgram();
	void GenerateNoiseTexture();
	void InitBlurSSAO();
	void InitBrdf();

	//Update ==
	void ForwardRendering();
	void DeferredRendering();
	void GeometryPass();
	void SSAOPass();
	void LightingPass();
	void CameraUpdate();
	//Other ==
	void InitSkybox();

	void SendLights(std::shared_ptr<Program> program);

	std::vector<glm::vec3> GenerateKernelPoints(int number);

private:
	Camera camera;
	glm::vec3 lightPos = { 0.5,1.0,0.3 };
	RenderMethod renderMethod = DEFERRED;

	std::shared_ptr<Program> skyProgram;
	std::shared_ptr<Program> deferredProgram;
	std::unique_ptr<Program> ssaoProgram;
	std::unique_ptr<Program> ssaoBlurProgram;
	std::unique_ptr<Program> geometryPassProgram;
	std::unique_ptr<Program> brdfProgram;

	std::unique_ptr<Material> material;
	std::unique_ptr<Material> materialGun;
	std::vector<glm::vec3> kernelsPoint;

	std::unique_ptr<Model> model = nullptr;
	std::unique_ptr<Model> gunModel;
	Model* cube = nullptr;
	std::shared_ptr<Model> renderQuad = nullptr;

	Texture* tex = nullptr;
	Cubemap* skybox = nullptr;
	std::shared_ptr<Cubemap> skyboxHDR;
	std::shared_ptr<Texture> hdrTexture;
	std::unique_ptr<Texture> noiseTex;
	std::unique_ptr<Texture> gPosition;
	std::unique_ptr<Texture> gAlbedoSpec;
	std::unique_ptr<Texture> gNormal;
	std::unique_ptr<Texture> gData;
	std::unique_ptr<Texture> gDepth;
	std::unique_ptr<Texture> brdfLutTexture;
	std::unique_ptr<Texture> ssaoTex;
	std::unique_ptr<Texture> ssaoBlurTex;

	std::vector<Light*> lights;

	std::shared_ptr<Environment> environment;

	static RendererAPI* rendererAPI;

	std::unique_ptr<FBO> gFBO;
	unsigned int ssaoFBO;
	unsigned int ssaoBlurFBO;
	int renderMode = 0;
};


NAMESPACE_END