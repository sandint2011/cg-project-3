#include "ofApp.h"
#include <vector>
#include <random>

//--------------------------------------------------------------
void ofApp::setup()
{
	ofDisableArbTex();

	ofEnableDepthTest();
	glEnable(GL_CULL_FACE);

	// Low resolution terrain setup.
	heightmapLowRes.setUseTexture(false);
	heightmapLowRes.load("TamrielLowRes.png");
	assert(heightmapLowRes.getWidth() != 0 && heightmapLowRes.getHeight() != 0);

	// High resolution terrain setup.
	heightmapHighRes.setUseTexture(false);
	heightmapHighRes.load("TamrielHighRes.png");
	assert(heightmapHighRes.getWidth() != 0 && heightmapHighRes.getHeight() != 0);

	// Build terrain mesh and VBO.
	buildTerrainMesh(terrainMesh, heightmapLowRes, 0, 0, heightmapLowRes.getWidth() - 1, heightmapLowRes.getHeight() - 1, glm::vec3(1, heightScale, 1));
	terrainVBO.setMesh(terrainMesh, GL_STATIC_DRAW);

	// Setup cell manager.
	cameraPosition = glm::vec3(heightmapHighRes.getWidth() / 2, 0, heightmapHighRes.getHeight() / 2);
	cellManager.initializeForPosition(cameraPosition);

	reloadShaders();
}

//--------------------------------------------------------------
void ofApp::reloadShaders()
{
	if (needsShaderReload)
	{
		// Reload all shaders here.
		shader.load("shader.vert", "shader.frag");

		needsShaderReload = false;
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
	reloadShaders();

	cellManager.optimizeForPosition(cameraPosition);
}

//--------------------------------------------------------------
void ofApp::draw()
{
	// Camera settings.
	const float nearClip = 0.1;
	const float farClip = 200 * 32 * 10;

	const float startFade = farClip * 0.7;
	const float endFade = farClip * 0.9;

	float aspectRatio = static_cast<float>(ofGetViewportWidth()) / static_cast<float>(ofGetViewportHeight());

	// Movel-view-projection.
	glm::mat4 modelLowRes = (
		glm::translate(glm::vec3(0, -heightScale * 32, 0))
		* glm::scale(glm::vec3(32, 32, 32))
		);
	glm::mat4 modelHighRes = (
		glm::translate(glm::vec3(0, -heightScale * 32, 0))
		* glm::scale(glm::vec3(1, heightScale / 50, 1))
		);

	glm::mat4 view = glm::lookAt(cameraPosition, cameraPosition + cameraFront, cameraUp);
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), aspectRatio, nearClip, farClip);


	// Shader drawing.
	shader.begin();

	shader.setUniform3f("cameraPosition", cameraPosition);
	shader.setUniform1f("startFade", startFade);
	shader.setUniform1f("endFade", endFade);

	shader.setUniformMatrix4f("m", modelLowRes);
	shader.setUniformMatrix4f("mvp", projection * view * modelLowRes);
	terrainVBO.drawElements(GL_TRIANGLES, terrainVBO.getNumIndices());

	glClear(GL_DEPTH_BUFFER_BIT);

	// Cell manager drawing.
	shader.setUniformMatrix4f("m", modelHighRes);
	shader.setUniformMatrix4f("mvp", projection * view * modelHighRes);
	cellManager.drawActiveCells(cameraPosition, farClip);

	shader.end();
}

//--------------------------------------------------------------
void ofApp::exit()
{
	cellManager.stop();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == '`')
	{
		needsShaderReload = true;
	}

	const float cameraSpeed = 20 * 32;
	const float sprint = 5;
	const float dt = ofGetLastFrameTime();

	// Forward / backward.
	if (key == 'w')
		cameraPosition += cameraFront * cameraSpeed * dt;
	else if (key == 'W')
		cameraPosition += cameraFront * cameraSpeed * sprint * dt;
	if (key == 's')
		cameraPosition -= cameraFront * cameraSpeed * dt;
	else if (key == 'S')
		cameraPosition -= cameraFront * cameraSpeed * sprint * dt;

	// Left / right.
	if (key == 'a')
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * dt;
	else if (key == 'A')
		cameraPosition -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * sprint * dt;
	if (key == 'd')
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * dt;
	else if (key == 'D')
		cameraPosition += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed * sprint * dt;

	// Up / down.
	if (key == 'q')
		cameraPosition += cameraUp * cameraSpeed * dt;
	else if (key == 'Q')
		cameraPosition += cameraUp * cameraSpeed * sprint * dt;
	if (key == 'e')
		cameraPosition -= cameraUp * cameraSpeed * dt;
	else if (key == 'E')
		cameraPosition -= cameraUp * cameraSpeed * sprint * dt;
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
	// Get the mouse's change ins position since last frame.
	float dx = x - lastMouseX;
	float dy = y - lastMouseY;
	lastMouseX = x;
	lastMouseY = y;

	// Apply sensitivity to mouse movement.
	const float sensitivity = 0.5f;
	dx *= sensitivity;
	dy *= sensitivity;

	// Store camera rotation in radians.
	cameraHead += glm::radians(dx);
	cameraPitch += glm::radians(dy);

	// Clamp the pitch.
	cameraPitch = CLAMP(cameraPitch, glm::radians(-89.0f), glm::radians(89.0f));

	// Calculate camera direction.
	cameraDirection.x = cos(cameraHead - glm::radians(90.0f)) * cos(cameraPitch); // Sutract 90 degrees from head because otherwise 0 radians points at +X instead of -Z like we want.
	cameraDirection.y = -sin(cameraPitch); // Negative because otherwise we're doing inverted Y and that's gross.
	cameraDirection.z = sin(cameraHead - glm::radians(90.0f)) * cos(cameraPitch); // Sutract 90 degrees from head because otherwise 0 radians points at +X instead of -Z like we want.
	cameraFront = glm::normalize(cameraDirection);
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button)
{
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y)
{
}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h)
{
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg)
{
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo)
{
}

