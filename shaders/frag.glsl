#version 330 core

out vec4 FragColor;

in vec2 v_texCoord;
in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform sampler2D u_DiffuseMap; 
uniform sampler2D u_NormalMap;

void main()
{
    // Sample normal from normal map and transform from [0,1] to [-1,1]
    vec3 normal = texture(u_NormalMap, v_texCoord).rgb;
    normal = normalize(normal * 2.0 - 1.0); // Transform to [-1, 1]

    // Calculate light direction
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);

    // Diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);

    // Sample color from diffuse map
    vec3 diffuse = texture(u_DiffuseMap, v_texCoord).rgb;

    // Specular shading (optional)
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
    vec3 specular = vec3(0.2) * spec; // Adjust specular intensity

    // Combine results
    vec3 result = (diff * diffuse) + specular;

    FragColor = vec4(result, 1.0);
}
