#include "pch.h"
#include "BallComponent.h"
#include "Game.h"
#include <directxmath.h>
#include "PaddleComponent.h"
//#include "DebugDraw.h"

void BallComponent::ResetBall()
{
	data.offset.x = 0;
	data.offset.y = 0;

	collider.Center.x = collider.Center.y = 0;
	collider.Extents.x = collider.Extents.y = 0.05;
}

void BallComponent::Initialize()
{
	ResetBall();

	a = 1;

	topBox.Center.x = bottomBox.Center.x = 0;
	topBox.Extents.y = bottomBox.Extents.y = 0.01f;
	topBox.Center.y = 1.f;
	topBox.Extents.x = bottomBox.Extents.x = 1.f;
	bottomBox.Center.y = -1.f;

	leftBox.Center.y = rightBox.Center.y = 0;
	leftBox.Center.x = -1.f;
	rightBox.Center.x = 1.f;
	leftBox.Extents.y = rightBox.Extents.y = 1.f;
	leftBox.Extents.x = rightBox.Extents.x = 0.01f;

	ID3DBlob* errorVertexCode = nullptr;

	auto res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl",
		nullptr /*macros*/,
		nullptr /*include*/,
		"VSMain",
		"vs_5_0",
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION,
		0,
		&vertexBC,
		&errorVertexCode);

	if (FAILED(res)) {
		// If the shader failed to compile it should have written something to the error message.
		if (errorVertexCode) {
			char* compileErrors = (char*)(errorVertexCode->GetBufferPointer());

			std::cout << compileErrors << std::endl;
		}
		// If there was  nothing in the error message then it simply could not find the shader file itself.
		else
		{
			MessageBox(game->Display->hWnd, L"MyVeryFirstShader.hlsl", L"Missing Shader File", MB_OK);
		}

		return;
	}

	ID3DBlob* errorPixelCode;
	res = D3DCompileFromFile(L"./Shaders/MyVeryFirstShader.hlsl", nullptr, nullptr /*include*/, "PSMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, &pixelBC, &errorPixelCode);

	game->device->CreateVertexShader(
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		nullptr, &vertexShader);

	game->device->CreatePixelShader(
		pixelBC->GetBufferPointer(),
		pixelBC->GetBufferSize(),
		nullptr, &pixelShader);

	D3D11_INPUT_ELEMENT_DESC inputElements[] = {
		D3D11_INPUT_ELEMENT_DESC {
			"POSITION",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			0,
			D3D11_INPUT_PER_VERTEX_DATA,
			0},
		D3D11_INPUT_ELEMENT_DESC {
			"COLOR",
			0,
			DXGI_FORMAT_R32G32B32A32_FLOAT,
			0,
			D3D11_APPEND_ALIGNED_ELEMENT,
			D3D11_INPUT_PER_VERTEX_DATA,
			0}
	};

	game->device->CreateInputLayout(
		inputElements,
		2,
		vertexBC->GetBufferPointer(),
		vertexBC->GetBufferSize(),
		&layout);

	DirectX::XMFLOAT4 points[8] = {
		DirectX::XMFLOAT4(0.05f, 0.05f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.05f, -0.05f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),
		DirectX::XMFLOAT4(0.05f, -0.05f, 0.5f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),
		DirectX::XMFLOAT4(-0.05f, 0.05f, 0.5f, 1.0f),	DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f),
	};

	D3D11_BUFFER_DESC constBuffDesc;
	constBuffDesc.Usage = D3D11_USAGE_DYNAMIC;
	constBuffDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constBuffDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	constBuffDesc.MiscFlags = 0;
	constBuffDesc.StructureByteStride = 0;
	constBuffDesc.ByteWidth = sizeof(ConstData);

	game->device->CreateBuffer(&constBuffDesc, nullptr, &constantBuffer);



	D3D11_BUFFER_DESC vertexBufDesc = {};
	vertexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufDesc.CPUAccessFlags = 0;
	vertexBufDesc.MiscFlags = 0;
	vertexBufDesc.StructureByteStride = 0;
	vertexBufDesc.ByteWidth = sizeof(DirectX::XMFLOAT4) * std::size(points);

	D3D11_SUBRESOURCE_DATA vertexData = {};
	vertexData.pSysMem = points;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	game->device->CreateBuffer(&vertexBufDesc, &vertexData, &vb);


	int indeces[] = { 0,1,2, 0, 1, 3 };
	D3D11_BUFFER_DESC indexBufDesc = {};
	indexBufDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufDesc.CPUAccessFlags = 0;
	indexBufDesc.MiscFlags = 0;
	indexBufDesc.StructureByteStride = 0;
	indexBufDesc.ByteWidth = sizeof(int) * std::size(indeces);

	D3D11_SUBRESOURCE_DATA indexData = {};
	indexData.pSysMem = indeces;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	game->device->CreateBuffer(&indexBufDesc, &indexData, &ib);

	CD3D11_RASTERIZER_DESC rastDesc = {};
	rastDesc.CullMode = D3D11_CULL_NONE;
	rastDesc.FillMode = D3D11_FILL_SOLID;

	*strides = 32;
	*offsets = 0;

	res = game->device->CreateRasterizerState(&rastDesc, &rastState);
}

