#include "ofMain.h"

void buildTerrainMesh(ofMesh& terrainMesh, const ofShortPixels& heightmap,
    unsigned int xStart, unsigned int yStart, unsigned int xEnd, unsigned int yEnd, glm::vec3 scale)
{
    using namespace glm;

    for (int x = xStart; x <= xEnd; x++)
    {
        for (int y = yStart; y <= yEnd; y++)
        {
            float normalizedHeightmapValue = (heightmap.getColor(x, y).r) / static_cast<float>(USHRT_MAX);
            terrainMesh.addVertex(vec3(x * scale.x, scale.y * normalizedHeightmapValue, y * scale.z));
        }
    }

    int imageHeight = (yEnd - yStart);
    int thisX;
    int thisY;
    int nextX;
    int nextY;

    for (int x = xStart; x <= xEnd - 1; x++)
    {
        for (int y = yStart; y <= yEnd - 1; y++)
        {
            thisX = x - xStart;
            thisY = y - yStart;
            nextX = thisX + 1;
            nextY = thisY + 1;

            //    0 -- 3
            //    |  / |
            //    | /  |
            //    1 -- 2

            terrainMesh.addIndex(thisX * imageHeight + thisY); // 0 Top-left.
            terrainMesh.addIndex(thisX * imageHeight + nextY); // 1 Bottom-left.
            terrainMesh.addIndex(nextX * imageHeight + thisY); // 3 Top-right.

            terrainMesh.addIndex(nextX * imageHeight + thisY); // 3 Top-right.
            terrainMesh.addIndex(thisX * imageHeight + nextY); // 1 Bottom-left.
            terrainMesh.addIndex(nextX * imageHeight + nextY); // 2 Bottom-right.
        }
    }

    terrainMesh.flatNormals();
    //terrainMesh.smoothNormals(glm::radians(0.0f));
    
    // Flip normals.
    for (int n = 0; n < terrainMesh.getNumNormals(); n++)
    {
        terrainMesh.setNormal(n, -terrainMesh.getNormal(n));
    }
}
