R"(
#version 440 core

struct Vertex {
	vec3 position;
	vec2 uv;
	vec3 normal;
	vec3 tangent;
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
	Vertex o = Vertex(
		vec3(getObjectTransform() * vec4(v.position, 1.0)), //Position
		v.uv, //UV
		normalize(vec3(transpose(inverse(getObjectTransform())) * vec4(v.normal, 1.0))), //Normal
		normalize(vec3(transpose(inverse(getObjectTransform())) * vec4(v.tangent, 1.0))) //Tangent
	);
	setFragInfo(o);
}
)"
