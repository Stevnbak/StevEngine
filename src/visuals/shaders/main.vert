R"(
#version 440 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec3 vertexNormal;
layout(location = 2) in vec2 vertexUV;

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
};

Vertex getVertex() {
    return Vertex(vertexPosition, vertexNormal, vertexUV);
}

uniform mat4 objectTransform;
mat4 getObjectTransform() { return objectTransform; }
uniform mat4 viewTransform;
mat4 getViewTransform() { return viewTransform; }
uniform mat4 projectionTransform;
mat4 getProjectionTransform() { return projectionTransform; }

out vec3 fragPosition;
out vec2 fragUV;
out vec3 fragNormal;

void setFragInfo(Vertex info) {
    fragPosition = info.position;
    fragUV = info.uv;
    fragNormal = info.normal;
}
)"
