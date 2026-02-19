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

float diffuse(vec3 normal, vec3 lightDir)
{
    return max(dot(normal, lightDir), 0.0);
}


vec3 CalculateLighting(Material material, Light light, vec3 Normal, vec3 worldPos, vec3 viewPos, vec2 TexCoord)
{
    vec3 norm = normalize(Normal);
    vec3 lightDir;
    float attenuation = 1.0;

    if(light.type == DIRECTIONAL_LIGHT)
    {
        lightDir = normalize(-light.direction);
    }
    else
    {
        lightDir = normalize(light.position - worldPos);

        float distance = length(light.position - worldPos);
        attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * distance * distance);

        if(light.type == SPOT_LIGHT)
        {
            float theta = dot(lightDir, normalize(-light.direction));
            float epsilon = max(light.cutOff - light.outerCutOff, 0.0001);
            float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
            attenuation *= intensity;
        }
    }

    float diffAmount = max(dot(norm, lightDir), 0.0);
    vec3 diffuseSample = texture(material.tex1, TexCoord).rgb;
    vec3 diffuseColor = light.diffuse * diffAmount * diffuseSample;
    vec3 ambient = diffuseSample * light.ambient;
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float specFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    vec3 specularColor = light.specular * specFactor * texture(material.tex2, TexCoord).rgb;

    diffuseColor *= attenuation;
    specularColor *= attenuation;

    return diffuseColor + specularColor;
}

void main()
{
    vec3 result = vec3(0.0);
    vec3 norm = normalize(Normal);
    for(int i = 0; i < lightCount; i++){
        result += CalculateLighting(material, lights[i], Normal, worldPos, viewPos, TexCoord);
    }
    
    FragColor = vec4(result, 1.0);

}  