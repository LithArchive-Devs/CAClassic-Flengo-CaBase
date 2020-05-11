#include "DirectX.h"
#include "Menu.h"
#include "Files.h"

oPresent pPresent;
cDirectx Directx;
cDetours Detour;

#pragma optimize("", on)

LPDIRECT3DDEVICE9 g_pDevice = 0;
HRESULT GenerateTexture(IDirect3DDevice9 *pD3Ddev, IDirect3DTexture9 **ppD3Dtex, DWORD colour32)
{
	if( FAILED(pD3Ddev->CreateTexture(8, 8, 1, 0, D3DFMT_A4R4G4B4, D3DPOOL_MANAGED, ppD3Dtex,NULL)) )
		return E_FAIL;
	
	WORD colour16 =	((WORD)((colour32>>28)&0xF)<<12)
			|(WORD)(((colour32>>20)&0xF)<<8)
			|(WORD)(((colour32>>12)&0xF)<<4)
			|(WORD)(((colour32>>4)&0xF)<<0);

	D3DLOCKED_RECT d3dlr;    
	(*ppD3Dtex)->LockRect(0, &d3dlr, 0, 0);
	WORD *pDst16 = (WORD*)d3dlr.pBits;
	for(int xy=0; xy < 8*8; xy++)
		*pDst16++ = colour16;

	(*ppD3Dtex)->UnlockRect(0);

	return S_OK;
}

HRESULT WINAPI Present(LPDIRECT3DDEVICE9 pDevice, CONST RECT *pSourceRect, CONST RECT *pDestRect, HWND hDestWindowOverride, CONST RGNDATA *pDirtyRegion)
{
	//Draws our menu and renders our hacks
	__asm NOP;
	__asm PUSHAD;

	Base.InitGamePointers();
	Base.Render(pDevice);
	//Base.RenderHacks();
//	Base.hack_t();
	Sleep(25);

    __asm POPAD;
	return pPresent(pDevice, pSourceRect, pDestRect, hDestWindowOverride, pDirtyRegion);
}

DWORD * FindDevice(VOID)//Finds the Device on Injection
{
    DWORD Base = (DWORD)LoadLibraryW(L"d3d9.dll");

    for(DWORD i = 0; i < 0x128000; i++ )
    {
      if ( (*(BYTE *)(Base+i+0x00))==0xC7
        && (*(BYTE *)(Base+i+0x01))==0x06
        && (*(BYTE *)(Base+i+0x06))==0x89
        && (*(BYTE *)(Base+i+0x07))==0x86
        && (*(BYTE *)(Base+i+0x0C))==0x89
        && (*(BYTE *)(Base+i+0x0D))==0x86 )
        return (DWORD *)(Base + i + 2);
    }
    return NULL;
} 

void __stdcall HookzShit(void)//DA Hook homey :)
{
    DWORD * VtablePtr = FindDevice();
    DWORD* devicePtr = ***(DWORD****)VtablePtr;

    DWORD * VTable ;
    *(DWORD *)&VTable = *(DWORD *)VtablePtr;
	
	pPresent = ( oPresent )Detour.LegoMonster((LPVOID)VTable[17], (LPVOID)&Present, DETOUR_TYPE_STC_JC, 12);
}

bool IsGameReady()
{
	if( GetModuleHandle( "d3d9.dll" ) != NULL  )
	   return true;
	return false;
}

DWORD WINAPI dwAThread(LPVOID)
{
	while(!IsGameReady())
		Sleep(100);
		Base.hEvent = 0;//CreateEvent(0, FALSE, FALSE, 0);

	Base.Menu = new HackMenu;
	Base.pFont = new D3DFont;
	Base.pSprite = new D3DSprite;

	Base.InitMenu();
	HookzShit();
	return 0;
}

BOOL WINAPI DllMain ( HMODULE hDll, DWORD dwReason, LPVOID lpReserved )
{
	DisableThreadLibraryCalls(hDll);

	if ( dwReason == DLL_PROCESS_ATTACH )
	{
		CreateThread(NULL, NULL, dwAThread, NULL, NULL, NULL);
		//MessageBox(NULL, "Flengo Testing", "Flengo", MB_OK);
	}

	return TRUE;
}
