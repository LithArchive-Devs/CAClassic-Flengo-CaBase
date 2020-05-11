#include "HackSprite.h"

inline D3DSprite::SpriteVertex D3DSprite::InitSpriteVertex(const float& x, const float& y, DWORD Color, const float& TexU, const float& TexV)
{
	D3DSprite::SpriteVertex Vertex;
	Vertex.p.x = x;
	Vertex.p.y = y;
	Vertex.p.z = 0.9f;
	Vertex.p.w = 1.0f;
	Vertex.Color = Color;
	Vertex.TexU = TexU;
	Vertex.TexV = TexV;
	return Vertex;
}

D3DSprite::D3DSprite() : vBuff(0), SavedState(0), DrawingState(0), BegunCount(0)
{}

D3DSprite::~D3DSprite()
{
	Destroy();
}

HRESULT D3DSprite::Destroy()
{
	BegunCount = 0;
	if (vBuff != 0)								// Release all classes
	{
		vBuff->Release();
		vBuff = 0;
	}
	if (SavedState != 0)
	{
		SavedState->Release();
		SavedState = 0;
	}
	if (DrawingState != 0)
	{
		DrawingState->Release();
		DrawingState = 0;
	}

	return S_OK;
}

HRESULT D3DSprite::Create(LPDIRECT3DDEVICE9 _pDevice)
{
	pDevice = _pDevice;
												// Create vertex and index buffers for drawing
	HRESULT hr = pDevice->CreateVertexBuffer(sizeof(SpriteVertex) * 6, 0, D3DFVF_SPRITEVERTEX, D3DPOOL_MANAGED, &vBuff, 0);
	if (FAILED(hr))
		return hr;

	for (int i = 0; i < 2; ++i)					// Create state blocks for saving renderstates before drawing and applying states for drawing
	{
		pDevice->BeginStateBlock();

		pDevice->SetTexture(0, 0);

		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_CLIPPING, FALSE);
		pDevice->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
		pDevice->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_FOGENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
		pDevice->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
		pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
		pDevice->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
		pDevice->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS, D3DTTFF_DISABLE);
		pDevice->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
		pDevice->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);
		pDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
		pDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
		pDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);

		if (i == 0)
			pDevice->EndStateBlock(&SavedState);
		else
			pDevice->EndStateBlock(&DrawingState);
	}

	return S_OK;
}

HRESULT D3DSprite::Begin()
{
	if (BegunCount == 0)
	{
		HRESULT hr;
		hr = SavedState->Capture();					// Save the current render states and apply our saved ones
		if (SUCCEEDED(hr))
			hr = DrawingState->Apply();
		if (FAILED(hr))
			return hr;
	}
	BegunCount++;

	return D3D_OK;
}

HRESULT D3DSprite::End()
{
	if (BegunCount == 1)
	{
		HRESULT hr = SavedState->Apply();
		if (FAILED(hr))
			return hr;
	}
	BegunCount--;

	return D3D_OK;
}

bool D3DSprite::IsValid(LPDIRECT3DDEVICE9 _pDevice)
{
	if (_pDevice != 0)
		if (pDevice != _pDevice)
			return false;
	bool Valid = (vBuff != 0 && SavedState != 0 && DrawingState != 0);
	if (_pDevice == 0 || !Valid)
		return Valid;
	LPDIRECT3DDEVICE9 pObjectDev;
	vBuff->GetDevice(&pObjectDev);
	if (pObjectDev != _pDevice)
		return false;
	SavedState->GetDevice(&pObjectDev);
	if (pObjectDev != _pDevice)
		return false;
	DrawingState->GetDevice(&pObjectDev);
	if (pObjectDev != _pDevice)
		return false;
	return true;
}

HRESULT D3DSprite::Draw(LPDIRECT3DTEXTURE9 pTex, int DestX, int DestY, int DestW, int DestH, int TexSrcX, int TexSrcY, int TexSrcW, int TexSrcH, D3DCOLOR ModColor)
{
	HRESULT hr;
	hr = Begin();
	if (FAILED(hr))
		return hr;
	hr = pDevice->SetStreamSource(0, vBuff, 0, sizeof(SpriteVertex));
	if (FAILED(hr))
		goto Cleanup;
	hr = pDevice->SetPixelShader(0);
	if (FAILED(hr))
		goto Cleanup;
	hr = pDevice->SetFVF(D3DFVF_SPRITEVERTEX);
	if (FAILED(hr))
		goto Cleanup;
	hr = pDevice->SetTexture(0, pTex);
	if (FAILED(hr))
		goto Cleanup;

	int TexWidth;
	int TexHeight;

	if (pTex != 0)
	{
		D3DSURFACE_DESC TexDesc;
		pTex->GetLevelDesc(0, &TexDesc);

		TexWidth = TexDesc.Width;
		TexHeight = TexDesc.Height;
	}
	else
	{
		TexWidth = 1;
		TexHeight = 1;
	}

	if (TexSrcW == 0)
		TexSrcW = TexWidth - TexSrcX;
	if (TexSrcH == 0)
		TexSrcH = TexHeight - TexSrcY;
	if (DestW == 0)
		DestW = TexSrcW;
	if (DestH == 0)
		DestH = TexSrcH;

	SpriteVertex* Vertices;
	hr = vBuff->Lock(0, 0, (void**) &Vertices, 0);
	if (FAILED(hr))
		goto Cleanup;

	*(Vertices + 0) = InitSpriteVertex(DestX         - 0.5f, DestY + DestH - 0.5f, ModColor, (float) TexSrcX / TexWidth            , (float) (TexSrcY + TexSrcH) / TexHeight);
	*(Vertices + 1) = InitSpriteVertex(DestX         - 0.5f, DestY         - 0.5f, ModColor, (float) TexSrcX / TexWidth            , (float) TexSrcY / TexHeight            );
	*(Vertices + 3) = InitSpriteVertex(DestX + DestW - 0.5f, DestY         - 0.5f, ModColor, (float) (TexSrcX + TexSrcW) / TexWidth, (float) TexSrcY / TexHeight            );
	*(Vertices + 2) = InitSpriteVertex(DestX + DestW - 0.5f, DestY + DestH - 0.5f, ModColor, (float) (TexSrcX + TexSrcW) / TexWidth, (float) (TexSrcY + TexSrcH) / TexHeight);
	*(Vertices + 4) = *(Vertices + 2);
	*(Vertices + 5) = *(Vertices + 1);

	hr = vBuff->Unlock();
	if (FAILED(hr))
		goto Cleanup;

	pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);

	hr = S_OK;

Cleanup:
	HRESULT hrEnd = End();
	if (FAILED(hrEnd))
		hr = hrEnd;

	return hr;
}