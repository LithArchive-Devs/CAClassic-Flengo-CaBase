#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>

class D3DSprite
{
	struct SpriteVertex
	{
		struct Vector4
		{
			float x;
			float y;
			float z;
			float w;
		};
		Vector4 p;
		DWORD Color;
		float TexU;
		float TexV;
	};
	SpriteVertex InitSpriteVertex(const float& x, const float& y, DWORD Color, const float& TexU, const float& TexV);

	LPDIRECT3DDEVICE9 pDevice;
	LPDIRECT3DVERTEXBUFFER9 vBuff;
	LPDIRECT3DSTATEBLOCK9 SavedState;
	LPDIRECT3DSTATEBLOCK9 DrawingState;
	int BegunCount;

	static const DWORD D3DFVF_SPRITEVERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
public:
	D3DSprite();
	~D3DSprite();

	HRESULT Create(LPDIRECT3DDEVICE9 _pDevice);
	HRESULT Destroy();

	HRESULT Recreate(LPDIRECT3DDEVICE9 _pDevice = 0);

	HRESULT Begin();
	HRESULT End();

	bool IsValid(LPDIRECT3DDEVICE9 _pDevice = 0);

	HRESULT Draw(LPDIRECT3DTEXTURE9 pTex, int DestX, int DestY, int DestW = 0, int DestH = 0, int TexSrcX = 0, int TexSrcY = 0, int TexSrcW = 0, int TexSrcH = 0, D3DCOLOR ModColor = 0xFFFFFFFF);
};