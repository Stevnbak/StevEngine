R"(
#version 440 core

//Input
in VS_OUT {
	vec3 Position;
	vec2 UV;
	mat3 TBN;
} fs_in;
vec3 GetFragPosition() { return fs_in.Position; }
vec2 GetFragUV() { return fs_in.UV; }
mat3 GetFragTBN() { return fs_in.TBN; }
uniform vec3 viewPosition;
uniform vec3 viewDirection;
vec3 GetViewPosition() { return viewPosition; }
vec3 GetViewDirection() { return viewDirection; }

//Material
uniform Material objectMaterial;
Material GetObjectMaterial() { return objectMaterial; }
vec4 GetObjectColor() { return objectMaterial.color; }
//Textures
uniform sampler2D albedoTexture;
uniform bool usingAlbedoTexture;
uniform sampler2D normalTexture;
uniform bool usingNormalTexture;
vec4 GetObjectAlbedo(vec2 uv) {
	vec4 albedo = vec4(1.0);
	if(usingAlbedoTexture) {
		albedo = texture(albedoTexture, uv);
	}
	return albedo;
}
vec3 GetFragNormal(vec2 uv) {
	vec3 normal;
	if(usingNormalTexture) {
		//Get and transform RGB values of normal map:
		normal = texture(normalTexture, uv).rgb * 2.0 - 1.0;
		normal = normalize(fs_in.TBN * normal);
	}
	else {
		//Set normal based on TBN matrix:
		normal = normalize(fs_in.TBN * vec3(0,0,1));
	}
	return normal;
}

//Output
out vec4 FragColor;
void SetFragColor(vec4 color) {
	FragColor = color;
}
)"
