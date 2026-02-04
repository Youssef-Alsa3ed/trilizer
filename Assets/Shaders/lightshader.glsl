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

    sampler2D diffuse;
    sampler2D specular;
    float shininess;
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
};

out vec4 FragColor;

#define MAX_LIGHTS 6

uniform Material material;
uniform Light lights[MAX_LIGHTS];

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
    bool isDirectional = length(light.direction) > 0.001 && light.cutOff < 0.001;
    bool isSpot        = light.cutOff > 0.0;


    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(light.position - worldPos);
    float attenuation = 1.0;
    if(isDirectional)
    {
        lightDir = normalize(-light.direction);
    }
    else {
        float distance    = length(light.position - worldPos);
        float constant    = light.constant;
        float linear      = light.linear;
        float quadratic   = light.quadratic;
        float denom = constant + linear*distance + quadratic*distance*distance;
        attenuation = 1.0 / max(denom, 0.0001);

        if(isSpot)
        {            
            float theta = dot(lightDir, normalize(-light.direction));
            float epsilon   = light.cutOff - light.outerCutOff;
            float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0) * 1.0;
            attenuation *= intensity;
        }
    }

    float diff = diffuse(norm, lightDir) * attenuation;
    vec3 diffuse = light.diffuse * diff * texture(material.diffuse, TexCoord).rgb;
    
    // specular
    vec3 viewDir = normalize(viewPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * attenuation;
    vec3 specsample =  texture(material.specular, TexCoord).rgb;
    vec3 specular = light.specular * spec * specsample;

    vec3 result = diffuse + specular;

    result = vec3(max(result.r, 0.0), max(result.g, 0.0), max(result.b, 0.0));

    return (result);
}


void main()
{
    
    // ambient
    vec3 ambient = vec3(0.1, 0.1, 0.1) * texture(material.diffuse, TexCoord).rgb;
    vec3 result = ambient;
    for(int i = 0; i < MAX_LIGHTS; i++){
        result += CalculateLighting(material, lights[i], Normal, worldPos, viewPos, TexCoord);
    }
    FragColor = vec4(result, 1.0);
}  