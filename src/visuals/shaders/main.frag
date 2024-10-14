R"(
#version 440 core

in VS_OUT {
    vec3 Position;
    vec2 UV;
    mat3 TBN;
    vec3 Normal;
} fs_in;

vec3 GetFragPosition() { return fs_in.Position; }
vec2 GetFragUV() { return fs_in.UV; }
mat3 GetFragTBN() { return fs_in.TBN; }

uniform vec4 objectColor;
vec4 GetObjectColor() { return objectColor; }
uniform bool objectIsTextured;
uniform sampler2D objectTexture;
vec4 GetObjectTexture(vec2 uv) {
    vec4 tex = vec4(1.0);
    if(objectIsTextured) {
        tex = texture(objectTexture, uv);
    }
    return tex;
}
uniform bool objectIsNormalMapped;
uniform sampler2D objectNormalMap;
vec3 GetFragNormal(vec2 uv) {
	vec3 normal;
    if(objectIsNormalMapped) {
    	//Get RGB values of normal map:
        normal = texture(objectNormalMap, uv).rgb;
        // transform normal vector to range [-1,1] and transform according to TBN matrix:
        normal = normalize(fs_in.TBN * (normal * 2.0 - 1.0));
    }
    else {
    	//Set normal based on TBN matrix:
    	normal = normalize(fs_in.TBN * vec3(0,0,1));
    }
    return normal;
}

out vec4 FragColor;
void SetFragColor(vec4 color) {
    FragColor = color;
}
)"
