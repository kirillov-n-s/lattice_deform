#version 330 core

layout (location = 0) in vec4 aPosition;
layout (location = 1) in vec3 aTexcoord;
layout (location = 2) in vec3 aNormal;

out vec4 Position;
out vec3 Normal;

uniform mat4 uModel;
uniform mat4 uNormalModel;
uniform mat4 uView;
uniform mat4 uProjection;

void main()
{
    gl_Position = uProjection * uView * uModel * aPosition;
    Position = vec4(vec3(uModel * aPosition), 1.);
    Normal = mat3(uNormalModel) * aNormal;
}
