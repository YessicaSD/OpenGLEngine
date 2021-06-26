
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
		
		material->SetTexture(TextureType::ALBEDO, Resources::LoadTexture("Assets/backpack/diffuse.jpg"));
		material->SetTexture(TextureType::NORMAL, Resources::LoadTexture("Assets/backpack/normal.png"));
		material->SetTexture(TextureType::METAL, Resources::LoadTexture("Assets/backpack/specular.jpg"));
		material->SetTexture(TextureType::AO, Resources::LoadTexture("Assets/backpack/ao.jpg"));
		material->SetTexture(TextureType::ROUGHNESS, Resources::LoadTexture("Assets/backpack/roughness.jpg"));

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

		Renderer::PushLight(new PointLight({ -13.0,0.0,5.0 }));
		Renderer::PushLight(new PointLight({ 17.0,1.0,5.0 }));
		Renderer::PushLight(new PointLight({ 5.0,23.0,5.0 }));

		sphere.reset(Resources::LoadModel("Assets/sphere/sphere.fbx"));		
		rustedMaterial.reset(new Material());
		rustedMaterial->SetRoughness(0);
		rustedMaterial->SetMetalness(1.0);
		newEntity = Entity(sphere, rustedMaterial, "metal sphere");
		newEntity.SetPosition(glm::vec3(4, 0.0, 0.0));
		Renderer::PushEntity(newEntity);

		std::shared_ptr<Material>  material1(new Material);
		material1->SetTexture(TextureType::ALBEDO, Resources::LoadTexture("Assets/PBRMaterials/antique-grate1-unity/antique-grate1-albedo.png"));
		material1->SetTexture(TextureType::AO, Resources::LoadTexture("Assets/PBRMaterials/antique-grate1-unity/antique-grate1-ao.png"));
		material1->SetTexture(TextureType::NORMAL, Resources::LoadTexture("Assets/PBRMaterials/antique-grate1-unity/antique-grate1-normal-ogl.png"));
		material1->SetRoughness(0.4);
		material1->SetMetalness(1.0);
		newEntity = Entity(sphere, material1, "antique sphere");
		newEntity.SetPosition(glm::vec3(6, 0.0, 0.0));
		Renderer::PushEntity(newEntity);

		std::shared_ptr<Material>  material2(new Material);
		material2->SetRoughness(0.3);
		material2->SetMetalness(0.1);
		material2->SetTexture(TextureType::ALBEDO, Resources::LoadTexture("Assets/PBRMaterials/dirty-wicker/dirty-wicker-weave1-albedo.png"));
		material2->SetTexture(TextureType::AO, Resources::LoadTexture("Assets/PBRMaterials/dirty-wicker/dirty-wicker-weave1-ao.png"));
		material2->SetTexture(TextureType::NORMAL, Resources::LoadTexture("Assets/PBRMaterials/dirty-wicker/dirty-wicker-weave1-normal-dx.png"));
		//material2->SetTexture(TextureType::ROUGHNESS, Resources::LoadTexture("Assets/PBRMaterials/dirty-wicker/dirty-wicker-weave1-roughness.png"));
		newEntity = Entity(sphere, material2, "dirty wicker sphere");
		newEntity.SetPosition(glm::vec3(8, 0.0, 0.0));
		Renderer::PushEntity(newEntity);

		std::shared_ptr<Material>  material3(new Material);
		material3->SetTexture(TextureType::ALBEDO, Resources::LoadTexture("Assets/PBRMaterials/ornate-celtic-gold-bl/ornate-celtic-gold-albedo.png"));
		material3->SetTexture(TextureType::AO, Resources::LoadTexture("Assets/PBRMaterials/ornate-celtic-gold-bl/ornate-celtic-gold-ao.png"));
		material3->SetTexture(TextureType::NORMAL, Resources::LoadTexture("Assets/PBRMaterials/ornate-celtic-gold-bl/ornate-celtic-gold-normal-ogl.png"));
		material3->SetTexture(TextureType::ROUGHNESS, Resources::LoadTexture("Assets/PBRMaterials/ornate-celtic-gold-bl/ornate-celtic-gold-roughness.png"));
		material3->SetTexture(TextureType::METAL, Resources::LoadTexture("Assets/PBRMaterials/ornate-celtic-gold-bl/ornate-celtic-gold-metallic.png"));
		newEntity = Entity(sphere, material3, "ornate-celtic sphere");
		newEntity.SetPosition(glm::vec3(10, 0.0, 0.0));
		Renderer::PushEntity(newEntity);

		std::shared_ptr<Material>  material4(new Material);
		material4->SetTexture(TextureType::ALBEDO, Resources::LoadTexture("Assets/PBRMaterials/plasticpattern/plasticpattern1-albedo.png"));
		material4->SetTexture(TextureType::NORMAL, Resources::LoadTexture("Assets/PBRMaterials/plasticpattern/plasticpattern1-normal2-unity2b.png"));
		material4->SetRoughness(0.6);
		material4->SetMetalness(0.0);
		newEntity = Entity(sphere, material4, "plastic sphere");
		newEntity.SetPosition(glm::vec3(12, 0.0, 0.0));
		Renderer::PushEntity(newEntity);

		std::shared_ptr<Material>  material5(new Material);
		material5->SetTexture(TextureType::ALBEDO, Resources::LoadTexture("Assets/PBRMaterials/rock/rock_vstreaks_Base_Color.png"));
		material5->SetTexture(TextureType::AO, Resources::LoadTexture("Assets/PBRMaterials/rock/rock_vstreaks_Ambient_Occlusion.png"));
		material5->SetTexture(TextureType::NORMAL, Resources::LoadTexture("Assets/PBRMaterials/rock/rock_vstreaks_Normal-unity.png"));
		material5->SetRoughness(1.0);
		material5->SetMetalness(0.0);
		newEntity = Entity(sphere, material5, "rock sphere");
		newEntity.SetPosition(glm::vec3(14, 0.0, 0.0));
		Renderer::PushEntity(newEntity);


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
