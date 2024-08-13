R"(
#version 330 core
in vec3 fragPosition;
in vec2 fragTexture;
in vec3 fragNormal;

out vec4 FragColor;

uniform vec4 objectColor;
uniform bool objectIsTextured;
uniform sampler2D objectTexture;

//Material
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material objectMaterial = Material(vec3(1.0), vec3(1.0), vec3(1.0), 0.1);

//Global ambient lighting
uniform float ambientStrength = 0.5;
uniform vec3 ambientColor = vec3(1.0);

//Lights
uniform vec3 viewPosition;
struct Light {
    vec3 diffuse;
    vec3 specular;
};

#define MAX_DIRECTIONAL_LIGHTS 10
#define MAX_POINT_LIGHTS 50
#define MAX_SPOT_LIGHTS 50

//Directional lights
struct DirectionalLight {
    vec3 direction;

    Light basic;
};
uniform DirectionalLight directionalLights[MAX_DIRECTIONAL_LIGHTS];
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), objectMaterial.shininess);
    // combine results
    vec3 diffuse  = light.basic.diffuse  * diff * objectMaterial.diffuse;
    vec3 specular = light.basic.specular * spec * objectMaterial.specular;
    return (diffuse + specular);
}
//Point lights
struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    Light basic;
};
uniform PointLight pointLights[MAX_POINT_LIGHTS];
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), objectMaterial.shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float a = (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    float attenuation = (a == 0 ? 0 : (1.0 / a));
    // combine results
    vec3 diffuse  = light.basic.diffuse  * diff * objectMaterial.diffuse * attenuation;
    vec3 specular = light.basic.specular * spec * objectMaterial.specular * attenuation;
    return (diffuse + specular);
}
//Spot lights
struct SpotLight {
    vec3 position;
    vec3 direction;

    float cutOff;
    float outerCutOff;

    Light basic;
};
uniform SpotLight spotLights[MAX_SPOT_LIGHTS];
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    float theta     = dot(viewDir, normalize(-light.direction));
    float epsilon   = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 diffuse  = light.basic.diffuse  * intensity * objectMaterial.diffuse;
    vec3 specular = light.basic.specular * intensity * objectMaterial.specular;
    return (diffuse + specular);
}

//Main
void main() {
    //Texture
    vec4 tex = vec4(1.0);
    if(objectIsTextured) {
        tex = texture(objectTexture, fragTexture);
    }
    //Lights
    vec3 norm = normalize(-fragNormal);
    vec3 viewDir = normalize(viewPosition - fragPosition);
    vec3 lights = objectMaterial.ambient * ambientColor * ambientStrength;
    // Directional lights
    for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
        lights += CalculateDirectionalLight(directionalLights[i], norm, viewDir);
    // Point lights
    for(int i = 0; i < MAX_POINT_LIGHTS; i++)
        lights += CalculatePointLight(pointLights[i], norm, fragPosition, viewDir);
    // Spot lights
    for(int i = 0; i < MAX_SPOT_LIGHTS; i++)
        lights += CalculateSpotLight(spotLights[i], norm, fragPosition, viewDir);
    //Final color
    FragColor = vec4(lights, 1.0) * tex * objectColor;
}
)"
