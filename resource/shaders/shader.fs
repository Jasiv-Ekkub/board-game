#version 330

in vec3 fragNormal;
in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;

vec3 lightDirection = normalize(vec3(0, 2, 1));

void main()
{
	float lightFactor = max(dot(fragNormal, lightDirection), 0);
	vec3 lightVec = vec3(1) * lightFactor;

	finalColor = texture(texture0, fragTexCoord) * colDiffuse * vec4(lightVec, 1);
}
