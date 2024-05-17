//memory leak detection
#define CRTDBG_MAP_ALLOC
#define _USE_MATH_DEFINES
#ifdef _DEBUG
	#ifndef DBG_NEW
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
		#define new DBG_NEW
	#endif
#endif  // _DEBUG

#include <stdlib.h>
#include <crtdbg.h>
#include <math.h>

#include "Blit3D.h"

#include "SceneNodes.h"

#include "Helicopter.h"
#include "Ground.h"
#include "Config.h"
#include "Joystick.h"

Blit3D *blit3D = NULL;

//GLOBAL DATA
GLSLProgram* prog = NULL;
SceneManager *sceneManager = NULL;
MeshManager* meshManager = NULL;

glm::vec3 cameraOffset;
Configuration configuration;
float moveZ = 0.f;
float moveX = 0.f;
float moveY = 0.f;
float moveSpeed = 4.f;

float elapsedTime = 0;
const float timeSlice = 1.f / 60.f;
glm::vec3 GRAVITY = glm::vec3(0, -1.f, 0);
Helicopter* h;
Ground* groundSet;
Joystick joystick;

void Init()
{
	if (!LoadConfig(configuration))
	{
		std::cout << "ERROR reading the config file!\n\n";
	}
	else
		DisplayConfig(configuration);

	sceneManager = new SceneManager();
	meshManager = new MeshManager(blit3D);

	//joystick stuff
	joystick.Init();

	blit3D->SetMode(Blit3DRenderMode::BLIT3D);

	prog = blit3D->sManager->UseShader("lighting.vert", "lighting.frag"); //load/compile/link

	//3d perspective projection
	blit3D->projectionMatrix = glm::perspective(45.0f, (GLfloat)(blit3D->screenWidth) / (GLfloat)(blit3D->screenHeight), 0.1f, 10000.0f);

	prog->setUniform("projectionMatrix", blit3D->projectionMatrix);
	prog->setUniform("viewMatrix", blit3D->viewMatrix);
	prog->setUniform("modelMatrix", glm::mat4(1.f));

	//lighting variables
	glm::vec3 LightPosition = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 LightIntensity = glm::vec3(1.0f, 1.0f, 1.0f);

	//send lighting info to the shader
	prog->setUniform("LightPosition", LightPosition);
	prog->setUniform("LightIntensity", LightIntensity);

	//send alpha to the shader
	prog->setUniform("in_Alpha", 1.f);

	//attributes
	prog->bindAttribLocation(0, "in_Position");
	prog->bindAttribLocation(1, "in_Normal");
	prog->bindAttribLocation(2, "in_Texcoord");

	prog->printActiveUniforms();
	prog->printActiveAttribs();

	//enable blending
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	h = new Helicopter(sceneManager->sceneGraph);
	groundSet = new Ground(sceneManager->sceneGraph, glm::vec3(-30.f, -15.f, -100.f), 40, 5);
	glClearColor(0.5f, 0.5f, 0.5f, 0.0f);	//clear colour: r,g,b,a
}

void DeInit(void)
{
	if (sceneManager != NULL) delete sceneManager;
	if (meshManager != NULL) delete meshManager;	
	if (h != NULL) delete h;
	if (groundSet != NULL) delete groundSet;
	//any sprites/fonts still allocated are freed automatically by the Blit3D object when we destroy it
}

void Update(double seconds)
{
	if (seconds > 0.15) elapsedTime += 0.15f; //prevent lag spike
	else elapsedTime += static_cast<float>(seconds);

	while (elapsedTime >= timeSlice)
	{
		joystick.DoJoystick();
		elapsedTime -= timeSlice;
		cameraOffset.x += moveX * moveSpeed * timeSlice;
		cameraOffset.y += moveY * moveSpeed * timeSlice;
		cameraOffset.z += moveZ * moveSpeed * timeSlice;
		sceneManager->UpdateWorld(timeSlice);
		h->Update(timeSlice);
		cameraOffset -= h->currentForce * 0.8f;
	}
	
}

