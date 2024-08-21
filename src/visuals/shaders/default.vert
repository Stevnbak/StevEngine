R"(
#version 440 core

struct Vertex {
    vec3 position;
    vec3 normal;
    vec2 uv;
};
Vertex getVertex();

mat4 getObjectTransform();
mat4 getViewTransform();
mat4 getProjectionTransform();

void setFragInfo(Vertex info);

out gl_PerVertex
{
	vec4 gl_Position;
};

void main() {
    Vertex v = getVertex();
    gl_Position = getProjectionTransform() * getViewTransform() * getObjectTransform() * vec4(v.position, 1.0);
    setFragInfo(Vertex(
        vec3(getObjectTransform() * vec4(v.position, 1.0)),
        mat3(transpose(inverse(getObjectTransform()))) * v.normal,
        v.uv
    ));
}
)"
