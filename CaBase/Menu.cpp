#include <windows.h>
#include "Menu.h"

HackMenu::HackMenu() : Show(false), Active(false), NumItems(0), NumTabs(0), NumHeaders(0), CurrentTab(0), XPos(0), YPos(0), Width(220), Selected(0), pDevice(0), AnimFrame(0), AnimDelta(0), pFont(0), pSprite(0), CachedTab(-1), CachedHeight(0), hChangeEvent(0)
{}

HackMenu::~HackMenu()
{
}

void HackMenu::AddItem(char* _Name, int _Min, int _Max, int _Begin, char* _Display, HackProc _ChangeProc, bool AlwaysDisplay, bool DontDisplayZero, bool Slider)
{
	Items[NumItems].Name = _Name;
	Items[NumItems].Min = _Min;
	Items[NumItems].Max = _Max;
	Items[NumItems].Current = _Begin;
	Items[NumItems].Display = _Display;
	Items[NumItems].Flags = (AlwaysDisplay ? MenuFlagAlwaysDisplay : MenuFlagNormal) | (DontDisplayZero ? MenuFlagDontDisplayZero : 0) | (Slider ? MenuFlagSlider : 0);
	Items[NumItems].ChangeProc = _ChangeProc;
	NumItems++;
}

void HackMenu::AddHeader(char* _Name)
{
	Headers[NumHeaders].Name = _Name;
	Headers[NumHeaders].End = (NumItems > 0);
	NumHeaders++;
}

void HackMenu::AddTab(char* _Name)
{
	Tabs[NumTabs].Name = _Name;
	Tabs[NumTabs].Pos = NumItems;
	NumTabs++;
}

int HackMenu::GetNumItems()
{
	return NumItems;
}

void HackMenu::SetWidth(int _Width)
{
	Width = _Width;
}

void HackMenu::SetMenuPos(int X, int Y)
{
	XPos = X;
	YPos = Y;
}

int HackMenu::GetTabStart(int Tab)
{
	return Tabs[Tab].Pos;
}

int HackMenu::GetTabEnd(int Tab)
{
	if (NumTabs > Tab + 1)
		return Tabs[Tab + 1].Pos - 1;
	else
		return NumItems - 1;
}

int HackMenu::GetHeaderSize(bool Bottom)
{
	int Size = 0;
	for (int i = 0; i < NumHeaders; ++i)
		if (Headers[i].End == Bottom)
			Size += 15;
	return Size;
}

int HackMenu::GetCurrentHeight()
{
	if (CurrentTab == CachedTab)
		return CachedHeight;
	else
	{
		CachedTab = CurrentTab;
		int TabStart = GetTabStart(CurrentTab);
		int TabEnd = GetTabEnd(CurrentTab);
		CachedHeight = 13 * (TabEnd - TabStart + 1) + 30 + 13 * NumHeaders;

		for (int i = TabStart; i <= TabEnd; ++i)
			if (Items[i].Flags & MenuFlagSlider)
				CachedHeight += 13;

		return CachedHeight;
	}
}

void HackMenu::ShowMenu(bool Visible)
{
	Show = Visible;
	GetAsyncKeyState(VK_UP);
	GetAsyncKeyState(VK_DOWN);
	GetAsyncKeyState(VK_LEFT);
	GetAsyncKeyState(VK_RIGHT);
}

bool HackMenu::IsShown()
{
	return Show;
}

void HackMenu::SetChangeEvent(HANDLE hEvent)
{
	hChangeEvent = hEvent;
}

HANDLE HackMenu::GetChangeEvent()
{
	return hChangeEvent;
}

void HackMenu::CallChangeProc(int Index)
{
	if (Items[Index].ChangeProc != 0)
		Items[Index].ChangeProc(Items[Index].Current);
}

MenuItem& HackMenu::operator[](int Index)
{
	return Items[Index];
}

