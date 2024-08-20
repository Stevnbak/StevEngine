#version 330
in vec4 FragColor;
out vec4 FragColor2;

void main() {
    FragColor2 = FragColor * vec4(1.0, 0.5, 0.5, 1.0);
}
