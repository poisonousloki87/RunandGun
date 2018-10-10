#include "PacMan.h"

PacMan::PacMan()
{
}


PacMan::~PacMan()
{
}
	
int map[21][21] = {
{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
{ 1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1 },
{ 1,1,4,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,4,1,1 },
{ 1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
{ 1,1,0,1,1,0,1,0,1,1,1,1,1,0,1,0,1,1,0,1,1 },
{ 1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1 },
{ 1,1,1,1,1,0,1,1,1,2,1,2,1,1,1,0,1,1,1,1,1 },
{ 1,1,1,1,1,0,1,2,2,2,9,2,2,2,1,0,1,1,1,1,1 },
{ 1,1,1,1,1,0,1,2,1,1,3,1,1,2,1,0,1,1,1,1,1 },
{ 1,1,1,1,1,0,1,2,1,9,9,9,1,2,1,0,1,1,1,1,1 },
{ 1,1,1,1,1,0,1,2,1,1,1,1,1,2,1,0,1,1,1,1,1 },
{ 1,1,1,1,1,0,1,2,2,2,2,2,2,2,1,0,1,1,1,1,1 },
{ 1,1,1,1,1,0,1,2,1,1,1,1,1,2,1,0,1,1,1,1,1 },
{ 1,1,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,1 },
{ 1,1,0,1,1,0,1,1,1,0,1,0,1,1,1,0,1,1,0,1,1 },
{ 1,1,4,0,1,0,0,0,0,0,8,0,0,0,0,0,1,0,4,1,1 },
{ 1,1,1,0,1,1,1,0,1,1,1,1,1,0,1,0,1,0,1,1,1 },
{ 1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,1,1 },
{ 1,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,1,1,0,1,1 },
{ 1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1 },
{ 1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1 },
};

//TOP GHOST 1
void GhostAI1(int playerR, int playerC)
{
	//location of ghost
	int G1R = 0;
	int G1C = 0;

	//location of player
	int playerLocationR = playerR;
	int playerLocationC = playerC;

	int direction = NULL;

	if ((playerLocationR >= 2 && playerLocationC >= 1) && (playerLocationR <= 11 && playerLocationC <= 11))
	{
		//FOLLOW PLAYER
		//direction = rand() % 100 + 1;

		if (playerLocationR > G1R)
		{
			if (G1R + 1 == 0 || G1R == 2)
				G1R -= 1;

		}
		/*
		if (direction > 0 && direction < 25)
		{

		}
		else if (direction > 25 && direction < 50)
		{

		}
		else if (direction > 50 && direction < 75)
		{

		}
		else
		{

		}
		*/
	}
	else
	{
		//RANDOM WALK
	}
}

void GhostAI2()
{
	//location of ghost
	int G2R;
	int G2C;

	//location of player
	int playerLocationR;
	int playerLocationC;
}

void GhostAI3()
{
	//location of ghost
	int G3R;
	int G3C;

	//location of player
	int playerLocationR;
	int playerLocationC;
}

void GhostAI4()
{
	//location of ghost
	int G4R;
	int G4C;

	//location of player
	int playerLocationR;
	int playerLocationC;
}


void printDungeon(int(*a)[21])
{
	//print out data map
	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			if (*(*(a + i) + j) > 0)
				cout << *(*(a + i) + j);
			else
				cout << " ";
		}
		cout << endl;
	}

	cout << "\n\n\n\n\n";

	for (int i = 0; i < 21; i++)
	{
		for (int j = 0; j < 21; j++)
		{
			if (a[i][j] == 0 || a[i][j] == 2 || a[i][j] == 4)
				cout << "#";
			else if (a[i][j] == 8)
			{
				cout << "P";
			}
			else if (a[i][j] == 9)
			{
				cout << "G";
			}
			else
				cout << ".";
		}
		cout << endl;
	}
}