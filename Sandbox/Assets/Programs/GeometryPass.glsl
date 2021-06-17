#if defined(VERTEX_SHADER)

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aNormal;
		layout (location = 2) in vec2 aUV;
		layout (location = 3) in vec3 aTangent;
		layout (location = 4) in vec3 aBitangent;

		uniform mat4 Model;
		uniform mat4 view;
		uniform mat4 projViewMatrix;
		uniform vec3 lightPos;
		uniform vec3 viewPos;

		out VS_OUT {
		vec3 FragPos;
		vec2 TexCoords;
		mat3 TBN;
		vec3 oNormal;
		} vs_out;

		out vec3 vNormal;

		void main()
		{
			vs_out.FragPos = vec3(Model * vec4(aPos, 1.0));   
			vs_out.TexCoords = aUV;
						vec3 T = normalize(vec3(Model * vec4(aTangent, 0.0)));
			vec3 N = normalize(vec3(Model * vec4(aNormal, 0.0)));
			T = normalize(T - dot(T, N) * N);
			vec3 B = cross(N, T);

			vs_out.TBN = mat3(T, B, N);


			vs_out.oNormal = aNormal;

			gl_Position =  projViewMatrix * Model * vec4(aPos, 1.0);
			
		};

#elif defined(FRAGMENT_SHADER)
	layout (location = 0) out vec3 gPosition;
	layout (location = 1) out vec3 gNormal;
	layout (location = 2) out vec4 gAlbedoSpec;		
	layout (location = 3) out vec4 gRoughness;		
	layout (location = 4) out vec4 gMetal;		


	in VS_OUT {
	vec3 FragPos;
	vec2 TexCoords;
	mat3 TBN;
	vec3 oNormal;
	} fs_in;

	uniform sampler2D albedoTex;
	uniform sampler2D normalTex;
	uniform sampler2D roughnessTex;
	uniform sampler2D metalTex;
	
	in vec3 vNormal;

	void main()
	{
		vec3 texNormal = texture(normalTex, fs_in.TexCoords).rgb;
		texNormal = texNormal * 2.0 - 1.0;
		gNormal = normalize(fs_in.TBN * texNormal);		
		//gNormal = normalize(fs_in.oNormal);		
		gAlbedoSpec = texture(albedoTex, fs_in.TexCoords);
		gRoughness = texture(roughnessTex, fs_in.TexCoords);
		gMetal = texture(metalTex,fs_in.TexCoords);
		gPosition = fs_in.FragPos;
	};
#endif