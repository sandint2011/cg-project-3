#include "ofMain.h"

void buildTerrainMesh(ofMesh& terrainMesh, const ofShortPixels& heightmap,
    unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd, glm::vec3 scale)
{
    using namespace glm;

    for (int x = xStart; x <= xEnd; x++)
    {
        for (int y = yStart; y <= yEnd; y++)
        {
            terrainMesh.addVertex(vec3(x * scale.x, y * scale.z, scale.y));
        }
    }
}
