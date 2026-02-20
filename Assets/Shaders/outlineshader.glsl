#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec3 worldPos;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    worldPos = vec3(model * vec4(aPos, 1.0));
    Normal = mat3(transpose(inverse(model))) * aNormal;
    TexCoord = aTexCoord;

    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment

struct Material {
    vec3 diffuse;
    vec3 ambient;
    float specular;
    float shininess;
    sampler2D tex1;
    sampler2D tex2;
    sampler2D tex3;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    vec3 direction;

    float constant;
    float linear;
    float quadratic;

    float cutOff;
    float outerCutOff;

    int type;
};

out vec4 FragColor;

#define MAX_LIGHTS 6
#define DIRECTIONAL_LIGHT 0
#define SPOT_LIGHT 1
#define POINT_LIGHT 2

uniform Material material;
uniform Light lights[MAX_LIGHTS];

uniform int lightCount;
uniform vec3 viewPos;
in vec3 Normal;
in vec3 worldPos;
in vec2 TexCoord;

void main()
{
    FragColor = vec4(0.0, 0.0, 0.0, 1.0);
}  