void BallComponent::Draw()
{
	game->context->RSSetState(rastState);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = static_cast<float>(game->Display->clientWidth);
	viewport.Height = static_cast<float>(game->Display->clientHeight);
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1.0f;

	game->context->RSSetViewports(1, &viewport);

	game->context->IASetInputLayout(layout);
	game->context->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	game->context->IASetIndexBuffer(ib, DXGI_FORMAT_R32_UINT, 0);
	game->context->IASetVertexBuffers(0, 1, &vb, strides, offsets);
	game->context->VSSetShader(vertexShader, nullptr, 0);
	game->context->PSSetShader(pixelShader, nullptr, 0);


	D3D11_MAPPED_SUBRESOURCE res = {};
	game->context->Map(constantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &res);
	auto dataPtr = reinterpret_cast<float*>(res.pData);
	memcpy(dataPtr, &data, sizeof(ConstData));
	game->context->Unmap(constantBuffer, 0);
	game->context->VSSetConstantBuffers(0, 1, &constantBuffer);
	
	game->context->DrawIndexed(6, 0, 0);

	if (lockScore)
	{
		
		lockScore = false;
	}
}

void BallComponent::DestroyResources()
{
	vertexShader->Release();
	vertexBC->Release();
	vb->Release();
	layout->Release();
	pixelShader->Release();
	pixelBC->Release();
	rastState->Release();
	ib->Release();
}

void BallComponent::Update(float deltaTime)
{
	for (GameComponent* comp : game->gameComponents)
	{
		if (auto c = dynamic_cast<PaddleComponent*>(comp))
		{
			if (collider.Intersects(c->collider))
			{
				DirectX::SimpleMath::Vector2 vec(collider.Center.x - c->collider.Center.x, collider.Center.y - c->collider.Center.y);
				vec.Normalize();
				a =  vec.x;
				b = vec.y;
			}
		}
			
	}

	if (lockScore)
		return;

	float dist = 2.f;

	if (collider.Intersects(bottomBox))
	{
		b = -b;
	}
	if (collider.Intersects(topBox))
	{
		b = -b;
	}
	if (collider.Intersects(leftBox))
	{
		game->UpdateScore(false);
		game->PrintScore();
		ResetBall();
		lockScore = true;
		//std::cout << "left\n";
	}
	if (collider.Intersects(rightBox))
	{
		game->UpdateScore(true);
		game->PrintScore();
		ResetBall();
		lockScore = true;
		//std::cout << "right\n";
	}
	
	float deltaX = a * deltaTime;
	float deltaY = b * deltaTime;
	data.offset.x = data.offset.x + deltaX;
	data.offset.y = data.offset.y + deltaY;

	collider.Center.x = collider.Center.x + deltaX;
	collider.Center.y = collider.Center.y + deltaY;
	//std::cout << collider.Center.x << "\n";
}