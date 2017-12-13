// DontPanicEpisode1.cpp : Defines the entry point for the console application.
//
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

/**
* Auto-generated code below aims at helping you parse
* the standard input according to the problem statement.
**/
int main()
{
	int nbFloors; // number of floors
	int width; // width of the area
	int exitFloor; // floor on which the exit is found
	int exitPos; // position of the exit on its floor
	int nbRounds; // maximum number of rounds
	int nbTotalClones; // number of generated clones
	int nbAdditionalElevators; // ignore (always zero)
	int nbElevators; // number of elevators
	cin >> nbFloors >> width >> nbRounds >> exitFloor >> exitPos >> nbTotalClones >> nbAdditionalElevators >> nbElevators; cin.ignore();

	int* elevators = new int[nbElevators * 2]();
	int currentElevator = 0;
	for (int i = 0; i < nbElevators; i++) {
		int elevatorFloor; // floor on which this elevator is found
		int elevatorPos; // position of the elevator on its floor
		cin >> elevatorFloor >> elevatorPos; cin.ignore();

		elevators[currentElevator * 2] = elevatorFloor;
		elevators[currentElevator * 2 + 1] = elevatorPos;
		currentElevator++;
	}

	// game loop
	while (true) {
		int cloneFloor; // floor of the leading clone
		int clonePos; // position of the leading clone on its floor
		string direction; // direction of the leading clone: LEFT or RIGHT
		cin >> cloneFloor >> clonePos >> direction; cin.ignore();

		// Write an action using cout. DON'T FORGET THE "<< endl"
		// To debug: cerr << "Debug messages..." << endl;

		string action = "WAIT";
		if (cloneFloor == exitFloor)
		{
			if (clonePos > exitPos)
			{
				if (direction[0] == 'R')
					action = "BLOCK";
			}
			else if (clonePos < exitPos)
			{
				if (direction[0] == 'L')
					action = "BLOCK";
			}
		}
		else
		{
			int elevatorInFloor[2] = { -1, -1 };
			for (int i = 0; i < nbElevators; i++)
			{
				if (elevators[i * 2] == cloneFloor)
				{
					elevatorInFloor[0] = elevators[i * 2];
					elevatorInFloor[1] = elevators[i * 2 + 1];
				}
			}
			if (elevatorInFloor[0] != -1 && elevatorInFloor[1] != -1)
			{
				cerr << "[Debug] " << "Has elevator this floor at pos " << elevatorInFloor[1] << endl;
				cerr << "[Debug] " << "Clone pos " << clonePos << ", direction " << direction << endl;
				if (cloneFloor < exitFloor)
				{
					if (clonePos > elevatorInFloor[1])
					{
						if (direction[0] == 'R')
							action = "BLOCK";
					}
					else if (clonePos < elevatorInFloor[1])
					{
						if (direction[0] == 'L')
							action = "BLOCK";
					}
				}
			}
		}

		cout << action << endl; // action: WAIT or BLOCK
	}
}