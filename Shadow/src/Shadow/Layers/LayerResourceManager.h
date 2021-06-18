#pragma once
#include "Shadow/Layers/Layer.h"
#include "Shadow/Resources/ResourceModel.h"
#include "Shadow/Resources/ResourceTexture.h"
#include "Shadow/Resources/ResourceCubemap.h"
#include "Shadow/Resources/ResourceProgram.h"
#include "Shadow/Renderer/FrameBufferObject.h"
#include "Shadow/Renderer/RenderBufferObjects.h"

NAMESPACE_BEGAN

class Resources: public Layer
{
public:
	Resources() {};
	~Resources() {};

	static Model* LoadModel(std::string path);
	static Model* GetQuad();
	static std::shared_ptr<Model> GetCube();
	static Texture* LoadTexture(std::string path);
	static Texture* CreateTextureFromArray(unsigned int* array, int width, int height, int layerCount = 2, int mipLevelCount = 1);
	static Texture* CreateTextureFromArray(std::vector<glm::vec3>& array, int width, int height, int layerCount = 2, int mipLevelCount = 1);
	static Texture* CreateEmptyTexture(int width, int height, int internalFormat, int format, int type, bool defineParameteri = true);
	static Program* CreateShader(std::string& vs, std::string& fs);

	static Cubemap* CreateCubemap();
	static Cubemap* CreateCubemapFromTexture(Texture* texture);
	static FBO* CreateFBO();
	static RBO* CreateRBO();
	static std::shared_ptr<Texture> GetNoTextureTexture();
	virtual void OnImGuiRender() override;
	virtual void OnMainTopBar() override;

public:
	virtual void Init() override;

private:
	void CreateNoTextureTexture();

private:

	static Resources* instance;

	std::vector<Model*> Models;
	std::vector<Texture*> textures;
	std::shared_ptr<Texture> noTextureTex;
	std::unique_ptr<Program> cubeToTexture;
	std::unique_ptr<Program> irradianceProgram;
	std::unique_ptr<Program> prefilterProgram;
	std::unique_ptr<Program> brdfProgram;

	std::shared_ptr<Model> cubeModel;
	std::shared_ptr<Model> quadModel;

	std::unique_ptr<FBO> bakeFBO;
	std::unique_ptr<RBO> bakeRBO;

	friend class OpenGLTextureCube;
	friend class Environment;
	friend class Renderer;
};

NAMESPACE_END