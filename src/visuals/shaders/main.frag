R"(
#version 440 core

in vec3 fragPosition;
vec3 GetFragPosition() { return fragPosition; }
in vec2 fragUV;
vec2 GetFragUV() { return fragUV; }
in vec3 fragNormal;
vec3 GetFragNormal() { return fragNormal; }

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

out vec4 FragColor;
void SetFragColor(vec4 color) {
    FragColor = color;
}
)"
