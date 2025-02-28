#pragma once
#include "SimpleMath.h"

using namespace DirectX::SimpleMath;

class Game;

class GameComponent
{
public:

	Game* game;

	GameComponent(Game* game) : game(game) {}

	virtual void DestroyResources() = 0;
	virtual void Draw() = 0;
	virtual void Initialize() = 0;
	virtual void Update() = 0;

	struct ConstData
	{
		Vector4 offset;
	};

	ConstData data;
};

