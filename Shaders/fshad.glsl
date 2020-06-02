#version 410 core

struct Material {
    sampler2D albedoMap;
    sampler2D normalMap;
    sampler2D roughnessMap;
    sampler2D aoMap;
    sampler2D depthMap;
};

struct PointLight {
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;
};

struct DirLight {
    vec3 direction;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    
    float cutOff;
    float outerCutOff;
    
    float constant;
    float linear;
    float quadratic;
};

out vec4 FragColor;

in vec2 texCoords;
in vec3 FragPosition;
in mat3 TBN;

uniform vec3 cameraPos;
uniform Material material;
uniform PointLight pointLight;
uniform DirLight dirLight;
uniform SpotLight spotLight;
uniform bool Blinn;
uniform int lightType;
uniform float heightScale;

vec3 calcPointLight(PointLight pointLight, vec2 TexCoords){
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(material.normalMap, TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
    normal = normal * vec3(1,-1,1);
    normal = normalize(normal);
    
    // get original color
    vec3 albedo = texture(material.albedoMap, TexCoords).rgb;
    float ao = texture(material.aoMap, TexCoords).r;
    
    //ambient lighting
    vec3 ambient = vec3(0.1) * albedo * ao;
    
    //diffuse lighting
    vec3 tangentCamPos = TBN * cameraPos;
    vec3 tangentLightPos = TBN * pointLight.position;
    vec3 tangentFragPos = TBN * FragPosition;
    
    vec3 lightDir = normalize(tangentLightPos - tangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * albedo;
    
    //specular
    vec3 viewDir = normalize(tangentCamPos - tangentFragPos);
    float spec = 0.0;
    if(Blinn){
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0*(1-texture(material.roughnessMap, TexCoords).r));
    }
    else{
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0*(1-texture(material.roughnessMap, TexCoords).r));
    }
    //In Blinn Phong, the halfway vector is generally closer than the reflected dir, so to handle that, the shininess is generally set to 2-4 times of normal
    
    
    vec3 specular = vec3(0.3) * spec;
    
    float distance = length(pointLight.position - FragPosition);
    float attenuation = 1.0 / (pointLight.constant + pointLight.linear * distance + pointLight.quadratic * (distance * distance));
    
    return (ambient+diffuse+specular)*attenuation;
}

vec3 calcDirLight(DirLight dirLight, vec2 TexCoords){
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(material.normalMap, TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
    normal = normal * vec3(1,-1,1);
    normal = normalize(normal);
    
    // get original color
    vec3 albedo = texture(material.albedoMap, TexCoords).rgb;
    float ao = texture(material.aoMap, TexCoords).r;
    
    //ambient lighting
    vec3 ambient = vec3(0.1) * albedo * ao;
    
    //diffuse lighting
    vec3 tangentCamPos = TBN * cameraPos;
    vec3 tangentFragPos = TBN * FragPosition;
    vec3 tangentLightDir = TBN * dirLight.direction;
    
    vec3 lightDir = normalize(tangentLightDir);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * albedo;
    
    //specular
    vec3 viewDir = normalize(tangentCamPos - tangentFragPos);
    float spec = 0.0;
    if(Blinn){
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0*(1-texture(material.roughnessMap, TexCoords).r));
    }
    else{
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0*(1-texture(material.roughnessMap, TexCoords).r));
    }
    //In Blinn Phong, the halfway vector is generally closer than the reflected dir, so to handle that, the shininess is generally set to 2-4 times of normal
    
    
    vec3 specular = vec3(0.3) * spec;
    
    return ambient+diffuse+specular;
}

vec3 calcSpotLight(SpotLight spotLight, vec2 TexCoords){
    // obtain normal from normal map in range [0,1]
    vec3 normal = texture(material.normalMap, TexCoords).rgb;
    // transform normal vector to range [-1,1]
    normal = normalize(normal * 2.0 - 1.0);  // this normal is in tangent space
    normal = normal * vec3(1,-1,1);
    normal = normalize(normal);
    
    // get original color
    vec3 albedo = texture(material.albedoMap, TexCoords).rgb;
    float ao = texture(material.aoMap, TexCoords).r;
    
    //ambient lighting
    vec3 ambient = vec3(0.1) * albedo * ao;
    
    //diffuse lighting
    vec3 tangentCamPos = TBN * cameraPos;
    vec3 tangentLightPos = TBN * spotLight.position;
    vec3 tangentFragPos = TBN * FragPosition;
    vec3 tangentDirection = TBN * spotLight.direction;
    
    vec3 lightDir = normalize(tangentLightPos - tangentFragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * albedo;
    
    //specular
    vec3 viewDir = normalize(tangentCamPos - tangentFragPos);
    float spec = 0.0;
    if(Blinn){
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(normal, halfwayDir), 0.0), 32.0*(1-texture(material.roughnessMap, TexCoords).r));
    }
    else{
        vec3 reflectDir = reflect(-lightDir, normal);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), 8.0*(1-texture(material.roughnessMap, TexCoords).r));
    }
    //In Blinn Phong, the halfway vector is generally closer than the reflected dir, so to handle that, the shininess is generally set to 2-4 times of normal
    
    vec3 specular = vec3(0.3) * spec;
    float distance = length(spotLight.position - FragPosition);
    float attenuation = 1.0 / (spotLight.constant + spotLight.linear * distance + spotLight.quadratic * (distance * distance));
    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-tangentDirection));
    float epsilon = spotLight.cutOff - spotLight.outerCutOff;
    float intensity = clamp((theta - spotLight.outerCutOff) / epsilon, 0.0, 1.0);
    
    return (ambient+diffuse+specular)*attenuation*intensity;
}

void main(){
    vec3 light = vec3(0.0);
    if(lightType == 0)
        light = calcPointLight(pointLight, texCoords);
    else if(lightType == 1)
        light = calcDirLight(dirLight, texCoords);
    else if(lightType == 2)
        light = calcSpotLight(spotLight, texCoords);
    
    FragColor = vec4(light, 1.0);
}
