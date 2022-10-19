#pragma once

#include "ofMain.h"
#include "CellManager.h"
#include <vector>

class ofApp : public ofBaseApp
{

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

private:
	ofShortImage heightmapLowRes;
	ofShortImage heightmapFullRes;
	CellManager<5> cellManager{heightmapFullRes, 1600, 256};
	ofShader shader;

	// Camera position.
	glm::vec3 cameraPosition = glm::vec3(0, 0, 0);
	glm::vec3 cameraFront = glm::vec3(0, 0, -1);
	glm::vec3 cameraUp = glm::vec3(0, 1, 0);

	// Camera rotation (direction).
	glm::vec3 cameraDirection = glm::vec3(0, 0, 0);
	float cameraPitch = 0; // In radians.
	float cameraHead = 0; // In radians.

	// Mouse controls.
	int lastMouseX = ofGetViewportWidth() / 2;
	int lastMouseY = ofGetViewportHeight() / 2;

	// Helper functions.
	void buildCube(ofVbo& cubeVBO);
	void buildCircle(ofVbo& circleVBO, int sides);
	void buildCylinder(ofVbo& cylinderVBO, int sides);
	void buildSphere(ofVbo& sphereVBO, int subdivTheta, int subdivPhi);
	void buildMesh(ofMesh &mesh, glm::vec3 pos, float width, float height);
	float randf(float start, float end);

	// Shader reloading.
	bool needsShaderReload = true;
	void reloadShaders();
};
