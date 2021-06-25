
#include <Shadow.h>
#include "imgui.h"
using namespace Shadow;

class Test : public Shadow::Layer
{
public:
	Test() : Layer("Test layer") {}

	void OnUpdate() override
	{
		if (Shadow::Input::IsKeyPressed(SW_KEY_0))
			SW_LOG_TRACE("0 IS PRESSED");
	}
	virtual void OnImGuiRender() override {
		ImGui::Begin("Hello");
		ImGui::End();
	};
};
class Sandbox : public Shadow::Application
{

public:
	Sandbox() 
	{
		model.reset(Resources::LoadModel("Assets/backpack/backpack.obj"));
		material.reset(new Material());
		material->SetTexture(TextureType::ROUGHNESS, Resources::LoadTexture("Assets/backpack/roughness.jpg"));
		material->SetTexture(TextureType::ALBEDO, Resources::LoadTexture("Assets/backpack/diffuse.jpg"));
		material->SetTexture(TextureType::NORMAL, Resources::LoadTexture("Assets/backpack/normal.png"));
		material->SetTexture(TextureType::METAL, Resources::LoadTexture("Assets/backpack/specular.jpg"));
		material->SetTexture(TextureType::AO, Resources::LoadTexture("Assets/backpack/ao.jpg"));

		gunModel.reset(Resources::LoadModel("Assets/Cerberus_by_Andrew_Maximov/Cerberus_LP.FBX"));
		materialGun.reset(new Material());
		materialGun->SetTexture(TextureType::ALBEDO, Resources::LoadTexture("Assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_A.tga"));
		materialGun->SetTexture(TextureType::NORMAL, Resources::LoadTexture("Assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_N.tga"));
		materialGun->SetTexture(TextureType::ROUGHNESS, Resources::LoadTexture("Assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_R.tga"));
		materialGun->SetTexture(TextureType::METAL, Resources::LoadTexture("Assets/Cerberus_by_Andrew_Maximov/Textures/Cerberus_M.tga"));

		Entity newEntity = Entity(gunModel, materialGun, "Gun");
		newEntity.SetScale(0.1);
		newEntity.SetRotation(glm::vec3(275, 0.0, 180.0));
		Renderer::PushEntity(newEntity);

		newEntity = Entity(model, material, "Backpack");
		newEntity.SetPosition(glm::vec3(-4.5, 0.0, 0.0));
		Renderer::PushEntity(newEntity);

		Renderer::PushLight(new PointLight({ 5.0,0.0,5.0 }));
		Renderer::PushLight(new PointLight({ 5.0,1.0,5.0 }));
		Renderer::PushLight(new PointLight({ 5.0,2.0,5.0 }));

		//sphere.reset(Resources::LoadModel("Assets/sphere/sphere.fbx"));		
		//rustedMaterial.reset(new Material());
		//rustedMaterial->SetTexture(TextureType::ROUGHNESS, Resources::LoadTexture("Assets/RustedIronMaterial/rustediron2_roughness.png"));
		//rustedMaterial->SetTexture(TextureType::ALBEDO, Resources::LoadTexture("Assets/RustedIronMaterial/rustediron2_basecolor.png"));
		//rustedMaterial->SetTexture(TextureType::NORMAL, Resources::LoadTexture("Assets/RustedIronMaterial/rustediron2_normal.png"));
		//rustedMaterial->SetTexture(TextureType::METAL, Resources::LoadTexture("Assets/RustedIronMaterial/rustediron2_metallic.png"));

		//newEntity = Entity(sphere, rustedMaterial, "rusted sphere");
		//newEntity.SetPosition(glm::vec3(4.5, 0.0, 0.0));
		//Renderer::PushEntity(newEntity);
	};
	~Sandbox() {};

private:

	std::shared_ptr<Model> model;
	std::shared_ptr<Model> gunModel;
	std::shared_ptr<Model> sphere;
	
	std::shared_ptr<Material> material;
	std::shared_ptr<Material> materialGun;
	std::shared_ptr<Material> rustedMaterial;

};

Shadow::Application* Shadow::CreateApplication()
{
	return new Sandbox();
}
