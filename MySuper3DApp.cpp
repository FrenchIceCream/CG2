// MySuper3DApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>
#include <chrono>
#include "Game.h"
#include "TriangleComponent.h"
#include "PaddleComponent.h"
#include "BallComponent.h"


int main()
{
	srand(std::time(NULL));
	Game * MyGame = new Game(L"Pong", 700, 700);

	MyGame->gameComponents.push_back(new PaddleComponent(MyGame, true));
	MyGame->gameComponents.push_back(new PaddleComponent(MyGame, false));
	MyGame->gameComponents.push_back(new BallComponent(MyGame));

	MyGame->Run();

    std::cout << "\nFINAL SCORE: ";
	MyGame->PrintScore();
}