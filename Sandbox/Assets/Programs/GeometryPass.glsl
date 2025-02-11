#if defined(VERTEX_SHADER)

		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aNormal;
		layout (location = 2) in vec2 aUV;
		layout (location = 3) in vec3 aTangent;
		layout (location = 4) in vec3 aBitangent;

		uniform mat4 Model;
		uniform mat4 view;
		uniform mat4 projViewMatrix;
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

			//vs_out.oNormal = normalMatrix * aNormal;
			vs_out.oNormal = transpose(inverse(mat3(Model))) * aNormal;

			gl_Position =  projViewMatrix * Model * vec4(aPos, 1.0);
			
		};

#elif defined(FRAGMENT_SHADER)
	layout (location = 0) out vec3 gPosition;
	layout (location = 1) out vec3 gNormal;
	layout (location = 2) out vec4 gAlbedoSpec;		
	layout (location = 3) out vec4 gData;			


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
	uniform sampler2D aoTex;

	uniform int activeTextures[5];
	uniform vec3 color;
	uniform vec2 rmValue;
	
	in vec3 vNormal;

	void main()
	{
		gAlbedoSpec = vec4(color, 1.0);
		if(activeTextures[0] == 1)
			gAlbedoSpec = texture(albedoTex, fs_in.TexCoords);

		gNormal = normalize(fs_in.oNormal);	
		if(activeTextures[1] == 1)
		{
			vec3 texNormal = texture(normalTex, fs_in.TexCoords).rgb;
			texNormal = texNormal * 2.0 - 1.0;
			gNormal = normalize(fs_in.TBN * texNormal);		
		}
		
		gData.r = rmValue.x;
		if(activeTextures[2] == 1)
			gData.r = texture(roughnessTex, fs_in.TexCoords).r;

		gData.g = rmValue.y;
		if(activeTextures[3] == 1)	
		gData.g = texture(metalTex,fs_in.TexCoords).r;

		gData.b = 1.0;
		if(activeTextures[4] == 1)
			gData.b = texture(aoTex,fs_in.TexCoords).r;
			
		gPosition = fs_in.FragPos;
	};
#endif