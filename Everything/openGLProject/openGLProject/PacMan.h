#pragma once

#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

class PacMan
{
public:
	PacMan();
	~PacMan();

	//Prototypes
	void printDungeon(int(*a)[21]);
	void GhostAI1(int, int);
	void GhostAI2(int, int);
	void GhostAI3(int, int);
	void GhostAI4(int, int);
};

