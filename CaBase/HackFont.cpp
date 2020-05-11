#include "HackFont.h"

inline D3DFont::FontVertex D3DFont::InitFontVertex(const float& x, const float& y, DWORD Color, const float& TexU, const float& TexV)
{
	D3DFont::FontVertex Vertex;
	Vertex.p.x = x;
	Vertex.p.y = y;
	Vertex.p.z = 0.9f;
	Vertex.p.w = 1.0f;
	Vertex.Color = Color;
	Vertex.TexU = TexU;
	Vertex.TexV = TexV;
	return Vertex;
}

D3DFont::D3DFont() : Height(0), pTex(0), vBuff(0), SavedState(0), DrawingState(0), TexBits(0)
{
	ZeroMemory(CharWidths, 128 * sizeof(ABC));
}

D3DFont::~D3DFont()
{
	Destroy();
}

HRESULT D3DFont::Destroy()
{
	DestroyDeviceObjects();
	if (TexBits != 0)
	{
		delete[] TexBits;
		TexBits = 0;
	}

	return S_OK;
}

HRESULT D3DFont::DestroyDeviceObjects()
{
	if (pTex != 0)								// Release all classes
	{
		pTex->Release();
		pTex = 0;
	}
	if (vBuff != 0)
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

HRESULT D3DFont::Create(LPDIRECT3DDEVICE9 _pDevice, const char* FontName, int FontHeight, DWORD FontFlags)
{
	pDevice = _pDevice;

	Height = FontHeight;

	HDC hdc = CreateCompatibleDC(0);			// Create a DC to draw on

	HFONT hFont = CreateFontA(-FontHeight, 0, 0, 0, (FontFlags & D3DFONT_BOLD) ? FW_BOLD : FW_NORMAL, (FontFlags & D3DFONT_ITALIC), FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, ANTIALIASED_QUALITY, DEFAULT_PITCH | FF_DONTCARE, FontName);
	if (hFont == INVALID_HANDLE_VALUE)
		return GetLastError();

	HGDIOBJ OldFont = SelectObject(hdc, hFont);	// Select font into DC and get char widths for all ASCII characters
	GetCharABCWidthsA(hdc, 0, 128, CharWidths);

	SIZE sz;
	for (char i = 0; i < 127; ++i)
	{
		GetTextExtentPoint32A(hdc, &i, 1, &sz);
//		CharWidths[i].abcB = sz.cx;
	}

	TEXTMETRIC tm;
	GetTextMetrics(hdc, &tm);

	Height = tm.tmHeight;

	TexWidth = 128;
	DWORD MaxHeight = 128;
	DWORD CurrentWidth;
	
	do											// Multiply texture height and width by 2 until all characters will fit on texture
	{
		TexWidth *= 2;
		MaxHeight *= 2;
		TexHeight = 0;
		CurrentWidth = 0;

		for (char i = 0; i < 127; ++i)
		{
			TexCoor[i].top = TexHeight;			// Fill out character texture position
			TexCoor[i].left = CurrentWidth; // TODO: Put in seperate loop so it doesn't fill out for every iteration
			CurrentWidth += CharWidths[i].abcA + CharWidths[i].abcB + CharWidths[i].abcC + 1;
			TexCoor[i].bottom = TexHeight + Height;
			TexCoor[i].right = CurrentWidth;
			CurrentWidth += int(sz.cy * 0.3f + 0.99f);
			if (CurrentWidth > TexWidth)		// If width overflows past the current maximum
			{
				--i;
				CurrentWidth = 0;				// Reset width
				TexHeight += Height + 1;
			}
		}
		TexHeight += Height;
	} while (TexHeight > MaxHeight);
	TexHeight = MaxHeight;

	BITMAPINFO bi;								// Create and initialize a bitmap header
	BITMAPINFOHEADER bih;
	ZeroMemory(&bih, sizeof(BITMAPINFOHEADER));
	bih.biSize = sizeof(BITMAPINFOHEADER);
	bih.biWidth = TexWidth;
	bih.biHeight = TexHeight;
	bih.biPlanes = 1;
	bih.biBitCount = 32;
	bih.biCompression = BI_RGB;
	bih.biClrUsed = 0;
	bih.biClrImportant = 0;
	bi.bmiHeader = bih;

	RGBQUAD* Bits;

	HBITMAP hBmp = CreateDIBSection(hdc, &bi, DIB_RGB_COLORS, (void**) &Bits, 0, 0); // Create bitmap and select it into hdc
	HGDIOBJ OldBmp = SelectObject(hdc, hBmp);

	SetTextAlign(hdc, TA_TOP);					// Align to top
	SetTextColor(hdc, RGB(255, 255, 255));		// White text
	SetBkColor(hdc, RGB(0, 0, 0));				// Black background

	for (char i = 0; i < 127; ++i)				// Loop through all characters and draw them
	{
		::DrawTextA(hdc, &i, 1, &TexCoor[i], 0);
	}
	GdiFlush();									// Flush changes (to bitmap)

	SelectObject(hdc, OldBmp);
	SelectObject(hdc, OldFont);
	DeleteObject(hFont);

	DeleteDC(hdc);
												// Now we have all the character coordinates AND the texture width and height
	HRESULT hr;
	hr = pDevice->CreateTexture(TexWidth, TexHeight, 0, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, &pTex, 0);
	if (FAILED(hr))
	{
		DeleteObject(hBmp);
		return hr;
	}

	TexBits = new WORD[TexWidth * TexHeight];

	D3DLOCKED_RECT LockedRect;					// Lock texture so we can copy the bitmap over
	pTex->LockRect(0, &LockedRect, 0, 0);

	WORD* Current = (WORD*) LockedRect.pBits;
	RGBQUAD BmpCurrent;
	BYTE Alpha;
	for (int y = (int) TexHeight - 1; y >= 0; --y)
	{
		for (int x = 0; x < (int) TexWidth; ++x)
		{										// Loop through and convert the 32 bit bitmap to the 16 bit texture
			BmpCurrent = *(Bits + (y * TexWidth) + x);
			if (BmpCurrent.rgbRed != 0 || BmpCurrent.rgbGreen != 0 || BmpCurrent.rgbBlue != 0)
			{
				Alpha = ((int) BmpCurrent.rgbRed + (int) BmpCurrent.rgbGreen + (int) BmpCurrent.rgbBlue) / 3;
				*Current = ((Alpha / 0x10) << 12) | ((BmpCurrent.rgbRed / 0x10) << 8) | ((BmpCurrent.rgbGreen / 0x10) << 4) | ((BmpCurrent.rgbBlue / 0x10) << 0);
			}
			else
				*Current = 0x0555;
			Current++;
		}
	}

	memcpy(TexBits, LockedRect.pBits, TexWidth * TexHeight * sizeof(WORD));

	pTex->UnlockRect(0);

	DeleteObject(hBmp);
												// Create vertex and index buffers for drawing
	hr = pDevice->CreateVertexBuffer(sizeof(FontVertex) * 60, 0, D3DFVF_FONTVERTEX, D3DPOOL_MANAGED, &vBuff, 0);
	if (FAILED(hr))
	{
		pTex->Release();
		pTex = 0;
		delete[] TexBits;
		TexBits = 0;
		return hr;
	}

	for (int i = 0; i < 2; ++i)					// Create state blocks for saving renderstates before drawing and applying states for drawing
	{
		pDevice->BeginStateBlock();

		pDevice->SetTexture(0, pTex);

		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE );
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC,D3DCMP_GREATEREQUAL);
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
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

HRESULT D3DFont::Recreate(LPDIRECT3DDEVICE9 _pDevice)
{
	if (_pDevice != 0)
		pDevice = _pDevice;

	HRESULT hr;
	hr = pDevice->CreateTexture(TexWidth, TexHeight, 0, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, &pTex, 0);
	if (FAILED(hr))
	{
		return hr;
	}

	D3DLOCKED_RECT LockedRect;					// Lock texture so we can copy the bitmap over
	pTex->LockRect(0, &LockedRect, 0, 0);

	memcpy(LockedRect.pBits, TexBits, TexWidth * TexHeight * sizeof(WORD));

	pTex->UnlockRect(0);
												// Create vertex and index buffers for drawing
	hr = pDevice->CreateVertexBuffer(sizeof(FontVertex) * 60, 0, D3DFVF_FONTVERTEX, D3DPOOL_MANAGED, &vBuff, 0);
	if (FAILED(hr))
	{
		pTex->Release();
		pTex = 0;
		return hr;
	}

	for (int i = 0; i < 2; ++i)					// Create state blocks for saving renderstates before drawing and applying states for drawing
	{
		pDevice->BeginStateBlock();

		pDevice->SetTexture(0, pTex);

		pDevice->SetRenderState(D3DRS_ZENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
		pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
		pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
		pDevice->SetRenderState(D3DRS_ALPHAREF, 0x08);
		pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
		pDevice->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
		pDevice->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
		pDevice->SetRenderState(D3DRS_STENCILENABLE, FALSE);
		pDevice->SetRenderState(D3DRS_CLIPPING, TRUE);
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

DWORD D3DFont::GetStringWidth(const char* Text, int Count)
{
	DWORD StrWidth = 0;
	while (*Text != 0 && *Text != '\n' && Count-- != 0)
	{
//		StrWidth += CharWidths[*Text].abcA + CharWidths[*Text].abcB + CharWidths[*Text].abcC;
		if (*Text <= 127 && *Text >= 0)
			StrWidth += TexCoor[*Text].right - TexCoor[*Text].left - 1;
		Text++;
	}

	return StrWidth;
}

int D3DFont::GetHeight()
{
	return Height;
}

bool D3DFont::IsValid(LPDIRECT3DDEVICE9 _pDevice)
{
	if (_pDevice != 0)
		if (pDevice != _pDevice)
			return false;
	bool Valid = (pTex != 0 && vBuff != 0 && SavedState != 0 && DrawingState != 0);
	if (_pDevice == 0 || !Valid)
		return Valid;
	LPDIRECT3DDEVICE9 pObjectDev;
	pTex->GetDevice(&pObjectDev);
	if (pObjectDev != _pDevice)
		return false;
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

HRESULT D3DFont::DrawText(const char* Text, int Count, const RECT* rc, DWORD Format, D3DCOLOR Color)
{
	HRESULT hr;
	hr = SavedState->Capture();
	if (FAILED(hr))
		return hr;
	hr = DrawingState->Apply();
	if (FAILED(hr))
		return hr;
	hr = pDevice->SetStreamSource(0, vBuff, 0, sizeof(FontVertex));
	if (FAILED(hr))
		goto Cleanup;
	hr = pDevice->SetPixelShader(0);
	if (FAILED(hr))
		goto Cleanup;
	hr = pDevice->SetFVF(D3DFVF_FONTVERTEX);
	if (FAILED(hr))
		goto Cleanup;

	float StartX = float(rc->left);				// Get the starting x value depending on the flags
	float StartY = float(rc->top);
	if ((Format & DT_RIGHT) > 0)
	{
		StartX = rc->left + float((rc->right - rc->left) - GetStringWidth(Text, Count));
	}
	else if ((Format & DT_CENTER) > 0)
	{
		StartX = rc->left + float((rc->right - rc->left) - GetStringWidth(Text, Count)) / 2.0f + 0.5f;
	}
												// Lock buffers
	FontVertex* Vertices;
	hr = vBuff->Lock(0, 0, (void**) &Vertices, 0);
	if (FAILED(hr))
		goto Cleanup;
	int VertOffset = 0;

	float CharWidth;
	float CharHeight;

	while (*Text != 0 && Count-- != 0)
	{
		if (*Text > 127 || *Text < 0)
		{
			Text++;
			continue;
		}

		if (*Text == '\n')						// Newline character, move to next line and reset X
		{
			Text++;
			StartX = (float) rc->left;
			if ((Format & DT_RIGHT) > 0)
			{
				StartX = float((rc->right - rc->left) - GetStringWidth(Text, Count));
			}
			else if ((Format & DT_CENTER) > 0)
			{
				StartX = float((rc->right - rc->left) - GetStringWidth(Text, Count)) / 2.0f + 0.5f;
			}

			StartY += (float) Height;

			continue;
		}
												// Use our saved character widths to create the vertexes
		CharWidth = (float) TexCoor[*Text].right - TexCoor[*Text].left;
		CharHeight = (float) TexCoor[*Text].bottom - TexCoor[*Text].top;

		*(Vertices + VertOffset + 0) = InitFontVertex(StartX             - 0.5f, StartY + CharHeight - 0.5f, Color, (float) TexCoor[*Text].left / TexWidth , (float) TexCoor[*Text].bottom / TexHeight);
		*(Vertices + VertOffset + 1) = InitFontVertex(StartX             - 0.5f, StartY              - 0.5f, Color, (float) TexCoor[*Text].left / TexWidth , (float) TexCoor[*Text].top / TexHeight);
		*(Vertices + VertOffset + 3) = InitFontVertex(StartX + CharWidth - 0.5f, StartY              - 0.5f, Color, (float) TexCoor[*Text].right / TexWidth, (float) TexCoor[*Text].top / TexHeight);
		*(Vertices + VertOffset + 2) = InitFontVertex(StartX + CharWidth - 0.5f, StartY + CharHeight - 0.5f, Color, (float) TexCoor[*Text].right / TexWidth, (float) TexCoor[*Text].bottom / TexHeight);
		*(Vertices + VertOffset + 4) = *(Vertices + VertOffset + 2);
		*(Vertices + VertOffset + 5) = *(Vertices + VertOffset + 1);

		VertOffset += 6;

		if (VertOffset > 60 - 6)
		{
			hr = vBuff->Unlock();				// Unlock and draw
			if (FAILED(hr))
				goto Cleanup;

			hr = pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, VertOffset / 3);
			if (FAILED(hr))
				goto Cleanup;

			hr = vBuff->Lock(0, 0, (void**) &Vertices, 0);
			if (FAILED(hr))
				goto Cleanup;

			VertOffset = 0;
		}
												// Adjust to next character
		StartX += CharWidth - 1;

		Text++;
	}

	vBuff->Unlock();

	if (VertOffset > 0)
		pDevice->DrawPrimitive(D3DPT_TRIANGLELIST, 0, VertOffset / 3);

Cleanup:
	HRESULT hrEnd = SavedState->Apply();
	if (FAILED(hrEnd))
		hr = hrEnd;

	return hr;
}

void D3DFont::SaveToBitmap(void* pBits, BITMAPINFOHEADER& bih)
{
	HANDLE hFile = CreateFileA("tex_bmp.bmp", GENERIC_WRITE | GENERIC_READ, 0, 0, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	DWORD BytesWritten;

	bih.biSizeImage = bih.biWidth * bih.biHeight * (bih.biBitCount / 8);
	
	BITMAPFILEHEADER bfh;
	bfh.bfType = 0x4D42;
	bfh.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);
	bfh.bfSize = bfh.bfOffBits + bih.biSizeImage;

	WriteFile(hFile, &bfh, sizeof(BITMAPFILEHEADER), &BytesWritten, 0);
	WriteFile(hFile, &bih, sizeof(BITMAPINFOHEADER), &BytesWritten, 0);
	WriteFile(hFile, pBits, bih.biSizeImage, &BytesWritten, 0);
	CloseHandle(hFile);
}