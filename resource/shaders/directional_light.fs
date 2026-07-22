#version 330

in vec3 fragmentNormal;

out vec4 finalColor;

uniform vec4 colDiffuse;

void main()
{
	finalColor = vec4((colDiffuse * 0.5 * (max(dot(fragmentNormal, vec3(0,2,1)),0.2))).xyz, colDiffuse.w);
}
