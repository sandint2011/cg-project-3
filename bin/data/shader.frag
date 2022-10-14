#version 410

in vec3 fragNormal;
in float distanceFromCamera;

uniform float startFade;
uniform float endFade;

out vec4 outColor;

void main()
{
	// Color based on normals.
	outColor = vec4(fragNormal * 0.5 + 0.5, 1.0);

	// Fade out based on distance from camera.
	float fade = distanceFromCamera;
	fade = (fade - startFade) / (endFade - startFade); // Normalized.
	outColor.a = 1.0 - fade;
}