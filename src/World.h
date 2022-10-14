#pragma once
#include "ofMain.h"

struct World
{
public:
    // The pixel array containing the world heightmap.
    const ofShortPixels* heightmap { nullptr };

    // The desired x,y,z scale for the height map. 
    // The terrain will span from (0,0,0) to these dimensions, in world space coordinates
    // In other words, this field represents width, height, and depth of the world's terrain.
    glm::vec3 dimensions { 1 };

    // The scale of the world's gravity.
    // Represents the acceleration applied to gravity-affected bodies, 
    // in world-space distance units per second squared.
    float gravity { -1 };

    // The height at which water will be rendered in the world.
    // Terrain below this height is considered to be underwater.
    float waterHeight { 0 };

    // Gets the height of the terrain at a particular position in world space.
    float getTerrainHeightAtPosition(const glm::vec3& position) const;
};