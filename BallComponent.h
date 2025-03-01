#pragma once
#include "Game.h"
#include "GameComponent.h"
#include "SimpleMath.h"
#include "DirectXCollision.h"

using namespace DirectX::SimpleMath;

class BallComponent : public GameComponent
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


	DirectX::BoundingBox collider;

	DirectX::BoundingBox topBox;
	DirectX::BoundingBox bottomBox;
	DirectX::BoundingBox leftBox;
	DirectX::BoundingBox rightBox;

public:

	BallComponent(Game* game) : GameComponent(game){}

	void Initialize() override;
	void Draw() override;
	virtual void DestroyResources() override;
	virtual void Update() override;

	void AddY(float y) { data.offset.y += y; };
	void AddX(float x) { data.offset.x += x; };
};

