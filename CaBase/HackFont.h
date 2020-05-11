#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <d3d9.h>

#define D3DFONT_BOLD 1
#define D3DFONT_ITALIC 2

class D3DFont
{
	struct FontVertex
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
	FontVertex InitFontVertex(const float& x, const float& y, DWORD Color, const float& TexU, const float& TexV);

	int Height;
	LPDIRECT3DDEVICE9 pDevice;
	LPDIRECT3DTEXTURE9 pTex;
	DWORD TexWidth;
	DWORD TexHeight;
	LPDIRECT3DVERTEXBUFFER9 vBuff;
	LPDIRECT3DSTATEBLOCK9 SavedState;
	LPDIRECT3DSTATEBLOCK9 DrawingState;
	ABC CharWidths[128];
	RECT TexCoor[128];
	WORD* TexBits;

	static const DWORD D3DFVF_FONTVERTEX = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;
	void SaveToBitmap(void* pBits, BITMAPINFOHEADER& bih);
public:
	D3DFont();
	~D3DFont();

	HRESULT Create(LPDIRECT3DDEVICE9 _pDevice, const char* FontName, int FontHeight, DWORD FontFlags);
	HRESULT Destroy();

	HRESULT Recreate(LPDIRECT3DDEVICE9 _pDevice = 0);
	HRESULT DestroyDeviceObjects();

	DWORD GetStringWidth(const char* Text, int Count = -1);
	int GetHeight();

	bool IsValid(LPDIRECT3DDEVICE9 _pDevice = 0);

	HRESULT DrawText(const char* Text, int Count, const RECT* rc, DWORD Format, D3DCOLOR Color);
};