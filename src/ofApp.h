#pragma once

#include "ofMain.h"
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
	// Helper functions.
	void buildCube(ofVbo& cubeVBO);
	void buildCircle(ofVbo& circleVBO, int sides);
	void buildCylinder(ofVbo& cylinderVBO, int sides);
	void buildMesh(ofMesh &mesh, glm::vec3 pos, float width, float height);
	float randf(float start, float end);

	// Shader reloading.
	bool needsShaderReload{true};
	void reloadShaders();
};