void Draw(void)
{
	// wipe the drawing surface clear
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//move camera
	blit3D->viewMatrix = glm::translate(glm::mat4(), cameraOffset);
	prog->setUniform("viewMatrix", blit3D->viewMatrix);

	//draw stuff here
	sceneManager->DrawWorld();
}

//the key codes/actions/mods for DoInput are from GLFW: check its documentation for their values
void DoInput(int key, int scancode, int action, int mods)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		blit3D->Quit(); //start the shutdown sequence

	/*if (key == GLFW_KEY_UP && action == GLFW_PRESS)
	{
		moveZ = 1.f;
	}
	else if (key == GLFW_KEY_UP && action == GLFW_RELEASE)
	{
		moveZ = 0.f;
	}

	if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
	{
		moveZ = -1.f;
	}
	else if (key == GLFW_KEY_DOWN && action == GLFW_RELEASE)
	{
		moveZ = 0.f;
	}

	if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
	{
		moveX = 1.f;
	}
	else if (key == GLFW_KEY_LEFT && action == GLFW_RELEASE)
	{
		moveX = 0.f;
	}

	if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
	{
		moveX = -1.f;
	}
	else if (key == GLFW_KEY_RIGHT && action == GLFW_RELEASE)
	{
		moveX = 0.f;
	}

	if (key == GLFW_KEY_J && action == GLFW_PRESS)
	{
		moveY = 1.f;
	}
	else if (key == GLFW_KEY_J && action == GLFW_RELEASE)
	{
		moveY = 0.f;
	}

	if (key == GLFW_KEY_K && action == GLFW_PRESS)
	{
		moveY = -1.f;
	}
	else if (key == GLFW_KEY_K && action == GLFW_RELEASE)
	{
		moveY = 0.f;
	}*/

	if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
	{
		h->gas = true;
	}
	else if (key == GLFW_KEY_SPACE && action == GLFW_RELEASE)
	{
		h->gas = false;
	}

	if (key == GLFW_KEY_A && action == GLFW_PRESS)
	{
		h->headLeft = true;
	}
	else if (key == GLFW_KEY_A && action == GLFW_RELEASE)
	{
		h->headLeft = false;
	}

	if (key == GLFW_KEY_D && action == GLFW_PRESS)
	{
		h->headRight = true;
	}
	else if (key == GLFW_KEY_D && action == GLFW_RELEASE)
	{
		h->headRight = false;
	}

	if (key == GLFW_KEY_W && action == GLFW_PRESS)
	{
		h->pitchUp = true;
	}
	else if (key == GLFW_KEY_W && action == GLFW_RELEASE)
	{
		h->pitchUp = false;
	}

	if (key == GLFW_KEY_S && action == GLFW_PRESS)
	{
		h->pitchDown = true;
	}
	else if (key == GLFW_KEY_S && action == GLFW_RELEASE)
	{
		h->pitchDown = false;
	}

	if (key == GLFW_KEY_Q && action == GLFW_PRESS)
	{
		h->rollLeft = true;
	}
	else if (key == GLFW_KEY_Q && action == GLFW_RELEASE)
	{
		h->rollLeft = false;
	}

	if (key == GLFW_KEY_E && action == GLFW_PRESS)
	{
		h->rollRight = true;
	}
	else if (key == GLFW_KEY_E && action == GLFW_RELEASE)
	{
		h->rollRight = false;
	}
}

int main(int argc, char *argv[])
{
	//memory leak detection
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//set X to the memory allocation number in order to force a break on the allocation:
	//useful for debugging memory leaks, as long as your memory allocations are deterministic.
	//_crtBreakAlloc = X;

	blit3D = new Blit3D(Blit3DWindowModel::DECORATEDWINDOW, 1024, 1024);

	//set our callback funcs
	blit3D->SetInit(Init);
	blit3D->SetDeInit(DeInit);
	blit3D->SetUpdate(Update);
	blit3D->SetDraw(Draw);
	blit3D->SetDoInput(DoInput);

	//Run() blocks until the window is closed
	blit3D->Run(Blit3DThreadModel::SINGLETHREADED);
	if (blit3D) delete blit3D;
}