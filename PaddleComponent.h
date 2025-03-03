#pragma once
#include "Game.h"
#include "GameComponent.h"
#include "SimpleMath.h"
#include "DirectXCollision.h"

using namespace DirectX::SimpleMath;

class PaddleComponent : public GameComponent
{
	ID3D11InputLayout* layout;
	ID3D11RasterizerState* rastState;
	ID3DBlob* vertexBC;
	ID3D11VertexShader* vertexShader;
	ID3DBlob* pixelBC;
	ID3D11PixelShader* pixelShader;
	ID3D11Buffer* vb;
	ID3D11Buffer* ib;
	ID3D11Buffer* constantBuffer;
	UINT strides[1];
	UINT offsets[1];

	float a = 0.8;

	bool isLeft;

public:

	PaddleComponent(Game* game, bool isLeft) : GameComponent(game), isLeft(isLeft) {}

	void Initialize() override;
	void Draw() override;
	virtual void DestroyResources() override;
	virtual void Update(float deltaTime) override;

	bool IsLeft() { return isLeft; };

	void AddY(float y) { data.offset.y += y; collider.Center.y += y; };
	//void AddX(float x) { data.offset.x += x; };

	DirectX::BoundingBox collider;
};

