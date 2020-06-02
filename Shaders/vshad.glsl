#version 410 core

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoords;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;

out vec2 texCoords;
out vec3 FragPosition;
out mat3 TBN;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main(){
    gl_Position = projection * view * model * vec4(aPos,1.0);
    texCoords = aTexCoords;
    FragPosition = vec3(model*vec4(aPos,1.0));
    
    vec3 B = cross(aNormal, aTangent);
    TBN = transpose(mat3(aTangent, B, aNormal));
}
