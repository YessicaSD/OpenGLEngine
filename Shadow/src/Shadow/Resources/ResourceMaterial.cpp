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
		vec3 TangentLightPos;
		vec3 TangentViewPos;
		vec3 TangentFragPos;
		} vs_out;

		out vec3 vNormal;

		void main()
		{
			//vNormal = mat3(transpose(inverse(Model))) * aNormal;  
			
			vs_out.FragPos = vec3(view * Model * vec4(aPos, 1.0));   
			vs_out.TexCoords = aUV;
			
			mat3 normalMatrix = transpose(inverse(mat3(view * Model)));
			vec3 T = normalize(normalMatrix * aTangent);
			vec3 N = normalize(normalMatrix * aNormal);
			T = normalize(T - dot(T, N) * N);
			vec3 B = cross(N, T);
			
			mat3 TBN = transpose(mat3(T, B, N));    
			vs_out.TangentLightPos = TBN * lightPos;
			vs_out.TangentViewPos  = TBN * viewPos;
			vs_out.TangentFragPos  = TBN * vs_out.FragPos;

			gl_Position =  projViewMatrix * Model * vec4(aPos, 1.0);
			vNormal = normalize(normalMatrix * aNormal);
		})";

	fs = R"(
	#version 330 core
	layout (location = 0) out vec3 gPosition;
	layout (location = 1) out vec3 gNormal;
	layout (location = 2) out vec4 gAlbedoSpec;		

	in VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	vec3 TangentLightPos;
	vec3 TangentViewPos;
	vec3 TangentFragPos;
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
		// Normal ===============
		vec3 normal = texture(normalTex, fs_in.TexCoords).rgb;
		// transform normal vector to range [-1,1]
		normal = normalize(normal * 2.0 - 1.0);  
		
		// get diffuse color
		vec3 color = texture(albedoTex, fs_in.TexCoords).rgb;
		// ambient
		vec3 ambient = 0.1 * color;
		
		// diffuse
		vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
		float diff = max(dot(lightDir, normal), 0.0);
		vec3 diffuse = diff * color;
		
		// specular
		vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
		vec3 reflectDir = reflect(-lightDir, normal);
		vec3 halfwayDir = normalize(lightDir + viewDir);  
		float spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0);
		vec3 specular = vec3(0.2) * spec;

		//gAlbedoSpec =  vec4(ambient + diffuse + specular, 1.0);
		gAlbedoSpec = texture(albedoTex, fs_in.TexCoords);
		//gAlbedoSpec = vec4(vec3(0.8),1.0);
		//gNormal = normal;
		gNormal = vNormal;
		gPosition = fs_in.FragPos;
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


