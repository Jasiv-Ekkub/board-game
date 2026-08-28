#version 330

in vec3 vertexPosition;
in vec3 vertexNormal;
in vec2 vertexTexCoord;
in vec4 vertexColor;

out vec3 fragNormal;
out vec2 fragTexCoord;
out vec4 fragColor;

uniform mat4 mvp;
uniform mat4 matNormal;

void main()
{
	fragNormal = normalize(vec3(matNormal*vec4(vertexNormal, 1.0)));
	fragTexCoord = vertexTexCoord;
	fragColor = vertexColor;
	gl_Position = mvp * vec4(vertexPosition, 1);
}
