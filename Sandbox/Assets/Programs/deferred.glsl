#ifdef VERTEX_SHADER
		layout (location = 0) in vec3 aPos;
		layout (location = 1) in vec3 aNormal;
		layout (location = 2) in vec2 aTexCoords;

		out vec2 TexCoords;

		void main()
		{
			TexCoords = aTexCoords;
			gl_Position = vec4(aPos, 1.0);
		}
#endif

#ifdef FRAGMENT_SHADER

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

		in vec2 TexCoords;
		
		struct Light
		{
			int type;
			vec3 position;
			vec3 direction;
			vec3 color;
		};

	uniform sampler2D gPosition;
	uniform sampler2D gNormal;
	uniform sampler2D gAlbedoSpec;
	uniform sampler2D gDepth;
	uniform sampler2D gSSAO;
	uniform sampler2D gSSAOBlur;
	uniform sampler2D gData;
	uniform sampler2D brdfLUT;  
	uniform samplerCube irradianceMap;
	uniform samplerCube prefilterMap;
	uniform float bloomThreshold = 1.0;
	uniform float ssaoIntesity = 1.0;

	uniform int activeSSAO ;
	uniform Light lights[10];
	uniform int numLights = 0;

	uniform int renderMode;
	
	uniform vec3 camPos;
	vec3 WorldPos;

	const float PI = 3.14159265359;

float near = 0.1; 
float far  = 50.0; 
  
float LinearizeDepth(float depth) 
{
    float z = depth * 2.0 - 1.0; // back to NDC 
    return (2.0 * near) / (far + near - z * (far - near));	
}

	void CalculateFinalRender();

		void main()
		{   
			if(renderMode == 1)
				FragColor.xyz = texture(gNormal, TexCoords).xyz;
			else if(renderMode == 2)
			{
				FragColor.xyz = vec3(LinearizeDepth(texture(gDepth, TexCoords).z));
			}
				
			else if(renderMode == 3)
				FragColor.xyz = texture(gPosition, TexCoords).xyz;

			else if(renderMode == 4)
				FragColor.xyz = texture(gAlbedoSpec, TexCoords).xyz;

			else if(renderMode == 5)
			{
				FragColor.xyz = vec3(texture(gSSAO, TexCoords).xyz);
			}
			else if(renderMode == 6)
			{
				FragColor.xyz = vec3(texture(gSSAOBlur, TexCoords).r);
			}
			else if(renderMode == 7)
			{
				FragColor.xyz = vec3(texture(gData, TexCoords).x);
			}
			else if (renderMode == 8)
			{
				FragColor.xyz = vec3(texture(gData, TexCoords).y);
			}
			else if (renderMode == 9)
			{
				FragColor.xyz = vec3(texture(gData, TexCoords).b);
			}
			else if(renderMode == 10)
			{
				FragColor.xyz = vec3(texture(brdfLUT, TexCoords).rg, 0.0);
			}
			else if (renderMode == 11)
			{
				FragColor.xyz = vec3(texture(irradianceMap, texture(gPosition, TexCoords).xyz).xyz);
			}
			else if (renderMode == 12)
			{
				FragColor.xyz = vec3(texture(prefilterMap, texture(gPosition, TexCoords).xyz).xyz);
			}
			else if (renderMode == 13)
			{
				if(TexCoords.x < 0.25)
					FragColor.xyz = texture(gAlbedoSpec, TexCoords).xyz;
					
				else if (TexCoords.x < 0.5)
					FragColor.xyz = texture(gNormal, TexCoords).xyz;
					
				else if (TexCoords.x < 0.75)
					FragColor.xyz = vec3(LinearizeDepth(texture(gDepth, TexCoords).z));
					
				else
					FragColor.xyz = texture(gPosition, TexCoords).xyz;
			}

			else
			{
				CalculateFinalRender();
			}
			FragColor.w = 1;
		
		}

	float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a      = roughness*roughness;
    float a2     = a*a;
    float NdotH  = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;
	
    float num   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;
	
    return num / denom;
}

