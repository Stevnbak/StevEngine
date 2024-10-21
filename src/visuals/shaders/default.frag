R"(
#version 440 core

void SetFragColor(vec4 color);

vec4 GetLights(vec4 objectColor);

vec2 GetFragUV();

vec4 GetObjectAlbedo(vec2 uv);
vec4 GetObjectColor();
Material GetObjectMaterial();

//Main
void main() {
	//Texture
	vec4 tex = GetObjectAlbedo(GetFragUV());
	//Lights
	vec4 lights = GetLights(GetObjectColor() * tex);
	//Final color
	SetFragColor(lights);
}
)"
