R"(
#version 440 core

void SetFragColor(vec4 color);

vec3 GetLights();

vec2 GetFragUV();

vec4 GetObjectTexture(vec2 uv);
vec4 GetObjectColor();


//Main
void main() {
    //Texture
    vec4 tex = GetObjectTexture(GetFragUV());
    //Lights
    vec3 lights = GetLights();
    //Final color
    vec4 color = vec4(lights, 1.0) * tex * GetObjectColor();
    SetFragColor(color);
}
)"
