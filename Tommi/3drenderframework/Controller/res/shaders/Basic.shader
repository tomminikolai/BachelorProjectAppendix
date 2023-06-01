#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
//layout(location = 1) in vec2 TexCoord;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec3 col;

//out vec2 v_TexCoord;
out vec3 v_fragpos;
out vec3 v_normal;
out vec3 v_col;

uniform mat4 u_MVP;
uniform mat4 u_M;
uniform mat4 u_P;

void main()
{
    gl_Position = u_MVP * vec4(position, 1.0);
    v_fragpos = vec3((u_M * vec4(position, 1.0)).xyz);
    //v_TexCoord = TexCoord;
    v_normal = (u_M * vec4(normal, 0)).xyz;//mat3(transpose(inverse(u_M))) * normal;
    v_col = col;
};

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

//in vec2 v_TexCoord;
in vec3 v_fragpos;
in vec3 v_normal;
in vec3 v_col;

uniform vec4 u_Color;
uniform sampler2D u_Texture;
uniform vec3 u_lightpos;
uniform vec3 u_viewpos;

void main()
{
    vec3 norm = normalize(v_normal);
    vec3 lightdir = normalize(u_lightpos - v_fragpos);
    float diff = max(dot(norm, lightdir), 0.0);
    float ambient_strength = 0.1;
    float specstrength = 0.5;
    vec3 viewDir = normalize(u_viewpos - v_fragpos);
    vec3 reflectdir = reflect(-lightdir, norm);
    float spec = pow(max(dot(viewDir, reflectdir), 0.0), 32);
    //vec4 texColor = texture(u_Texture, v_TexCoord);
    color = vec4(v_col, 1.0) * vec4((ambient_strength + (1-0) * (diff + spec)) * vec3(1.0, 1.0, 1.0), 1.0);
};
