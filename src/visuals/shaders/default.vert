R"(
#version 330 core
layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexTexture;

uniform mat4 objectTransform;
uniform mat4 viewTransform;
uniform mat4 projectionTransform;

out vec3 fragPosition;
out vec2 fragTexture;
out vec3 fragNormal;

void main() {
    gl_Position = projectionTransform * viewTransform * objectTransform * vec4(vertexPosition, 1.0);
    fragPosition = vec3(objectTransform * vec4(vertexPosition, 1.0));
    fragTexture = vertexTexture;
    fragNormal = mat3(transpose(inverse(objectTransform))) * vertexNormal;
}
)"
