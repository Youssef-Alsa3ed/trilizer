#shader vertex

#version 330 core
layout (location = 0) in vec3 aPos;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);
}

#shader fragment

out vec4 FragColor;

struct Material {
    vec3 diffuse;
    vec3 ambient;
    float specular;
    float shininess;
    sampler2D tex1;
    sampler2D tex2;
    sampler2D tex3;
};
uniform Material material;
uniform vec3 lightColor;
void main()
{
    FragColor = vec4(lightColor, 1.0); // set all 4 vector values to 1.0
}