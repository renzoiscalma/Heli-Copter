#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "Joystick.h"

std::mutex joystickMutex;

bool Joystick::Init()
{
	//check for joysticks, stop at first joystick found plugged in
	for (joystickNumber = GLFW_JOYSTICK_1; joystickNumber <= GLFW_JOYSTICK_LAST; ++joystickNumber)
	{
		if (glfwJoystickPresent(joystickNumber))
		{
			foundJoystick = true;
			break;
		}
	}

	if (foundJoystick)
	{
		//get the state arrays for this joystick
		if (!PollJoystick()) foundJoystick = false;
	}

	return foundJoystick;
}

void Joystick::ProcessJoystickAxis(float& joystickAxis, const Configuration configuration)
{
	//deadzone

	if (joystickAxis >= -configuration.deadzone &&
		joystickAxis <= configuration.deadzone) joystickAxis = 0.f;
	else
	{
		if (joystickAxis < 0)
			joystickAxis -= configuration.deadzone;
		else
			joystickAxis += configuration.deadzone;

		//scale back into range 0 ... +/-1
		joystickAxis /= (1.f - configuration.deadzone);

		//scaling

		//squared response curve
		/*if (joystickAxis < 0)
		joystickAxis = -joystickAxis * joystickAxis;
		else
		joystickAxis = joystickAxis * joystickAxis;*/

		//cubed response curve
		joystickAxis = joystickAxis * joystickAxis * joystickAxis;
	}
}

bool Joystick::PollJoystick()
{
	if (!glfwJoystickPresent(joystickNumber)) return false;

	//first fetch axis states
	joystickState.axisStates = glfwGetJoystickAxes(joystickNumber, &joystickState.axisCount);
	//now fetch button states
	joystickState.buttonStates = glfwGetJoystickButtons(joystickNumber, &joystickState.buttonCount);

	return true;
}

void Joystick::DoJoystick(void)
{
	if (foundJoystick)
	{

		if (PollJoystick())
		{
			//joystick at joystickNumber is still plugged in, so we can work with it's states
			joystickMutex.lock();
			//axis states are between -1.f and 1.f
			if (joystickState.axisCount > 0) joystickPositionAxis1 = joystickState.axisStates[0];
			if (joystickState.axisCount > 1) joystickPositionAxis2 = joystickState.axisStates[1];
			if (joystickState.axisCount > 2) joystickPositionAxis3 = -joystickState.axisStates[2];
			if (joystickState.axisCount > 3) joystickPositionAxis4 = -joystickState.axisStates[3];


			h->controllerLStickAxisX = joystickState.axisStates[0];
			h->controllerLStickAxisY = joystickState.axisStates[1];
			h->controllerRStickAxisX = -joystickState.axisStates[2];
			//std::cout << joystickPositionAxis1 << " " << joystickPositionAxis2 << " " << joystickPositionAxis3 << " " << joystickPositionAxis4 << std::endl;

			if (joystickState.buttonCount > 6)
			{
				if (joystickState.buttonStates[0] == GLFW_PRESS)
				{
					h->gas = true;
				}
				else if (joystickState.buttonStates[0] == GLFW_RELEASE)
				{
					h->gas = false;
				}
				//pressing BACK button will exit the program
				if (joystickState.buttonStates[6] == GLFW_PRESS)  //start the shutdown sequence
				{
					blit3D->Quit();
				}
			}
			joystickMutex.unlock();
		}
		else foundJoystick = false;
	}
	else
	{
		//search for a joystick again
		joystickMutex.lock();
		//check for joysticks, stop at first joystick found plugged in
		for (joystickNumber = GLFW_JOYSTICK_1; joystickNumber <= GLFW_JOYSTICK_LAST; ++joystickNumber)
		{
			if (glfwJoystickPresent(joystickNumber))
			{
				foundJoystick = true;
				break;
			}
		}

		if (foundJoystick)
		{
			//get the state arrays for this joystick
			if (!PollJoystick()) foundJoystick = false;
		}

		joystickMutex.unlock();
	}
}