float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float num   = NdotV;
    float denom = NdotV * (1.0 - k) + k;
	
    return num / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2  = GeometrySchlickGGX(NdotV, roughness);
    float ggx1  = GeometrySchlickGGX(NdotL, roughness);
	
    return ggx1 * ggx2;
}
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}  

vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}  


void CalculateFinalRender()
{
	vec3 color;
	bool sky = texture(gAlbedoSpec, TexCoords).a == 0;
	if(sky)
	{
		color = texture(gAlbedoSpec, TexCoords).xyz;
	}
	else
	{
		float roughness = texture(gData, TexCoords).r;
		WorldPos = texture(gPosition, TexCoords).xyz;
		vec3 albedo =  pow(texture(gAlbedoSpec, TexCoords).xyz, vec3(2.2));
		float metallic = texture(gData, TexCoords).g;
		vec3 Normal =  texture(gNormal, TexCoords).xyz;
		float ao = texture(gData, TexCoords).b;
		float ssao = smoothstep( 0.0, ssaoIntesity,  texture(gSSAOBlur,TexCoords).r); 

		vec3 N = Normal; 
		vec3 V = normalize(camPos - WorldPos);
		vec3 R = reflect(-V, N); 

		vec3 F0 = vec3(0.04); 
		F0      = mix(F0, albedo, metallic);

		vec3 Lo = vec3(0.0);
		for(int i = 0; i < numLights; ++i) 
		{
			if(lights[i].type == 0)
				continue;

			vec3 L = normalize(lights[i].position - WorldPos);
			vec3 H = normalize(V + L);
			float distance    = length(lights[i].position - WorldPos);
			float attenuation = 1.0 / (distance * distance);
			vec3 radiance     = (lights[i].color * 255.0f) * attenuation; 

			float NDF = DistributionGGX(N, H, roughness);       
			float G   = GeometrySmith(N, V, L, roughness);     
			vec3 F  = fresnelSchlick(max(dot(H, V), 0.0), F0);

			vec3 numerator    = NDF * G * F;
			float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001; // 0.001 to prevent divide by zero.
			vec3 specular     = numerator / max(denominator, 0.001);  

			vec3 kS = F;
			vec3 kD = vec3(1.0) - kS;
			kD *= 1.0 - metallic;	  

			// add to outgoing radiance Lo
			float NdotL = max(dot(N, L), 0.0);                
			Lo += (kD * albedo / PI + specular) * radiance * NdotL; 
			
		}

		// ambient lighting (we now use IBL as the ambient term)
		vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
		
		vec3 kS = F;
		vec3 kD = 1.0 - kS;
		kD *= 1.0 - metallic;	  
		
		vec3 irradiance = texture(irradianceMap, N).rgb;
		vec3 diffuse      = irradiance * albedo;
		
		// sample both the pre-filter map and the BRDF lut and combine them together as per the Split-Sum approximation to get the IBL specular part.
		const float MAX_REFLECTION_LOD = 4.0;
		vec3 prefilteredColor = textureLod(prefilterMap, R,  roughness * MAX_REFLECTION_LOD).rgb;    
		vec2 brdf  = texture(brdfLUT, vec2(max(dot(N, V), 0.0), roughness)).rg;
		vec3 specular = prefilteredColor * (F * brdf.x + brdf.y);

		if(activeSSAO == 0)
			ssao = 1.0;
			
		vec3 ambient = (kD * diffuse + specular) * ao * ssao; 
		color = ambient + Lo;

	}
	

    FragColor = vec4(color, 1.0);

	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));

    if(brightness > bloomThreshold)
        BrightColor = vec4(FragColor.rgb, 1.0);
    else
        BrightColor = vec4(0.0, 0.0, 0.0, 1.0);

}

#endif