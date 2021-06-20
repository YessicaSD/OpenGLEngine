#if defined(VERTEX_SHADER)
layout (location = 0) in vec3 aPos;
layout(location=1) in vec3 aNormal;
layout(location=2) in vec2 aTexCoord;

out vec3 localPosition;

uniform mat4 projection;
uniform mat4 view;

void main()
{
    localPosition = aPos;
    gl_Position =  projection * view * vec4(localPosition, 1.0);
}

#elif defined(FRAGMENT_SHADER)

out vec4 FragColor;
in vec3 localPosition;

uniform sampler2D equirectangularMap;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{		
    vec2 uv = SampleSphericalMap(normalize(localPosition));
    vec3 envColor =  texture(equirectangularMap, uv).rgb;
    
    FragColor = vec4(envColor, 1.0);
}
#endif
