#ifndef __DIRECTX_H__
#define __DIRECTX_H__

#pragma once

#include "Files.h"		
			
//typedef HRESULT (WINAPI* oDrawIndexedPrimitive) (LPDIRECT3DDEVICE9 pDevice, D3DPRIMITIVETYPE Type, INT BaseVertexIndex, UINT MinVertexIndex, UINT NumVertices, UINT startIndex, UINT primCount);
typedef HRESULT ( WINAPI* oPresent ) ( LPDIRECT3DDEVICE9 pDevice, CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion);
typedef HRESULT ( WINAPI* oReset )( LPDIRECT3DDEVICE9 pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters );

class cDirectx{
public:
			void Hook(void);
			//void InitFonts( void );
			void DrawTri(LPDIRECT3DDEVICE9 dev, int x, int y, int w, int h, DWORD Color);
			HRESULT GenerateTexture(IDirect3DDevice9 *pD3Ddev, IDirect3DTexture9 **ppD3Dtex, DWORD colour32);
	
};

extern cDirectx Directx;

#endif

