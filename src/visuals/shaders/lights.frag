R"(
#version 440 core

vec3 GetFragPosition();
vec2 GetFragUV();
vec3 GetFragNormal(vec2 uv);
Material GetObjectMaterial();
vec3 GetViewDirection();
vec3 GetViewPosition();

//Global ambient lighting
uniform float ambientStrength;
uniform vec4 ambientColor;

//Lights
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
vec3 CalculateDirectionalLight(DirectionalLight light, vec3 normal, vec3 viewDir, vec4 objectColor)
{
	Material objectMaterial = GetObjectMaterial();
	vec3 lightDir = normalize(light.direction);
	// diffuse shading
	float diff = max(dot(normal, lightDir), 0.0);
	// specular shading
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), objectMaterial.shininess);
	// combine results
	vec3 diffuse  = light.basic.diffuse  * diff * objectMaterial.diffuse * vec3(objectColor);
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
vec3 CalculatePointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 objectColor)
{
	Material objectMaterial = GetObjectMaterial();
	vec3 lightDir = normalize((light.position) - fragPos);
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
	vec3 diffuse  = light.basic.diffuse  * diff * objectMaterial.diffuse * attenuation * vec3(objectColor);
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
vec3 CalculateSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir, vec4 objectColor)
{
	Material objectMaterial = GetObjectMaterial();
	float theta	 = dot(viewDir, normalize(light.direction));
	float epsilon   = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
	// combine results
	vec3 diffuse  = light.basic.diffuse  * intensity * objectMaterial.diffuse * vec3(objectColor);
	vec3 specular = light.basic.specular * intensity * objectMaterial.specular;
	return (diffuse + specular);
}

vec4 GetLights(vec4 objectColor)
{
	Material objectMaterial = GetObjectMaterial();
	vec3 FragPos = GetFragPosition();
	//Lights
	vec3 normal = normalize(GetFragNormal(GetFragUV()));
	vec3 viewDir = normalize(GetViewPosition() - FragPos);
	vec4 lights = vec4(objectMaterial.ambient * ambientStrength, 1.0) * ambientColor * objectColor;
	// Directional lights
	for(int i = 0; i < MAX_DIRECTIONAL_LIGHTS; i++)
	lights += vec4(CalculateDirectionalLight(directionalLights[i], normal, viewDir, objectColor), 1.0);
	// Point lights
	for(int i = 0; i < MAX_POINT_LIGHTS; i++)
	lights += vec4(CalculatePointLight(pointLights[i], normal, FragPos, viewDir, objectColor), 1.0);
	// Spot lights
	for(int i = 0; i < MAX_SPOT_LIGHTS; i++)
	lights += vec4(CalculateSpotLight(spotLights[i], normal, FragPos, viewDir, objectColor), 1.0);
	// Return combined lights
	return lights;
}
)"
