R"(
#version 440 core

layout(location = 0) in vec3 vertexPosition;
layout(location = 1) in vec2 vertexUV;
layout(location = 2) in vec3 vertexNormal;
layout(location = 3) in vec3 vertexTangent;

Vertex getVertex() {
	return Vertex(vertexPosition, vertexUV, vertexNormal, vertexTangent);
}

uniform mat4 objectTransform;
mat4 getObjectTransform() { return objectTransform; }
uniform mat4 viewTransform;
mat4 getViewTransform() { return viewTransform; }
uniform mat4 projectionTransform;
mat4 getProjectionTransform() { return projectionTransform; }

out VS_OUT {
	out vec3 Position;
	out vec2 UV;
	out mat3 TBN;
} vs_out;


void setFragInfo(Vertex info) {
	vs_out.Position = info.position;
	vs_out.UV = info.uv;
	//Calculate TBN matrix
	vec3 Tangent = normalize(info.tangent);
	vec3 Normal = normalize(info.normal);
	Tangent = normalize(Tangent - dot(Tangent, Normal) * Normal);
	vec3 Bitangent = cross(Normal, Tangent);
	vs_out.TBN = mat3(Tangent, Bitangent, Normal);
}
)"
