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
#include "Shadow/Renderer/RenderBufferObjects.h"
#include "Shadow/Renderer/Light.h"
#include "Shadow/Scene/Entity.h"

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
	static void PushEntity(Entity entity);
	static void PushLight(Light* light);

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
	void InitHdrFBO();
	void InitGeometrypass();

	//Update ==
	void ForwardRendering();
	void DeferredRendering();
	void GeometryPass();
	void EnvironmentMap();
	void BloomPass();
	void SSAOPass();
	void LightingPass();
	void CameraUpdate();
	//Other ==
	void InitSkybox();

	void SendLights(std::shared_ptr<Program> program);

	std::vector<glm::vec3> GenerateKernelPoints(int number);

	//UI
	void EntitiesUI();

private:
	Camera camera;
	std::vector<Entity> entities;
	RenderMethod renderMethod = DEFERRED;

	std::shared_ptr<Program> skyProgram;
	std::shared_ptr<Program> deferredProgram;
	std::unique_ptr<Program> ssaoProgram;
	std::unique_ptr<Program> ssaoBlurProgram;
	std::unique_ptr<Program> geometryPassProgram;
	std::unique_ptr<Program> brdfProgram;
	std::unique_ptr<Program> blurBloomProgram;
	std::unique_ptr<Program> finalBloom;

	std::shared_ptr<Material> material;
	std::shared_ptr<Material> materialGun;
	std::vector<glm::vec3> kernelsPoint;

	std::shared_ptr<Model> cube;
	std::shared_ptr<Model> renderQuad;

	Texture* tex = nullptr;
	std::shared_ptr<Cubemap> skybox;
	std::shared_ptr<Cubemap> skyboxHDR;
	std::shared_ptr<Texture> hdrTexture;
	std::unique_ptr<Texture> noiseTex;
	std::unique_ptr<Texture> gPosition;
	std::unique_ptr<Texture> gAlbedoSpec;
	std::unique_ptr<Texture> gNormal;
	std::unique_ptr<Texture> gData;
	std::unique_ptr<Texture> gDepth;
	std::unique_ptr<Texture> brdfLutTexture;
	std::unique_ptr<Texture> depthbrdf;
	std::unique_ptr<Texture> ssaoTex;
	std::unique_ptr<Texture> ssaoBlurTex;
	
	std::unique_ptr<Texture> finalRender;
	std::unique_ptr<FBO> FBO_FinalRender;
	std::unique_ptr<RBO> RBO_FinalRender;


	std::vector<Light*> lights;

	std::vector<std::shared_ptr<Environment>> environments;
	int currentEnvironment = 0;
	static RendererAPI* rendererAPI;

	std::unique_ptr<FBO> gFBO;

	std::unique_ptr<FBO> brdfFBO;
	std::unique_ptr<FBO> hdrFBO;
	FBO* pingpongFBO[2];
	Texture* pingpongBuffer[2];
	std::unique_ptr<Texture> renderTex;
	std::unique_ptr<Texture> highlightsTex;


	unsigned int ssaoFBO;
	unsigned int ssaoBlurFBO;
	int renderMode = 0;
	int finalMode = 0;
	float bloomRange = 0.2;
	int bloomBlurRange = 1;
	bool SSAO = true;
	int skyboxIndex = 1;
	float bloomThreshold = 1.0;
	bool ssaoRangeCheck = true;
	float ssaoIntesity = 1.0;

	static Renderer* instance;
};


NAMESPACE_END