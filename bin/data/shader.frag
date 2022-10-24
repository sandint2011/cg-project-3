#version 410

in vec3 fragNormal;
in float distanceFromCamera;

uniform float startFade;
uniform float endFade;

out vec4 outColor;

uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 ambientColor;

uniform int isWater;

void main()
{
	// Color based on normals.
	//vec3 meshColor = fragNormal * 0.5 + 0.5;
	vec3 meshColor = vec3(90.0/256.0, 130.0/256.0, 30.0/256.0);
	
	// Lighting.
	vec3 normal = normalize(fragNormal);
	float nDotL = max(0, dot(normal, lightDirection));
	// Surface lighting.
	vec3 irradiance = ambientColor + lightColor * nDotL;
	// Surface reflection.
	vec3 linearColor = meshColor * irradiance;

	// Custom water color (janky but whatever).
	if (isWater == 1) {
		linearColor = vec3(35.0/256.0, 100.0/256.0, 220.0/256.0);
	}

	// Fade out based on distance from camera.
	float fade = distanceFromCamera;
	fade = (fade - startFade) / (endFade - startFade); // Normalized.
	
	// Color based on normals, lighting, and distance fading.
	outColor = vec4(pow(linearColor, vec3(1.0 / 2.2)), 1.0 - fade);
}