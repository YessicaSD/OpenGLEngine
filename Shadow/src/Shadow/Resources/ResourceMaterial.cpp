#include "swpch.h"
#include "ResourceMaterial.h"

NAMESPACE_BEGAN

Material::Material()
{
	vs = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aNormal;
		layout (location = 2) in vec2 aUV;

		uniform mat4 projViewMatrix;

		out vec3 pos;
		out vec3 vNormal;
		out vec3 FragPos;
		out vec2 uv;

		void main()
		{
			uv = aUV;
			pos = aPos;
		    gl_Position =  projViewMatrix * vec4(aPos, 1.0);
			vNormal = vec3(projViewMatrix * vec4(aNormal,1.0));
			FragPos = aPos;
		})";

	fs = R"(
		#version 330 core
		out vec4 FragColor;

		uniform sampler2D u_Texture;
		in vec3 pos;
		in vec3 vNormal;
		in vec3 FragPos;
		in vec2 uv;

		void main()
		{
		  vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
		  vec3 lightPos = vec3(1.0,2.0,2.0);

		  // ambient
		  float ambientStrength = 0.1f;
		  vec3 ambient = ambientStrength * lightColor;

		  // Diffuse 
		  vec3 norm = normalize(vNormal);
		  vec3 lightDir = normalize(lightPos - FragPos);
		  float diff = max(dot(norm, lightDir), 0.0);
		  vec3 diffuse = diff * lightColor;

		  vec3 result = (ambient + diffuse) * texture(u_Texture, uv).xyz;
		  FragColor = texture(u_Texture, uv);

		})";

	program.reset(Shadow::CreateShader(vs, fs));
}

void Material::UseMaterial()
{
	program->Bind();
}

NAMESPACE_END


