#include "ofApp.h"
#include <vector>

//--------------------------------------------------------------
void ofApp::setup()
{
	ofDisableArbTex();

	ofEnableDepthTest();
	glEnable(GL_CULL_FACE);

	reloadShaders();
}

//--------------------------------------------------------------
void ofApp::reloadShaders()
{
	if (needsShaderReload)
	{
		// Reload all shaders here.
		// shader.load("shader.vert", "shader.frag");

		needsShaderReload = false;
	}
}

//--------------------------------------------------------------
void ofApp::update()
{
	reloadShaders();
}

//--------------------------------------------------------------
void ofApp::draw()
{
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key)
{
	if (key == '`')
	{
		needsShaderReload = true;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key)
{
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y)
{
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
	// float r = (float)(rand() % 100001) / 100000.0;
	// r = r * (end - start) + start;
	// return r;

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