#version 330 core

out vec4 FragColor;

in vec3 outNormal;

void main()
{
   FragColor = vec4(abs(outNormal), 1.0f);

}