//--------------------------------------------------------------
float ofApp::randf(float start = 0.0, float end = 1.0)
{
	float r = ((float)rand()) / (float)RAND_MAX;
	r = r * (end - start) + start;
	return r;
}

//--------------------------------------------------------------
void ofApp::buildCube(ofVbo& cubeVBO)
{
	using namespace glm;
	ofMesh mesh {};

	mesh.addVertex(vec3(-1, -1, 1));	// 0 front left bottom.
	mesh.addVertex(vec3(-1, -1, -1));	// 1 back left bottom.
	mesh.addVertex(vec3(-1, 1, 1));		// 2 front left top.
	mesh.addVertex(vec3(-1, 1, -1));	// 3 back left top.
	mesh.addVertex(vec3(1, -1, 1));		// 4 front right bottom.
	mesh.addVertex(vec3(1, -1, -1));	// 5 back right bottom.
	mesh.addVertex(vec3(1, 1, 1));		// 6 front right top.
	mesh.addVertex(vec3(1, 1, -1));		// 7 back right top.

	ofIndexType indices[36] = {
		2, 6, 3, 3, 6, 7,	// Top.
		0, 1, 4, 4, 1, 5,	// Bottom.
		0, 2, 1, 1, 2, 3,	// Left.
		4, 5, 6, 7, 6, 5,	// Right.
		0, 4, 2, 2, 5, 6,	// Front.
		1, 3, 7, 7, 5, 1,	// Back.
	};

	mesh.addIndices(indices, 36);

	mesh.flatNormals(); // Calculate normals.

	cubeVBO.setMesh(mesh, GL_STATIC_DRAW);
}

//--------------------------------------------------------------
void ofApp::buildCircle(ofVbo& circleVBO, int sides)
{
	using namespace glm;
	ofMesh mesh{};

	mesh.addVertex(vec3(0, 0, 0)); // 0 center.
	
	for (int i = 0; i < sides; i++)
	{
		double theta = i * 2 * PI / sides; // Remaps i to (0, 2*PI).
		mesh.addVertex(vec3(cos(theta), sin(theta), 0)); // N edge.
	}

	int ringStart = 1;

	for (int i = 0; i < sides; i++)
	{
		mesh.addIndex(0); // Center.
		mesh.addIndex(ringStart + i); // First edge.
		mesh.addIndex(ringStart + (i + 1) % sides); // Next edge (counter-clockwise).
	}

	mesh.flatNormals();

	circleVBO.setMesh(mesh, GL_STATIC_DRAW);
}

