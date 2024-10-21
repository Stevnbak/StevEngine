#version 440 core

void SetFragColor(vec4 color);
vec3 GetFragNormal(vec2 uv);
vec3 GetFragPosition();
vec2 GetFragUV();

void TestNormals() {
	vec3 normal = GetFragNormal(GetFragUV()) * 0.5 + 0.5;
	SetFragColor(vec4(normal, 1.0));
}

void TestUV() {
	vec2 uv = GetFragUV();
	if(uv.x > 1) uv.x -= 1;
	if(uv.y > 1) uv.y -= 1;
	SetFragColor(vec4(uv.x, uv.y, 1.0, 1.0));
}

void main() {
	//SetFragColor(vec4(normalize(GetFragPosition()) * 0.5 + 0.5, 1.0));
	TestNormals();
	//TestUV();
}
