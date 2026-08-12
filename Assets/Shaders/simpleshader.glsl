#shader vertex
#version 330 core
layout (location = 0) in vec3 aPos;   // the position variable has attribute position 0
layout (location = 1) in vec2 aTexCoord; // the texture coordinate variable has attribute position 2
out vec3 vertexColor; // specify a color output to the fragment shader
out vec2 TexCoord; // output texture coordinate to fragment shader

uniform mat4 mvp;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(aPos, 1.0);

    TexCoord = aTexCoord;
} 

#shader fragment

#version 330 core
out vec4 FragColor;  
in vec3 vertexColor; // the input variable from the vertex shader (same name and same type)
in vec2 TexCoord; // input texture coordinate from vertex shader

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
FragColor = mix(texture(texture1, TexCoord),
texture(texture2, TexCoord), 0.2);
}