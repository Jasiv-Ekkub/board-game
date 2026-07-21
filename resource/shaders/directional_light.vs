#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;

out vec3 fragmentNormal;

uniform mat4 mvp;

void main()
{
	fragmentNormal = normalize(vertexNormal);
	gl_Position = mvp * vec4(vertexPosition, 1);
}
