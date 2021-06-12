#include "swpch.h"
#include "ResourceMaterial.h"
#include "Shadow/Log.h"
#include "Shadow/Layers/LayerResourceManager.h"

NAMESPACE_BEGAN

Material::Material()
{
	vs = R"(
		#version 330 core

		struct Material {
			vec3 ambient;
			vec3 diffuse;
			vec3 specular;
			float shininess;
		}; 

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aNormal;
		layout (location = 2) in vec2 aUV;
		layout (location = 3) in vec3 aTangent;
		layout (location = 4) in vec3 aBitangent;

		uniform mat4 Model;
		uniform mat4 view;
		uniform mat4 projViewMatrix;
		uniform Material material;
		uniform vec3 lightPos;
		uniform vec3 viewPos;

		out VS_OUT {
		vec3 FragPos;
		vec2 TexCoords;
		mat3 TBN;
		} vs_out;

		out vec3 vNormal;

		void main()
		{
			vs_out.FragPos = vec3(view * Model * vec4(aPos, 1.0));   
			vs_out.TexCoords = aUV;
			
			vec3 T = normalize(vec3(Model * vec4(aTangent, 0.0)));
			vec3 N = normalize(vec3(Model * vec4(aNormal, 0.0)));
			T = normalize(T - dot(T, N) * N);
			vec3 B = cross(N, T);

			vs_out.TBN = mat3(T, B, N);

			gl_Position =  projViewMatrix * Model * vec4(aPos, 1.0);
			
		})";

	fs = R"(
	#version 330 core
	layout (location = 0) out vec3 gPosition;
	layout (location = 1) out vec3 gNormal;
	layout (location = 2) out vec4 gAlbedoSpec;		

	in VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	mat3 TBN;
	} fs_in;

	//out vec4 FragColor;

	uniform sampler2D albedoTex;
	uniform sampler2D normalTex;
	uniform sampler2D aoTex;
	uniform sampler2D roughnessTex;
	uniform sampler2D specularTex;
	
	in vec3 vNormal;

	void main()
	{
		vec3 tangentNormal = normalize(texture(normalTex, fs_in.TexCoords).xyz * 2.0 - 1.0);
		gNormal = normalize(fs_in.TBN * tangentNormal);		
		gAlbedoSpec = texture(albedoTex, fs_in.TexCoords);
		gPosition = fs_in.FragPos;
	})";

	program.reset(Shadow::Resources::CreateShader(vs, fs));
	Init();
}

Material::Material(std::string vs, std::string fs)
{
	program.reset(Shadow::Resources::CreateShader(vs, fs));
	Init();
}

void Material::UseMaterial()
{
	program->Bind();
	for (int i = 0; i < TextureType::MAX_TEXTURE; i++)
	{
		if(textures[i].use_count() > 0)
			textures[i]->Bind(i);
	}
}

void Material::SetTexture(TextureType::TextureType textureType, Texture* texture)
{
	switch (textureType)
	{
	case TextureType::ALBEDO:
		textures[TextureType::ALBEDO].reset(texture);
		break;
	case TextureType::NORMAL:
		textures[TextureType::NORMAL].reset(texture);
		break;
	case TextureType::AMBIENT_OCCLUSION:
		textures[TextureType::AMBIENT_OCCLUSION].reset(texture);
		break;
	case TextureType::ROUGHNESS:
		textures[TextureType::ROUGHNESS].reset(texture);
		break;
	case TextureType::SPECULAR:
		textures[TextureType::SPECULAR].reset(texture);
		break;
	default:
		SW_LOG_WARN("Could not set the texture");
		break;
	}
}

void Material::Init()
{
	program->Bind();
	program->UploadUniformInt("albedoTex", 0);
	program->UploadUniformInt("normalTex", 1);
	program->UploadUniformInt("aoTex", 2);
	program->UploadUniformInt("roughnessTex", 3);
	program->UploadUniformInt("specularTex", 4);

	//for (int i = 0; i < TextureType::MAX_TEXTURE; i++)
	//{
	//	textures[i] = Resources::GetNoTextureTexture();
	//}
}

NAMESPACE_END


