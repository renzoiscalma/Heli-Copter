#include "Ground.h"
extern SceneManager* sceneManager;
extern MeshManager* meshManager;

Ground::Ground(SceneNode* parentNode, glm::vec3 position, int mapSize, int mapHeight) {
	fillUpMap(mapSize, mapHeight);
	TranslaterNode* mainRootNode = new TranslaterNode(parentNode, position);
	for (int x = 0; x < mapSize; x++) {
		for (int y = 0; y < mapHeight; y++) {
			for (int z = 0; z < mapSize; z++) {
				if (!map[x][y][z]) continue;
				TranslaterNode* n1 = new TranslaterNode(mainRootNode, glm::vec3(2 * x, 2 * y, 2 * z));
				RotatorNode* r1 = new RotatorNode(n1, 0, 90.f, glm::vec3(0, 0, 1));
				if (map[x][y][z] == 1) {
					MeshNode* m1 = new MeshNode(r1, "Data//Cube.s3d", meshManager);
				}
				else {
					MeshNode* m1 = new MeshNode(r1, "Data//Grass.s3d", meshManager);
				}
			}
		}
	}
}

void Ground::fillUpMap(int mapSize, int mapHeight)
{
	PerlinNoise pnoise(3);
	for (int x = 0; x < mapSize; x++) {
		for (int z = 0; z < mapSize; z++) {
			//how high up the y axis does this column of land go?
			int topY = pnoise.noise((double)x / 20, (double)z / 20, 0.5) * (mapHeight - 1);
			map[x][topY][z] = mapInitial[x][topY][z] = 2; //grass top
			for (int y = topY - 1; y >= 0; --y) map[x][y][z] = mapInitial[x][y][z] = 1; //dirt
		}
	}

	//if a cube is surrounded by cubes, empty it (hollow out the earth)
	//loop over all of map interior
	for (int x = 1; x < mapSize - 1; ++x)
		for (int y = 1; y < mapHeight - 1; ++y)
			for (int z = 1; z < mapSize - 1; ++z)
			{
				if (mapInitial[x][y][z]) //is there a cube here?
				{
					//check 6 sides...if we are surrounded by ground, 
					//don't bother making a cube here
					if (mapInitial[x - 1][y][z]
						&& mapInitial[x + 1][y][z]
						&& mapInitial[x][y - 1][z]
						&& mapInitial[x][y + 1][z]
						&& mapInitial[x][y][z - 1]
						&& mapInitial[x][y][z + 1])
					{
						map[x][y][z] = 0;
					}
				}
			}
}