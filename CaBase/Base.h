#ifndef __BASE_H__
#define __BASE_H__

#include "Files.h"
#include "SafePtr.h"
#include "Menu.h"
using namespace std;
#define MENU_END 38
#define Red D3DCOLOR_ARGB (255 , 255 , 0 , 0 )    
#define Yellow D3DCOLOR_ARGB (255 , 255 , 255 , 0 ) 
#define Green D3DCOLOR_ARGB (255 , 0 , 255 , 0 )    
#define Blue D3DCOLOR_ARGB (255 , 0 , 0 , 255 )     
#define Purple D3DCOLOR_ARGB (255 , 102 , 0 , 153 )   
#define Pink D3DCOLOR_ARGB (255 , 255 , 20 , 147 ) 
#define Orange D3DCOLOR_ARGB (255 , 255 , 165 , 0 )  
#define Black D3DCOLOR_ARGB (255 , 0 , 0 , 0 )       
#define White D3DCOLOR_ARGB (255 , 255 , 255 , 255 )
#define Cyan D3DCOLOR_ARGB(255, 0, 255, 255)

#define M_PI 3.14159265358979323846f
#define DegToRad( degree ) ( (degree) * (3.141592654f / 180.0f) )
#define PITCH 0
#define YAW 1
#define ROLL 2

#define MENU_CROSS 28

//#define FullAuto 		0x374220D7
//#define S2S 		0x372F2D35
//#define FireMessage1 		0x37426AB3
//#define FireMessage2 		0x37426B02
//#define Recoil1 		0x373AB5D0
//#define Recoil2 		0x374D4629
//#define Recoil3 		0x373AB5E4
//#define Recoil4 		0x373AB5E7
//#define Recoil5 		0x373AB5F0
#define PlayerByIndex 		0x371550A0
#define LocalPlayer 		0x372C5060
#define ADDR_MTBWCRET		0x373B0257
#define ADDR_LTCLIENT		0x37808570
#define ADDR_WEAPONMGR		0x37856D34
#define ADDR_LOCALPTR		0x3786734C
#define ADDR_BUILDFONT		0x375E1D00
#define ADDR_FILLFONT		0x375E2990
#define ADDR_FONTECX		0x37851824
#define ADDR_GETFONTHAND	0x373B7520
#define ADDR_GETCONSOLEINT	0x373CB5D0
#define ADDR_GCS			0x37850018
#define ADDR_LTCOMMON		0x37856B58
#define ADDR_S_BULLETS		0x375003F6 
#define ClientInfoMgr 		0x378086B4
//#define PlayerMgr 		0x377FC27B
#define ADDR_REMOTEKILL1	0x37505E51
#define ADDR_REMOTEKILL2	0x37505E52
#define ADDR_REMOTEKILL3	0x375061D1
#define ADDR_REMOTEKILL4	0x375061D2
#define ADDR_WEAPON_RANGE1	0x37505D29
#define ADDR_WEAPON_RANGE2	0x375060A9
#define ADDR_RAPIDFIRE1		0x374FCC87
#define ADDR_RAPIDFIRE2		0x3750681E
#define ADDR_RAPIDFIRE3		0x37502367
#define ADDR_RAPIDFIRE4		0x37506825
#define ADDR_RAPIDFIRE5		0x3750660D
#define ADDR_FORCEFPS		0x377E0D18
#define ADDR_LTCSBASE		0x377E0C70
#define ADDR_GETPLAYERBYINDEX 0x372C4670 //CShell.dll[0x37100000] + 0x001C4670
#define ADDR_NAMETAGS1 0x37255AFF //CShell.dll[0x37100000] + 0x00155AFF
#define ADDR_NAMETAGS2 0x37255A79 //CShell.dll[0x37100000] + 0x00155A79
#define ADDR_GLASSWALLS 0x0057D32A //Engine.exe[0x00460000] + 0x0011D32A
#define ADDR_GAMESTATUS 0x37821CEC

#define OFF_FPS			0x14EF8

#define ADDR_W2S		0x00496C50
#define ADDR_INTERSECT		0x00470FA0
#define ADDR_D3D		0x009269C8
#define ADDR_DRAWPRIM		0x0081AA38
#define ADDR_CURSORCENTER	0x00804D24
#define ADDR_CONSOLEUNWRAPPED	0x0048B5B0

#define ADDR_CLIENTFX		0x0F2B6F34

class _PositionsPlayers;
class PlayerInfo;

class _PositionPlayers
{
public:
	char unknown0[4]; //0x00
	D3DXVECTOR3* PlayerPositions; //0x004
};

class PlayerInfo
{
public:

		char unknown0[4]; //0x0000
	__int32 index; //0x0004  
		char unknown8[8]; //0x0008
	char Name[20]; //0x0010  
		char unknown36[4]; //0x0024
	D3DXVECTOR3* PlayerPositions;
		//_PositionsPlayers* pPlayerPositions; //0x0028 
		char unknown44[68]; //0x002C
	__int32 Team; //0x0070  
		char unknown116[4]; //0x0074
	__int8 IsDead; //0x0078  
		char unknown124[504]; //0x007C
	PlayerInfo* Prev;
	PlayerInfo* Next;
		char unknown644[2504]; //0x0284
};

class cBase{
	public:
		void MemoryHacks();
		void ConsoleHacks();
		LPD3DXLINE	g_pLine;
		void InitMenu();
		void Render(LPDIRECT3DDEVICE9 pDevice);
		static void SaveProc(int Value);
		void InitGamePointers();
		void __stdcall PushCommands(const char* cmd);
		//bool WorldToScreen(LPDIRECT3DDEVICE9 pDevice, const D3DXVECTOR3& WorldPos, D3DXVECTOR3* pScreenPos);
		//void AddESP(LPDIRECT3DDEVICE9 pDevice);
		static void RenderHacks();
		static void DoServerCrash(int Status, LPDIRECT3DDEVICE9 pDevice);
		static void LifeTaker(int Status, LPDIRECT3DDEVICE9 pDevice);
		static void KickHostil(LPDIRECT3DDEVICE9 pDevice);
		static void ZumbiKill(LPDIRECT3DDEVICE9 pDevice);
		static void EliteVoteKick(LPDIRECT3DDEVICE9 pDevice);/**/
		static void InstantRespawn(LPDIRECT3DDEVICE9 pDevice);
		static void WeaponSpawn(int WEPID, LPDIRECT3DDEVICE9 pDevice);
		static void GodMod(int Status, LPDIRECT3DDEVICE9 pDevice);
		static void VoiceSpam(int Status, LPDIRECT3DDEVICE9 pDevice);
		static void Spammer(int Status, LPDIRECT3DDEVICE9 pDevice);
		static void CrashToDesktop(int Status, LPDIRECT3DDEVICE9 pDevice);
		static void LeaveGame(int Status, LPDIRECT3DDEVICE9 pDevice);
		static void Unknown(int Status, LPDIRECT3DDEVICE9 pDevice);
		static void Suicide(int Status, LPDIRECT3DDEVICE9 pDevice);

		bool InGame;
		cBase();
		bool IsInitted;
		enum _OSVer
		{
			Vista = 0,
			Win7 = 1,
			XP = 2,
		};
		_OSVer OSVer;
		int Kills;
		int Deaths;
		int Headshots;

		SafePtr<D3DFont> pFont;
		SafePtr<D3DSprite> pSprite;
		SafePtr<HackMenu> Menu;
		HANDLE hEvent;
};

extern class cBase Base;

#endif