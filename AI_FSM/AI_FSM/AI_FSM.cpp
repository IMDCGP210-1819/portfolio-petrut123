// AI_FSM.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <Windows.h>

enum States
{
	Idle,
	MoveUp,
	MoveDown,
	OpenDoors
};

int main()
{
	int currentFloor = 0;
	States currentState = Idle;
	int desiredFloor = 0;

	//Default floor is ground floor
	std::cout << "Current floor is:";
	std::cout << 0 << std::endl;

	while (!GetKeyState('R')) //Wait for key press
	{
		switch (currentState)
		{
		case Idle:
		{
			// randomly generate a floor number
			// if different from current floor, open doors
			desiredFloor = rand() % 5;
			std::cout << "Idle State" << std::endl;
			if (desiredFloor != currentFloor)
			{
				currentState = OpenDoors;
			}
			else
			{
				std::cout << "You entered the same floor" << std::endl;
			}

			break;
		}
		case MoveUp:
			//Sets the current floor and then open doors
			currentFloor = desiredFloor;
			std::cout << "Current floor is: ";
			std::cout << currentFloor << std::endl;
			currentState = OpenDoors;
			break;
		case MoveDown:
			//Sets the current floor and then open doors
			currentFloor = desiredFloor;
			std::cout << "Current floor is: ";
			std::cout << currentFloor << std::endl;
			currentState = OpenDoors;
			break;
		case OpenDoors:
			//This state is called twice, once when the lift has to move and once when the lift arrived to the destination
			//When it has to move it figures out where it should go (up / down)
			//When it arrived it goes into idle
			std::cout << "Doors opened" << std::endl;
			if (desiredFloor > currentFloor)
			{
				std::cout << "Going Up" << std::endl;
				currentState = MoveUp;
			}
			else if (desiredFloor < currentFloor)
			{
				std::cout << "Going Down" << std::endl;
				currentState = MoveDown;
			}
			else
			{
				currentState = Idle;
			}
			break;
		default:
			break;
		}
	}
	std::cin.get();

}



