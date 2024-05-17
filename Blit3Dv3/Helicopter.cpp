#include "Helicopter.h"
extern SceneManager* sceneManager;
extern MeshManager* meshManager;

extern glm::vec3 GRAVITY;

void _print_Vec(glm::vec3 v) {
	std::cout << v.x << " " << v.y << " " << v.z << std::endl;
};


Helicopter::Helicopter(SceneNode* parentNode) {
	// Chopper.s3d
	rootNode = new TranslaterNode(parentNode, glm::vec3(-0.f, -2.5f, -20.f));
	pitchRotNode = new RotatorNode(rootNode, 0.f, -10.f, glm::vec3(1, 0, 0));
	headingRotNode = new RotatorNode(pitchRotNode, 0.f, 0.f, glm::vec3(0, 1, 0));
	rollRotNode = new RotatorNode(headingRotNode, 0.f, 0.f, glm::vec3(0, 0, 1));
	RotatorNode* staticRotator1 = new RotatorNode(rollRotNode, 0.f, 180.f, glm::vec3(0, 1, 1));
	//RotatorNode* staticRotator2 = new RotatorNode(staticRotator1, 0.f, 10.f, glm::vec3(1, 0, 0)); // tilt
	MeshNode* chopperMesh = new MeshNode(staticRotator1, "Data//Chopper.s3d", meshManager);
	// Blade.s3d
	TranslaterNode* mainRotorTN = new TranslaterNode(chopperMesh, glm::vec3(0.f, 0.f, 1.5f));
	mainRotorRotNode = new RotatorNode(mainRotorTN, 2.f, 0.f, glm::vec3(0, 0, 1));
	RotatorNode* staticRotorRotNode = new RotatorNode(mainRotorRotNode, 0.f, 0.f, glm::vec3(0, 1, 0));
	MeshNode* mainRotorMesh = new MeshNode(staticRotorRotNode, "Data//Blade.s3d", meshManager);
	// TailRotor.s3d
	TranslaterNode* tailRotorTN = new TranslaterNode(chopperMesh, glm::vec3(0.4f, 9.f, 0.25f));
	tailRotorRotNode = new RotatorNode(tailRotorTN, 5.f, 0.f, glm::vec3(1, 0, 0));
	RotatorNode* staticTailRotorRotNode = new RotatorNode(tailRotorRotNode, 0.f, 0.f, glm::vec3(1, 0, 0));
	MeshNode* tailRotorMesh = new MeshNode(staticTailRotorRotNode, "Data//TailRotor.s3d", meshManager);
}

void Helicopter::Update(double seconds) {
	// keyboard inputs
	headingRotNode->angle += headRight ? angleSpeed * (float)seconds : 0.f;
	headingRotNode->angle += headLeft ? -angleSpeed * (float)seconds : 0.f;
	
	pitchRotNode->angle += pitchUp ? angleSpeed * (float)seconds : 0.f;
	pitchRotNode->angle += pitchDown ? -angleSpeed * (float)seconds : 0.f;

	rollRotNode->angle += rollLeft ? angleSpeed * (float)seconds : 0.f;
	rollRotNode->angle += rollRight ? -angleSpeed * (float)seconds : 0.f;

	// controller inputs
	headingRotNode->angle += (angleSpeed * controllerLStickAxisX) * (float)seconds;
	pitchRotNode->angle += (angleSpeed * controllerLStickAxisY) * (float)seconds;
	rollRotNode->angle += (angleSpeed * controllerRStickAxisX) * (float)seconds;

	glm::vec3 currGasVector = gasSpeed;

	// apply heading angle into currGasVector
	float headingAngle = glm::radians(headingRotNode->angle);
	float headingX = currGasVector.x * std::cos(headingAngle) + currGasVector.z * std::sin(headingAngle);
	float headingY = currGasVector.y;
	float headingZ = -currGasVector.x * std::cos(headingAngle) + currGasVector.z * std::cos(headingAngle);
	currGasVector = glm::vec3(headingX,
		headingY, headingZ);

	// apply pitch angle into currGasVector
	float pitchAngle = glm::radians(pitchRotNode->angle);
	float pitchX = currGasVector.x;
	float pitchY = currGasVector.y * std::cos(pitchAngle) - currGasVector.z * std::sin(pitchAngle);
	float pitchZ = currGasVector.y * std::sin(pitchAngle) + currGasVector.z * std::cos(pitchAngle);
	currGasVector = glm::vec3(pitchX, pitchY, pitchZ);

	// apply roll angle into currGasVector
	float rollAngle = glm::radians(rollRotNode->angle);
	float rollX = currGasVector.x * std::cos(rollAngle) - currGasVector.y * std::sin(rollAngle);
	float rollY = currGasVector.x * std::sin(rollAngle) + currGasVector.y * std::cos(rollAngle);
	float rollZ = currGasVector.z;
	currGasVector = glm::vec3(rollX, rollY, rollZ);

	//_print_Vec(gasSpeed);
	//_print_Vec(currGasVector);


	currentForce = GRAVITY * (float) seconds;
	if (gas) {
 		currentForce += (currGasVector) * gasScale * (float)seconds;
		mainRotorRotNode->rotationSpeed = 5000;
		tailRotorRotNode->rotationSpeed = 5000;
	}
	else {
		mainRotorRotNode->rotationSpeed = -200;
		tailRotorRotNode->rotationSpeed = -200;
	}

 	rootNode->offset += currentForce * 0.8f;
}


