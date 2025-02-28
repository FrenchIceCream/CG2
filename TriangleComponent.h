#pragma once
#include "Game.h"
#include "GameComponent.h"

class TriangleComponent: public GameComponent
{
	ID3D11InputLayout* layout;
	ID3D11RasterizerState* rastState;
	ID3DBlob* vertexBC;
	ID3D11VertexShader* vertexShader;
	ID3DBlob* pixelBC;
	ID3D11PixelShader* pixelShader;
	ID3D11Buffer* vb;
	ID3D11Buffer* ib;

	UINT strides[1];
	UINT offsets[1];

public:

	TriangleComponent(Game* game) : GameComponent(game) {}

	void Initialize() override;
	void Draw() override;
	virtual void DestroyResources() override;
	virtual void Update() override;
};

