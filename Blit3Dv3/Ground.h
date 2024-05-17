#pragma once
#include "SceneNodes.h"
#include "PerlinNoise.h"
class Ground
{
public:
	Ground(SceneNode* parentNode, glm::vec3 position, int sizeX, int sizeY);
	GLuint mapInitial[50][50][50] = { 0 };
	GLuint map[50][50][50] = { 0 };
private:
	void fillUpMap(int mapSize, int mapHeight);
};