bool HackMenu::Render()
{
	if (Show)
	{
		int OldSelected = Selected;
		if (Selected > 0 && (GetAsyncKeyState(VK_UP) & 1))
		{
			Selected--;
			if (Selected < 0)
				Selected = OldSelected;
			else if (Selected < GetTabStart(CurrentTab))
				CurrentTab--;
			GetAsyncKeyState(VK_DOWN);
		}

		if (Selected < NumItems && (GetAsyncKeyState(VK_DOWN) & 1))
		{
			Selected++;
			if (Selected == NumItems)
				Selected = OldSelected;
			else if (Selected > GetTabEnd(CurrentTab))
				CurrentTab++;
			GetAsyncKeyState(VK_UP);
		}

		if (GetAsyncKeyState(VK_LEFT) & 1)
		{
			if (Items[Selected].Min < Items[Selected].Current)
			{
				Items[Selected].Current--;
				CallChangeProc(Selected);
			}
			if (hChangeEvent != 0)
				SetEvent(hChangeEvent);
		}

		if (GetAsyncKeyState(VK_RIGHT) & 1)
		{
			if (Items[Selected].Max > Items[Selected].Current)
			{
				Items[Selected].Current++;
				CallChangeProc(Selected);
			}
			if (hChangeEvent != 0)
				SetEvent(hChangeEvent);
		}

		if (GetAsyncKeyState(VK_MBUTTON) & 1)
		{
			POINT pt;
			GetCursorPos(&pt);
			ScreenToClient(GetActiveWindow(), &pt);
			pt.y -= YPos + GetHeaderSize(false);
			pt.x -= XPos;
			if (pt.x > 0 && pt.x < Width && pt.y > 0 && pt.y < (GetTabEnd(CurrentTab) - GetTabStart(CurrentTab) + 1) * 13 + 17)
			{
				if (pt.y < 17)
				{
					CurrentTab = pt.x / (Width / NumTabs);
					Selected = GetTabStart(CurrentTab);
				}
				else
				{
					pt.y -= 18;
					pt.y /= 13;
					pt.y += GetTabStart(CurrentTab);
					if (Items[pt.y].Max == 1)
					{
						Items[pt.y].Current = (int) (Items[pt.y].Current == 0);
						CallChangeProc(pt.y);
					}
					else
					{
						if (pt.x < 20)
						{
							if (Items[pt.y].Min < Items[pt.y].Current)
							{
								Items[pt.y].Current--;
								CallChangeProc(pt.y);
							}
						}
						else if (pt.x > Width - 20)
						{
							if (Items[pt.y].Max > Items[pt.y].Current)
							{
								Items[pt.y].Current++;
								CallChangeProc(pt.y);
							}
						}
					}
					if (hChangeEvent != 0)
						SetEvent(hChangeEvent);
				}
			}
		}

		MenuItem* mi = Items;
		int YDrawOffset = YPos;
		D3DCOLOR Color;

		RECT rc;
		rc.left = XPos + 20;
		rc.right = XPos + Width - 20;
		rc.top = YPos;
		rc.bottom = YPos + (13 * NumItems);

		D3DRECT rcCheck;
		rcCheck.x1 = XPos;
		rcCheck.y1 = rc.top;
		rcCheck.x2 = XPos + 11;
		rcCheck.y2 = rc.bottom;

		rc.right += 10;
		rc.bottom += 10;
		pSprite->Draw(0, XPos - 5, YPos - 5, Width + 10, GetCurrentHeight(), 0, 0, 0, 0, BackColor); //, &rc, 0, 0, 0xFFFFFFFF);
		rc.right -= 10;
		rc.bottom -= 10;

		for (int i = 0; i < NumHeaders; ++i)
		{
			if (!Headers[i].End)
			{
				pFont->DrawText(Headers[i].Name, -1, &rc, DT_CENTER, D3DCOLOR_ARGB(255, 255, 0, 0));//Changed (255, 0, 255, 0);
				YDrawOffset += 15;
				rc.top = YDrawOffset;
				rc.bottom = YDrawOffset + 13;
			}
		}

		D3DRECT rcTab = {XPos, YDrawOffset, XPos + Width / NumTabs - 1, YDrawOffset + 15};

		for (int i = 0; i < NumTabs; ++i)
		{
			pDevice->Clear(1, &rcTab, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, (i == CurrentTab ? D3DCOLOR_ARGB(255, 50, 154, 154)/*50, 50, 153*/ : D3DCOLOR_ARGB(255, 33, 33, 100)), 1.0f, 0);
			pFont->DrawText(Tabs[i].Name, -1, (RECT*) &rcTab, DT_CENTER, D3DCOLOR_ARGB(255, 255, 0, 0));
			rcTab.x1 += Width / NumTabs;
			rcTab.x2 += Width / NumTabs;
		}

		YDrawOffset += 17;

		for (int i = GetTabStart(CurrentTab); i <= GetTabEnd(CurrentTab); ++i, YDrawOffset += 13)
		{
			mi = &Items[i];
			rc.top = YDrawOffset;
			rc.bottom = YDrawOffset + 13;
			rcCheck.y1 = YDrawOffset + 1;
			rcCheck.y2 = rcCheck.y1 + 11;

			if (mi->Min == mi->Max)
			{
				if (mi->Min == 0)
					pFont->DrawText(mi->Name, -1, &rc, DT_CENTER, D3DCOLOR_ARGB(255, 0, 255, 0));
				else
					pFont->DrawText(mi->Name, -1, &rc, 0, D3DCOLOR_ARGB(255, 92, 92, 92));
			}
			else
			{
				if (Selected == i)
					Color = D3DCOLOR_ARGB(255, 255 - AnimFrame, 0, 0);
				else if (mi->Current != 0)
					Color = D3DCOLOR_ARGB(255, 0, 255, 255);//D3DCOLOR_ARGB(255, 0, 0, 255)
				else // if (mi->Current == 0)
					Color = D3DCOLOR_ARGB(180, 0, 255, 255);//0, 255

				pFont->DrawText(mi->Name, -1, &rc, 0, Color);

				if (mi->Max > 1 || mi->Flags & MenuFlagAlwaysDisplay)
					if (!((mi->Flags & MenuFlagDontDisplayZero) && mi->Current == 0))
						pFont->DrawText(mi->Display + (16 * (mi->Current - mi->Min)), -1, &rc, DT_RIGHT, Color);

				if (mi->Flags & MenuFlagSlider)
				{
					YDrawOffset += 13;
					rc.top = YDrawOffset;
					rc.bottom = YDrawOffset + 13;
					rcCheck.y1 = YDrawOffset + 1;
					rcCheck.y2 = rcCheck.y1 + 11;

					D3DRECT rcSlider[10];
					rcSlider[0].x1 = XPos;
					rcSlider[0].x2 = XPos + Width;
					rcSlider[0].y1 = rcCheck.y1 + (rcCheck.y2 - rcCheck.y1) / 2 - 1;
					rcSlider[0].y2 = rcSlider[0].y1 + 2;
					pDevice->Clear(1, rcSlider, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 33, 99, 99)/*33,33,100*/, 1.0f, 0);
					for (int i = 0; i <= mi->Max; ++i)
					{
						rcSlider[i].x1 = XPos + i * ((Width - 5) / mi->Max) + 1;
						rcSlider[i].x2 = rcSlider[i].x1 + 4;
						rcSlider[i].y1 = rcCheck.y1;
						rcSlider[i].y2 = rcCheck.y2;
					}
					pDevice->Clear(mi->Max + 1, rcSlider, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 33, 99, 99), 1.0f, 0);
					rcSlider[mi->Current].x1 -= 1;
					rcSlider[mi->Current].x2 += 1;
					pDevice->Clear(1, &rcSlider[mi->Current], D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 50, 154, 154)/*50, 50, 154*/, 1.0f, 0);
				}
				else
				{
					if (mi->Max == 1 || (mi->Max != 1 && mi->Current > 0))
					{
						pDevice->Clear(1, &rcCheck, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 33, 99, 99)/*33, 33, 100*/, 1.0f, 0);
						rcCheck.x1 += 2; rcCheck.y1 += 2; rcCheck.x2 -= 2; rcCheck.y2 -= 2;
						pDevice->Clear(1, &rcCheck, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 50, 154, 154), 1.0f, 0);
						rcCheck.x1 -= 2; rcCheck.y1 -= 2; rcCheck.x2 += 2; rcCheck.y2 += 2;
					}
					if (mi->Max != 1)
					{
						if (mi->Current != mi->Max)
						{
							rcCheck.x2 = XPos + Width;
							rcCheck.x1 = rcCheck.x2 - 11;
							pDevice->Clear(1, &rcCheck, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 33, 99, 99), 1.0f, 0);
							rcCheck.x1 += 2; rcCheck.y1 += 2; rcCheck.x2 -= 2; rcCheck.y2 -= 2;
							pDevice->Clear(1, &rcCheck, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 50, 154, 154), 1.0f, 0);
							rcCheck.x1 -= 2; rcCheck.y1 -= 2; rcCheck.x2 += 2; rcCheck.y2 += 2;
							rcCheck.x1 = XPos;
							rcCheck.x2 = rcCheck.x1 + 11;
						}
					}
					else
					{
						if (mi->Current == 1)
						{
							rcCheck.x1 += 2; rcCheck.y1 += 2; rcCheck.x2 -= 2; rcCheck.y2 -= 2;
							pDevice->Clear(1, &rcCheck, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(255, 172, 0, 0), 1.0f, 0);
							rcCheck.x1 -= 2; rcCheck.y1 -= 2; rcCheck.x2 += 2; rcCheck.y2 += 2;
						}
					}
				}
			}
		}

		for (int i = 0; i < NumHeaders; ++i)
		{
			if (Headers[i].End)
			{
				rc.top = YDrawOffset;
				rc.bottom = YDrawOffset + 13;
				pFont->DrawText(Headers[i].Name, -1, &rc, DT_CENTER, D3DCOLOR_ARGB(255, 0, 255, 0));
				YDrawOffset += 15;
			}
		}
		AnimFrame += AnimDelta;
		if (AnimFrame >= 150)
			AnimDelta = -3;
		else if (AnimFrame <= 0)
			AnimDelta = 3;
	}

	return true;
}