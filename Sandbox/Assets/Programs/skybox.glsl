
#ifdef VERTEX_SHADER
layout (location = 0) in vec3 aPos;

		out vec3 TexCoords;

		uniform mat4 projViewMatrix;
		

		void main()
		{
			TexCoords = aPos;
			gl_Position = (projViewMatrix * vec4(aPos, 1.0)).xyww;
		}
#endif

#ifdef FRAGMENT_SHADER
	layout (location = 0) out vec3 gPosition;
	layout (location = 1) out vec3 gNormal;
	layout (location = 2) out vec4 gAlbedoSpec;		
	layout (location = 3) out vec4 gData;			

		in vec3 TexCoords;

		uniform samplerCube skybox;
		
		void main()
		{    
			gPosition = TexCoords;
			gData.r = 1.0;
			gData.g = 0.0;
			gNormal = vec3(0.0);

			vec3 envColor = textureLod(skybox, TexCoords, 0.0).xyz;

			gAlbedoSpec = vec4(envColor, 0.0);
		}
#endif 