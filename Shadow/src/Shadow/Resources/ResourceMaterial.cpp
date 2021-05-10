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

		uniform mat4 Model;
		uniform mat4 projViewMatrix;
		uniform Material material;

		out vec3 pos;
		out vec3 vNormal;
		out vec3 FragPos;
		out vec2 uv;

		out VS_OUT {
		vec3 FragPos;
		vec2 TexCoords;
		vec3 TangentLightPos;
		vec3 TangentViewPos;
		vec3 TangentFragPos;
		} vs_out;

		void main()
		{

			uv = aUV;
			pos = aPos;
			mat4 model = mat4(1.0);
		    gl_Position =  projViewMatrix * vec4(aPos, 1.0);
			vNormal = mat3(transpose(inverse(Model))) * aNormal;  
			FragPos = vec3(model * vec4(aPos,1.0));

		})";

	fs = R"(
	#version 330 core
	layout (location = 0) out vec3 gPosition;
	layout (location = 1) out vec3 gNormal;
	layout (location = 2) out vec4 gAlbedoSpec;		


	//out vec4 FragColor;

	uniform sampler2D albedoTex;
	uniform sampler2D normalTex;
	uniform sampler2D aoTex;
	uniform sampler2D roughnessTex;
	uniform sampler2D specularTex;
	
	uniform vec3 lightPos;

	in vec3 pos;
	in vec3 vNormal;
	in vec3 FragPos;
	in vec2 uv;

	void main()
	{

	  gPosition = FragPos;
		// Normal ===============
		vec3 normal = texture(normalTex, uv).rgb;
		// transform normal vector to range [-1,1]
		normal = normalize(normal * 2.0 - 1.0);  
		

	  gAlbedoSpec.xyz = texture(albedoTex, uv).xyz;
	  gAlbedoSpec.a = 1.0;

	  vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	  //vec3 lightPos = vec3(0.5,1.0,0.3);

	  // ambient
	  float ambientStrength = 0.5f;
	  vec3 ambient = ambientStrength * lightColor;

	  // Diffuse 
	  vec3 norm = normal;
	  vec3 lightDir = normalize(lightPos - FragPos);

	  float diff = max(dot(norm, lightDir), 0.0);
	  vec3 diffuse = diff * lightColor;

	  vec3 result = (ambient + diffuse) * texture(albedoTex, uv).xyz;
	  //FragColor = vec4(result,1.0);
	gAlbedoSpec =  vec4(result,1.0);
	gNormal = normal;
	})";

	program.reset(Shadow::CreateShader(vs, fs));
	Init();
}

Material::Material(std::string vs, std::string fs)
{
	program.reset(Shadow::CreateShader(vs, fs));
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


