#include "SceneNodes.h"
#include <glm/gtc/matrix_transform.hpp>

//for the PrintNode
#include <iostream>

TranslaterNode::TranslaterNode()
{
	typeId = sceneNodeType::TRANSLATE;

	modelMatrix = glm::mat4(1.f);
	parent = NULL;
}

TranslaterNode::TranslaterNode(SceneNode* ParentNode, glm::vec3 offSet)
{
	typeId = sceneNodeType::TRANSLATE;

	modelMatrix = glm::mat4(1.f);
	offset = offSet;
	//Set parent and add node to parent children
	parent = ParentNode;
	parent->children.push_back(this);
}

void TranslaterNode::Update(double seconds)
{
	assert(parent != NULL && "parent pointer is NULL");
	
	//get parent's modelmatrix
	snodeMutex.lock();	
	parent->snodeMutex.lock();
	//apply modelMatrix from our parent
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();

	//translate
	modelMatrix = glm::translate(modelMatrix, offset);
	snodeMutex.unlock();

	//update all of our children
	for(auto n : children)
	{
		n->Update(seconds);
	}
}

PrintNode::PrintNode()
{
	typeId = sceneNodeType::PRINT;

	modelMatrix = glm::mat4(1.f);
	parent = NULL;
}

PrintNode::PrintNode(SceneNode* ParentNode)
{
	typeId = sceneNodeType::PRINT;

	modelMatrix = glm::mat4(1.f);
	//Set parent and add node to parent children
	parent = ParentNode;
	parent->children.push_back(this);
}

void PrintNode::Draw()
{
	//we would normally use our modelMatrix to draw, so let's calculate where we 
	//would be in the world based on it
	glm::vec4 position = glm::vec4(0, 0, 0, 1);
	
	snodeMutex.lock();
	position = modelMatrix * position;
	snodeMutex.unlock();
	

	std::cout << "Drawing at: " << position.x << ", " << position.y << ", " << position.z << std::endl;
	//draw all of our children
	for(auto n : children)
	{
		n->Draw();
	}
}

MeshNode::~MeshNode()
{
	mManager->unloadMesh(mesh);
};

MeshNode::MeshNode(std::string fileName, MeshManager* meshManager)
{
	mManager = meshManager;
	typeId = sceneNodeType::MESH;
	mesh = mManager->LoadMesh(fileName);
	modelMatrix = glm::mat4(1.f);
	parent = NULL;
}

MeshNode::MeshNode()
{
	typeId = sceneNodeType::MESH;
	mesh = NULL;
	modelMatrix = glm::mat4(1.f);
	parent = NULL;
}

MeshNode::MeshNode(SceneNode* ParentNode, std::string fileName, MeshManager* meshManager)
{
	mManager = meshManager;
	typeId = sceneNodeType::MESH;
	mesh = mManager->LoadMesh(fileName);
	modelMatrix = glm::mat4(1.f);
	//Set parent and add node to parent children
	parent = ParentNode;
	parent->children.push_back(this);
}

void MeshNode::Draw()
{
	mesh->Draw(modelMatrix);

	for (auto& n : children) n->Draw();
}

RotatorNode::RotatorNode() 
{
	typeId = sceneNodeType::ROTATOR;
	this->angle = 0;
	modelMatrix = glm::mat4(1.f);
	this->axis = glm::vec3(0, 0, 0);
	parent = NULL;
}

RotatorNode::RotatorNode(SceneNode* ParentNode, float rotationSpeed, float angle, glm::vec3 axis) {
	typeId = sceneNodeType::ROTATOR;

	modelMatrix = glm::mat4(1.f);
	this->angle = angle;
	this->axis = axis;
	this->rotationSpeed = rotationSpeed;
	//Set parent and add node to parent children
	parent = ParentNode;
	parent->children.push_back(this);
}

void RotatorNode::Update(double seconds) 
{
	assert(parent != NULL && "parent pointer is NULL");

	//get parent's modelmatrix
	snodeMutex.lock();
	parent->snodeMutex.lock();
	//apply modelMatrix from our parent
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();

	angle += rotationSpeed * seconds;
	//translate
	modelMatrix = glm::rotate(modelMatrix, glm::radians(angle), axis);
	snodeMutex.unlock();

	//update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}

void RotatorNode::Draw() {
	//mesh->Draw(modelMatrix);

	for (auto& n : children) n->Draw();
}

FloaterNode::FloaterNode()
{
	typeId = sceneNodeType::FLOATER;
	parent = NULL;
}

FloaterNode::FloaterNode(SceneNode* ParentNode)
{
	typeId = sceneNodeType::FLOATER;
	//angle = glm::vec3(0, 0, 0);
	//angle2 = glm::vec3(0, 0, 0);
	//speed = glm::vec3(1.f, 123.7f, 100.f);
	//speed2 = glm::vec3(1, 235.f, 76.f);
	//offset = glm::vec3(0, 0, 0);
	//scale = glm::vec3(140.f, 140.f, 200.f);
	//scale2 = glm::vec3(20.f, 20.f, 20.f);
	parent = ParentNode;
	parent->children.push_back(this);
}

void FloaterNode::Draw()
{
	for (auto& n : children) n->Draw();
}

void FloaterNode::Update(double seconds)
{
	//change the angle variable based on time passed since last update
	angle.x += static_cast<float>(seconds) * speed.x;
	angle.y += static_cast<float>(seconds) * speed.y;
	angle.z += static_cast<float>(seconds) * speed.z;

	angle2.x += static_cast<float>(seconds) * speed2.x;
	angle2.y += static_cast<float>(seconds) * speed2.y;
	angle2.z += static_cast<float>(seconds) * speed2.z;
	std::cout << angle.x << std::endl;
	//wrap around for angle
	if (angle.x > 360.f) angle.x -= 360.f;
	if (angle.x < 0.f) angle.x += 360.f;

	if (angle2.x > 360.f) angle2.x -= 360.f;
	if (angle2.x < 0.f) angle2.x += 360.f;

	if (angle.y > 360.f) angle.y -= 360.f;
	if (angle.y < 0.f) angle.y += 360.f;

	if (angle2.y > 360.f) angle2.y -= 360.f;
	if (angle2.y < 0.f) angle2.y += 360.f;

	if (angle.z > 360.f) angle.z -= 360.f;
	if (angle.z < 0.f) angle.z += 360.f;

	if (angle2.z > 360.f) angle2.z -= 360.f;
	if (angle2.z < 0.f) angle2.z += 360.f;

	offset.x = cos(glm::radians(angle.x)) * scale.x;
	offset.x += cos(glm::radians(angle2.x)) * scale2.x;

	offset.y = sin(glm::radians(angle.y)) * scale.y;
	offset.y += sin(glm::radians(angle2.y)) * scale2.y;

	//offset.z = tan(glm::radians(angle.z)) * scale.z;
	//offset.z += tan(glm::radians(angle2.z)) * scale2.z;

	//get parent's modelmatrix
	snodeMutex.lock();
	parent->snodeMutex.lock();
	//apply modelMatrix from our parent
	modelMatrix = parent->modelMatrix;
	parent->snodeMutex.unlock();

	//translate
	modelMatrix = glm::translate(modelMatrix, offset);
	snodeMutex.unlock();

	//update all of our children
	for (auto n : children)
	{
		n->Update(seconds);
	}
}