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

out vec4 FragColor;
  
in vec2 TexCoords;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure = 0.2;
uniform int mode = 0;

void main()
{   
    if(mode == 0)
    {
        const float gamma = 2.2;
            vec3 hdrColor = texture(scene, TexCoords).rgb;      
            vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;
            hdrColor += bloomColor; // additive blending
            // tone mapping
            vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
            // also gamma correct while we're at it       
            result = pow(result, vec3(1.0 / gamma));
            FragColor = vec4(result, 1.0);
    }   
    else if(mode == 1)
    {
        FragColor = texture(scene, TexCoords);
    }
    else if (mode == 2)
    {
        FragColor = texture(bloomBlur, TexCoords);
    }
}  

#endif
