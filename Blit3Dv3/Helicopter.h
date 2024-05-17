#pragma once
#include "SceneNodes.h"
class Helicopter
{
public:
	Helicopter(SceneNode* ParentNode);
	// easy access nodes
	TranslaterNode* rootNode;			// root node of the helicopter
	RotatorNode* pitchRotNode;			// rotates the entire helicopter along x axis
	RotatorNode* headingRotNode;		// rotates the entire helicopter along y axis
	RotatorNode* rollRotNode;			// rotates the entire helicopter along z axis
	RotatorNode* mainRotorRotNode;		
	RotatorNode* tailRotorRotNode;	

	bool gas = false;

	bool headLeft = false;
	bool headRight = false;
	bool pitchUp = false;
	bool pitchDown = false;
	bool rollLeft = false;
	bool rollRight = false;
	float gasScale = 1; // TODO scale by controller

	float controllerLStickAxisX = 0.0f;
	float controllerLStickAxisY = 0.0f;
	float controllerRStickAxisX = 0.0f;
	glm::vec3 currentForce;
	glm::vec3 gasSpeed = glm::vec3(0.f, 2.f, -0.8f);
	glm::vec3 turnSpeed = glm::vec3(2.f, 0.f, 0.f);
	float angleSpeed = 45.f;

	void Update(double seconds);
};