//--------------------------------------------------------------
void ofApp::buildCylinder(ofVbo& cylinderVBO, int sides)
{
	using namespace glm;
	ofMesh mesh{};
	
	mesh.addVertex(vec3(0, 1, 0)); // 0 top center.

	for (int i = 0; i < sides; i++)
	{
		double theta = i * 2 * PI / sides; // Remaps i to (0, 2*PI).
		mesh.addVertex(vec3(cos(theta), 1, -sin(theta))); // N top edge.
	}

	mesh.addVertex(vec3(0, -1, 0)); // 0 bottom center.

	for (int i = 0; i < sides; i++)
	{
		double theta = i * 2 * PI / sides; // Remaps i to (0, 2*PI).
		mesh.addVertex(vec3(cos(theta), -1, -sin(theta))); // N bottom edge.
	}

	int topCenter = 0;
	int topRingStart = 1;
	int bottomCenter = sides + 1;
	int bottomRingStart = bottomCenter + 1;

	// Top.
	for (int i = 0; i < sides; i++)
	{
		mesh.addIndex(topCenter); // Top center.
		mesh.addIndex(topRingStart + i); // First top edge.
		mesh.addIndex(topRingStart + (i + 1) % sides); // Next top edge (counter-clockwise).
	}

	// Bottom.
	for (int i = 0; i < sides; i++)
	{
		mesh.addIndex(bottomCenter); // Bottom center.
		mesh.addIndex(bottomRingStart + (i + 1) % sides); // Next bottom edge (clockwise now, because it's unside-down).
		mesh.addIndex(bottomRingStart + i); // First bottom edge.
	}

	// Sides.
	for (int i = 0; i < sides; i++)
	{
		int indicesForSide[4] = {
			topRingStart + i,					// Top prev.
			topRingStart + (i + 1) % sides,		// Top next.
			bottomRingStart + i,				// Bottom prev.
			bottomRingStart + (i + 1) % sides,	// Bottom next.
		};

		mesh.addIndex(indicesForSide[0]);
		mesh.addIndex(indicesForSide[1]);
		mesh.addIndex(indicesForSide[2]);

		mesh.addIndex(indicesForSide[2]);
		mesh.addIndex(indicesForSide[1]);
		mesh.addIndex(indicesForSide[3]);
	}

	mesh.flatNormals();

	cylinderVBO.setMesh(mesh, GL_STATIC_DRAW);
}

//--------------------------------------------------------------
void buildSphere(ofVbo& sphereVBO, int subdivTheta, int subdivPhi)
{
	using namespace glm;
	ofMesh mesh{};

	// Mesh.

	mesh.addVertex(vec3(0, 1, 0)); // 0 north pole.

	for (int i = 1; i < subdivTheta; i++)
	{
		double theta = i * 2 * PI / subdivTheta; // Remaps i to (0, 2*PI).
		
		for (int j = 0; j < subdivPhi; j++)
		{
			double phi = j * 2 * PI / subdivPhi; // Remaps j to (0, 2*PI).

			mesh.addVertex(vec3(
				sin(theta) * cos(phi),
				cos(theta),
				-sin(theta) * sin(phi)
			));
		}
	}

	mesh.addVertex(vec3(0, -1, 0)); // N*M South pole.

	// Index buffer.

	int northPole = 0;
	int southPole = 1 + (subdivTheta - 1) * subdivPhi;

	// North pole circle.
	for (int i = 0; i < subdivPhi; i++)
	{
		mesh.addIndex(northPole);
		int ringStart = 1;
		mesh.addIndex(ringStart + i);
		mesh.addIndex(ringStart * (i + 1) & subdivPhi);
	}

	// South pole circle.
	for (int i = 0; i < subdivPhi; i++)
	{
		mesh.addIndex(southPole - subdivPhi);
		int ringStart = 1;
		mesh.addIndex(ringStart * (i + 1) & subdivPhi); // Inverted winding order since it's bottom.
		mesh.addIndex(ringStart + i);
	}

	for (int i = 1; i < subdivTheta - 1; i++)
	{
		for (int j = 0; j < subdivPhi; j++)
		{
			int topRingStart = 1 + subdivPhi * (i - 1);
			int bottomRingStart = topRingStart + subdivPhi;

			int indicesForSide[4] = {
				topRingStart + j,						// Top prev.
				topRingStart + (j + 1) % subdivPhi,		// Top next.
				bottomRingStart + j,					// Bottom prev.
				bottomRingStart + (j + 1) % subdivPhi,	// Bottom next.
			};

			mesh.addIndex(indicesForSide[0]);
			mesh.addIndex(indicesForSide[2]);
			mesh.addIndex(indicesForSide[1]);

			mesh.addIndex(indicesForSide[1]);
			mesh.addIndex(indicesForSide[2]);
			mesh.addIndex(indicesForSide[3]);
		}
	}

	mesh.flatNormals();
	sphereVBO.setMesh(mesh, GL_STATIC_DRAW);
}

//--------------------------------------------------------------
void ofApp::buildMesh(ofMesh &mesh, glm::vec3 pos, float width, float height)
{
	float verts[12]{
		pos.x - width, pos.y - height, pos.z,
		pos.x - width, pos.y + height, pos.z,
		pos.x + width, pos.y + height, pos.z,
		pos.x + width, pos.y - height, pos.z};

	float uvs[8]{
		0, 0,
		0, 1,
		1, 1,
		1, 0};

	for (unsigned int i = 0; i < 4; i++)
	{
		int vertIndex = i * 3;
		int uvIndex = i * 2;

		mesh.addVertex(glm::vec3(verts[vertIndex], verts[vertIndex + 1], verts[vertIndex + 2]));
		mesh.addTexCoord(glm::vec2(uvs[uvIndex], uvs[uvIndex + 1]));
	}

	ofIndexType indices[6] = {0, 1, 2, 2, 3, 0};
	mesh.addIndices(indices, 6);
}