#pragma once

#include "SceneManager.h"
#include "Mesh.h"

class TranslaterNode : public SceneNode
{
public:
	//need a vector to translate by
	glm::vec3 offset;
	//override Update() so that we can translate
	void Update(double seconds);
	TranslaterNode();
	TranslaterNode(SceneNode* ParentNode, glm::vec3 offSet);
};

//example node that "draws"
class PrintNode : public SceneNode
{
public:
	void Draw();
	PrintNode();
	PrintNode(SceneNode* ParentNode);
};

class MeshNode : public SceneNode
{
public:
	Mesh* mesh = NULL;
	MeshManager* mManager = NULL;

	void Draw();
	MeshNode();
	~MeshNode();
	MeshNode(std::string fileName, MeshManager* meshManager);
	MeshNode(SceneNode* ParentNode, std::string fileName, MeshManager* meshManager);
};

class RotatorNode : public SceneNode
{
public: 
	// angle in deg
	float angle;
	glm::vec3 axis;
	float rotationSpeed;

	void Draw();
	void Update(double seconds);
	RotatorNode();
	RotatorNode(SceneNode* ParentNode, float rotationSpeed, float angle, glm::vec3 axis);
};

class FloaterNode : public SceneNode
{
public:
	glm::vec3 angle = glm::vec3(0.f,0.f,0.f);
	glm::vec3 angle2 = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 speed = glm::vec3(70.f, 87.7f, 100.f);
	glm::vec3 speed2 = glm::vec3(186.f, 120.f, 76.f);
	glm::vec3 offset = glm::vec3(0.f, 0.f, 0.f);
	glm::vec3 scale = glm::vec3(0.5f, 0.5f, 0.f);
	glm::vec3 scale2 = glm::vec3(0.6f, 0.7f, 0.f);

	void Draw();
	void Update(double seconds);
	FloaterNode();
	FloaterNode(SceneNode* ParentNode);
};