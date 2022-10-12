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