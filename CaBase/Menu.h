#pragma once

#ifndef HACKMENU_H
#define HACKMENU_H

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "HackFont.h"
#include "HackSprite.h"

enum MenuFlags
{
	MenuFlagNormal = 0,
	MenuFlagAlwaysDisplay = 1,
	MenuFlagDontDisplayZero = 2,
	MenuFlagSlider = 4,
};

class HackMenu;

typedef void (*HackProc)(int Value);
struct MenuItem
{
	char* Name;
	int Current;
	int Min;
	int Max;
	char* Display;
	int Flags;
	HackProc ChangeProc;
};

struct MenuTab
{
	char* Name;
	int Pos;
};

struct MenuHeader
{
	char* Name;
	bool End;
};

class HackMenu
{
	int NumItems;
	int NumTabs;
	int NumHeaders;
	int CurrentTab;

	bool Active;
	bool Show;
	int XPos;
	int YPos;
	int Width;
	int Selected;

	int AnimFrame;
	int AnimDelta;

	int CachedTab;
	int CachedHeight;

	HANDLE hChangeEvent;

	int GetTabStart(int Tab);
	int GetTabEnd(int Tab);
	int GetHeaderSize(bool Bottom);

	int GetCurrentHeight();
public:
	D3DSprite* pSprite;
	D3DCOLOR BackColor;
	D3DFont* pFont;

	LPDIRECT3DDEVICE9 pDevice;

	MenuItem Items[56];
	MenuTab Tabs[8];
	MenuHeader Headers[4];

	HackMenu();
	~HackMenu();

	bool Render();
	void AddItem(char* _Name, int _Min, int _Max, int _Begin, char* _Display, HackProc _ChangeProc = 0, bool AlwaysDisplay = false, bool DontDisplayZero = false, bool Slider = false);
	void AddHeader(char* _Name);
	void AddTab(char* _Name);
	void SetChangeEvent(HANDLE hEvent);
	HANDLE GetChangeEvent();

	int GetNumItems();

	void SetWidth(int _Width);
	void SetMenuPos(int X, int Y);
	void ShowMenu(bool Visible);
	bool IsShown();

	void CallChangeProc(int Index);

	MenuItem& operator[](int Index);
};

#endif