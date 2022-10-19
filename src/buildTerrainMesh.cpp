#include "ofMain.h"

void buildTerrainMesh(ofMesh& terrainMesh, const ofShortPixels& heightmap,
    unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd, glm::vec3 scale)
{
    using namespace glm;

    for (int x = xStart; x <= xEnd; x++)
    {
        for (int y = yStart; y <= yEnd; y++)
        {
            terrainMesh.addVertex(vec3(x * scale.x, scale.y * (heightmap.getColor(x,y).r) / (float)USHRT_MAX, y * scale.z));
        }
    }
    
    for (int x = xStart; x <= xEnd -1; x++)
    {
        for (int y = yStart; y <= yEnd -1; y++)
        {
            terrainMesh.addIndex((x - xStart) * (yEnd - yStart) + (y - yStart));
            terrainMesh.addIndex((x - xStart) * (yEnd - yStart) + (y - yStart) + 1);
            terrainMesh.addIndex(((x - xStart) + 1) * (yEnd - yStart) + (y - yStart));
            terrainMesh.addIndex(((x - xStart) + 1) * (yEnd - yStart) + (y - yStart));
            terrainMesh.addIndex((x - xStart) * (yEnd - yStart) + (y - yStart) + 1);
            terrainMesh.addIndex(((x - xStart) + 1) * (yEnd - yStart) + (y - yStart) + 1);
        }
    }

    terrainMesh.flatNormals();
    //flip normals
}
