#version 330 core

layout(location=0)in vec3 position; 
layout(location=1)in vec3 normals;
layout(location=2)in vec2 texCoord;
layout(location=3)in vec3 tangents; 
layout(location=4)in vec3 bitangents; 

out vec2 v_texCoord;
out vec3 TangentLightPos;
out vec3 TangentViewPos;
out vec3 TangentFragPos;

uniform mat4 modelTransformMatrix;
uniform mat4 projectionMatrix;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main()
{
    // Calculate position in world space
    vec3 FragPos = vec3(modelTransformMatrix * vec4(position, 1.0f));

    // Create the TBN matrix (Tangent, Bitangent, Normal)
    mat3 TBN = transpose(mat3(tangents, bitangents, normals));

    // Transform positions into tangent space
    TangentLightPos = TBN * (lightPos - FragPos);
    TangentViewPos  = TBN * (viewPos - FragPos);
    TangentFragPos  = TBN * FragPos;

    // Pass texture coordinates to the fragment shader
    v_texCoord = texCoord;

    // Final position
    gl_Position = projectionMatrix * modelTransformMatrix * vec4(position, 1.0f);
}
