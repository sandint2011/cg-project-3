#include "ofMain.h"

void buildTerrainMesh(ofMesh& terrainMesh, const ofShortPixels& heightmap,
    unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd, glm::vec3 scale)
{
    using namespace glm;

    for (int x = xStart; x <= xEnd; x++)
    {
        for (int y = yStart; y <= yEnd; y++)
        {
            terrainMesh.addVertex(vec3(x * scale.x, scale.y * (heightmap.getColor(x,y).r), y * scale.z));
        }
    }
    
    for (int x = xStart; x <= xEnd -1; x++)
    {
        for (int y = yStart; y <= yEnd -1; y++)
        {
            terrainMesh.addIndex( x*(xEnd-xStart) + y);
            terrainMesh.addIndex(x * (xEnd - xStart) + y + 1);
            terrainMesh.addIndex((x + 1) * (xEnd - xStart) + y);
            terrainMesh.addIndex((x + 1) * (xEnd - xStart) + y);
            terrainMesh.addIndex(x * (xEnd - xStart) + y + 1);
            terrainMesh.addIndex((x + 1) * (xEnd - xStart) + y + 1);
        }
    }

    terrainMesh.flatNormals();
}
