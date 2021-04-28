#include "swpch.h"
#include "ResourceMaterial.h"

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

		void main()
		{

			uv = aUV;
			pos = aPos;
			mat4 model = mat4(1.0);
		    gl_Position =  projViewMatrix * vec4(aPos, 1.0);
			vNormal = mat3(transpose(inverse(Model))) * aNormal;  
			FragPos = aPos;

		})";

	fs = R"(
	#version 330 core
	//layout (location = 0) out vec3 gPosition;
	//layout (location = 1) out vec3 gNormal;
	//layout (location = 2) out vec4 gAlbedoSpec;		


	out vec4 FragColor;

	uniform sampler2D u_Texture;
	in vec3 pos;
	in vec3 vNormal;
	in vec3 FragPos;
	in vec2 uv;

	void main()
	{

	  //gPosition = FragColor;
	  //gNormal = normalize(vNormal);
	  //gAlbedoSpec = texture(u_Texture, uv);

	  vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
	  vec3 lightPos = vec3(0.0,5.0,1.0);

	  // ambient
	  float ambientStrength = 0.1f;
	  vec3 ambient = ambientStrength * lightColor;

	  // Diffuse 
	  vec3 norm = normalize(vNormal);
	  vec3 lightDir = normalize(lightPos - FragPos);

	  float diff = max(dot(norm, lightDir), 0.0);
	  vec3 diffuse = diff * lightColor;

	  vec3 result = (ambient + diffuse) * texture(u_Texture, uv).xyz;
	  FragColor = vec4(result,1.0);

	})";

	program.reset(Shadow::CreateShader(vs, fs));
}

Material::Material(std::string vs, std::string fs)
{
	program.reset(Shadow::CreateShader(vs, fs));
}

void Material::UseMaterial()
{
	program->Bind();
}

NAMESPACE_END


