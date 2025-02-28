// MySuper3DApp.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <iostream>
#include <chrono>
#include "Game.h"
#include "TriangleComponent.h"
#include "GameComponent.h"


int main()
{
	Game * MyGame = new Game(L"My3DApp", 800, 800);

	MyGame->gameComponents.push_back(new TriangleComponent(MyGame));

	MyGame->Run();

    std::cout << "Hello World!\n";
}
