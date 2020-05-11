#include "Base.h"
#include "Menu.h"
#include "DirectX.h"
#include "Files.h"

#include "Message.h"
#include "LTClient.h"
#include "Crasher.h"
#include "GameClientShell.h"
#include "HackFont.h"

#pragma optimize("", on)

#define MESSAGE_GUARANTEED (1<<7)

CClientInfoMgr* g_ClientInfoMgr;
CLTClient* g_LTClient;
CModelLT* g_ModelLT;
CCommonLT* g_CommonLT;
CGameClientShell* g_GameClientShell;

cBase Base;

bool ValidPointer(void* p)
{ 
	return !IsBadReadPtr(p, 4) && p;
}

void MemEdit(void* Dest, const void* Src, int Len)
{
	DWORD OldProtect;
	DWORD OldProtect2;
	VirtualProtect(Dest, Len, PAGE_EXECUTE_READWRITE, &OldProtect);
	memcpy(Dest, Src, Len);
	VirtualProtect(Dest, Len, OldProtect, &OldProtect2);
	FlushInstructionCache(GetCurrentProcess(), Dest, Len);
}

void cBase::InitGamePointers()
{
	g_LTClient = *(CLTClient**)ADDR_LTCLIENT;
	g_CommonLT = g_LTClient->Common();
	g_GameClientShell = (CGameClientShell*)ADDR_GCS;
	//g_PlayerMgr = g_GameClientShell->GetPlayerMgr();
	//g_ClientInfoMgr = g_GameClientShell->GetClientInfoMgr();
	//g_PlayerStats = g_PlayerMgr->GetPlayerStats();
}

void cBase::InitMenu()
{
	Menu->pFont = pFont;
	Menu->pSprite = pSprite;
	Menu->SetChangeEvent(hEvent);
	Menu->SetMenuPos(50, 100);
	Menu->SetWidth(230);

#ifdef VIP
	Menu->AddHeader("Flengo VIP"); // RDG CA NA VIP
#else
	Menu->AddHeader("Evolution - Flengo - Pimz");
#endif
	//Menu->AddTab("Aim bot");	// Alt+255
	//Menu->AddItem("Aim bone"          , 0, 6, 0, "Off\0            Body\0           Chest\0          Neck\0           Head\0           Nuts\0           Random", (HackProc) 0);
	//Menu->AddItem("Aim key"           , 0, 1, 0, "Middle Mouse\0   Alt", (HackProc) 0, true);
	//Menu->AddItem("Auto shoot"        , 0, 1, 0, "No\0             Yes", (HackProc) 0);
	//Menu->AddItem("Aim angle"         , 0, 7, 0, "10\0             30\0             45\0             70\0             90\0             120\0            180\0            360", (HackProc) 0);
	//Menu->AddItem("Aim through walls" , 0, 1, 1, "No\0             Yes", (HackProc) 0);
	//Menu->AddItem("Off after kill"    , 0, 1, 0, "No\0             Yes", (HackProc) 0);
	//Menu->AddItem("Aim speed"         , 0, 5, 0, "Instant\0        1/8 second\0     1/4 second\0     1/2 second\0     1 second\0       Random", (HackProc) 0);
	//Menu->AddItem("Aim recoil"        , 0, 2, 0, "None\0           All\0            Half", (HackProc) 0);
	//Menu->AddItem("Aim scan"          , 0, 1, 0, "Off\0            On", (HackProc) 0);
	//Menu->AddItem("Draw Aimspot"      , 0, 1, 0, "No\0             Yes", (HackProc) 0);
	//Menu->AddItem("Spawn check"       , 0, 1, 0, "No\0             Yes", (HackProc) 0);
	//Menu->AddTab("ESP");
#ifdef ACTUALCHAMS
#else
//	Menu->AddTab("Visuals");
//	Menu->AddItem("Chams"             , 0, 1, 0, "Off\0            NX", (HackProc)hack_t[0]);
#endif
	//Menu->AddItem("Crosshair"         , 0, 3, 0, "Hide\0           Show Red\0       Show Green\0     Show Blue", (HackProc) hack_t[13]);
	//Menu->AddItem("Boxes"             , 0, 2, 0, "Off\0            2D Enemy\0       2D All", (HackProc) ESP[0]);
	//Menu->AddItem("Nametags"          , 0, 1, 0, "None\0           Name", (HackProc) NULL);//NameTags
	//Menu->AddItem("Activation Distance"          , 0, 1, 0, "None\0           Name", (HackProc) hack_t[5]);//NameTags
	//Menu->AddItem("Wireframe"          , 0, 1, 0, "None\0           Name", (HackProc) NULL);
	/*Menu->AddItem("Fullbright"          , 0, 1, 0, "None\0           Name", (HackProc) hack_t[2]);
	Menu->AddItem("Show FPS"          , 0, 1, 0, "None\0           Name", (HackProc) hack_t[9]);
	Menu->AddItem("Removals"          , 0, 1, 0, "None\0           Name", (HackProc) hack_t[1]);*/
	//Menu->AddItem("Distance ESP"      , 0, 2, 0, "None\0           Enemy\0          All", (HackProc) ESP[1]);
	//Menu->AddItem("Score ESP"         , 0, 2, 0, "None\0           Enemy\0          All", (HackProc) ESP[2]);
	//Menu->AddItem("Health Bars"       , 0, 2, 0, "None\0           Enemy\0          All", (HackProc) ESP[3]);
	//Menu->AddItem("Pickup ESP"        , 0, 1, 0, "Off\0            On", (HackProc) ESP[4]);
	//Menu->AddItem("Nade ESP"          , 0, 1, 0, "Off\0            On", (HackProc) 0);
	//Menu->AddItem("Visiblity ESP"     , 0, 1, 0, "Off\0            On", (HackProc) 0);
	//Menu->AddItem("2D Radar"          , 0, 1, 0, "Off\0            On", (HackProc) 0);
	//Menu->AddItem("Game Info"         , 0, 1, 0, "Hide\0           Show", (HackProc) 0);
	//Menu->AddTab("Weapon");
	//Menu->AddItem("No Spread/Recoil"  , 0, 1, 0, "Off\0            On", (HackProc) hack_t[10]);
	////Menu->AddItem("Rapid Fire"    , 0, 1, 0, "Off\0            On", (HackProc) NULL);
	//Menu->AddItem("Gun Accuracy"      , 0, 1, 0, "Off\0            On", (HackProc) hack_t[7]);
	//Menu->AddItem("Weapon Range"      , 0, 1, 0, "Normal\0         Infinite", (HackProc) hack_t[12]);
	//Menu->AddItem("Superbullets"      , 0, 1, 0, "Off\0            Thru wall", (HackProc) hack_t[11]);
	//Menu->AddTab("Player");
	//Menu->AddItem("Speed"             , 0, 4, 0, "Off\0            Non-obvious\0    Fast\0           Faster\0         Fastest", (HackProc) hack_t[3]);
	////Menu->AddItem("Cam Position"      , 0, 2, 0, "Off\0            + Rise,- Lower\0 Locked", (HackProc) 0);
	//Menu->AddItem("Fly"          , 0, 1, 0, "None\0           Name", (HackProc) hack_t[6]);
	//Menu->AddItem("Jump"          , 0, 1, 0, "None\0           Name", (HackProc) hack_t[4]);
	////Menu->AddItem("OPK/Telekill"      , 0, 2, 0, "Off\0            Super OPK\0      OPK\0            Telekill", (HackProc) OPKProc);
	////Menu->AddItem("Ghost mode"        , 0, 1, 0, "Off\0            i j k l", (HackProc) GhostProc, true, true);
	////Menu->AddItem("Glitch key"        , 0, 1, 0, "Off\0            Alt+Arrow keys", (HackProc) 0, true, true);
	//Menu->AddItem("Infinite Stamina"  , 0, 1, 0, "Off\0            Infinite", (HackProc) 0);//Hmm this looks like
	Menu->AddTab("SendToServer");
	//Menu->AddItem("Glass walls"       , 0, 1, 0, "Off\0            On", (HackProc)0 );//Glass Wals Var
	//Menu->AddItem("Information"       , 0, 1, 0, "Off\0            On", (HackProc)DoInfo );
	//Menu->AddItem("Target teamates"   , 0, 1, 0, "No\0             Yes", (HackProc) 0);
#ifndef VIP
	//Menu->AddItem("LifeTaker QR"    , 0, 1, 0, "Off\0          On (QR)\0", (HackProc) LifeTaker);
	//Menu->AddItem("Crash-To-Desktop"    , 0, 1, 0, "Off\0            On", (HackProc) CrashToDesktop);
	Menu->AddItem("Server Crasher"    , 0, 1, 0, "Off\0            On", (HackProc) DoServerCrash);
	//Menu->AddItem("Hostil-Kick"    , 0, 1, 0, "Off\0            On", (HackProc) KickHostil);
	Menu->AddItem("Instant-Respawn"    , 0, 1, 0, "Off\0            On\0", (HackProc) InstantRespawn);
	Menu->AddItem("Weapon-Spawn"    , 0, 4, 0, "Off\0	Napalm\0	Meteor\0	First-Aid\0	Auto-Turret\0", (HackProc) WeaponSpawn);
	Menu->AddItem("God-Mode"    , 0, 1, 0, "Off\0            On", (HackProc) GodMod);
	//Menu->AddItem("Voice-Spam"    , 0, 1, 0, "Off\0            On", (HackProc) VoiceSpam);
	//Menu->AddItem("Chat-Spammer"          , 0, 1, 0, "None\0           Name", (HackProc) Spammer);
	Menu->AddItem("Suicide"          , 0, 1, 0, "None\0           Name", (HackProc) Suicide);
	Menu->AddItem("Unknown"          , 0, 1, 0, "None\0           Name", (HackProc) Unknown);
	//Menu->AddTab("Others");
	//Menu->AddItem("No Recoil"          , 0, 1, 0, "Off\0            On", (HackProc) hack_t[0]);
	//Menu->AddItem("Weapon-Range"          , 0, 1, 0, "Off\0            On", (HackProc) hack_t[2]);
	//Menu->AddItem("SuperBullets"          , 0, 1, 0, "Off\0            On", (HackProc) hack_t[1]);
	////Menu->AddItem("Removers"          , 0, 1, 0, "Off\0            On", (HackProc) hack_t[4]);
	//Menu->AddItem("NX Chams"          , 0, 1, 0, "Off\0            On", (HackProc) hack_t[3]);
	//Menu->AddItem("Fly"          , 0, 1, 0, "Off\0            Spacebar\0", (HackProc) hack_t[5]);
	//Menu->AddItem("KDR lock"          , 0, 1, 0, "Off\0            1.5", (HackProc) 0, true, true);
#endif

	//Menu->AddItem("Save Settings"     , 0, 1, 0, "\0               ", (HackProc) SaveProc);
#ifdef VIP
	Menu->AddHeader("<Something>");
#else
	Menu->AddHeader("Version : 1.0");
#endif
}

cBase::cBase() : InGame(false), IsInitted(false)
{
	OSVERSIONINFO os;
	os.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
	GetVersionEx(&os);
	if (os.dwMajorVersion == 6)
	{
		if (os.dwMinorVersion == 0)
			OSVer = Vista;
		else
			OSVer = Win7;
	}
	else if (os.dwMajorVersion == 5 && os.dwMinorVersion == 1)
		OSVer = XP;
}

void cBase::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if (pDevice == 0)
		return;

	if (!IsInitted)
	{
		IsInitted = true;
		pDevice = pDevice;
		Menu->pDevice = pDevice;
		Menu->ShowMenu(false);
		Menu->BackColor = D3DCOLOR_ARGB(220, 0, 0, 0);
		if (OSVer != XP)
			pFont->Create(pDevice, "Segoe UI", 11, 0);
		else
			pFont->Create(pDevice, "Verdana", 11, 0);
		pSprite->Create(pDevice);
	}

	if (pDevice != pDevice)
	{
		pDevice = pDevice;
		Menu->pDevice = pDevice;
		try
		{	
			if (pFont != 0 && !IsBadReadPtr(pFont, sizeof(*pFont)))
				pFont->DestroyDeviceObjects();
			if (pSprite != 0 && !IsBadReadPtr(pSprite, sizeof(*pSprite)))
				pSprite->Destroy();
		} catch (...) {}
		pSprite->Create(pDevice);
		pFont->Recreate(pDevice);
	}

	if (InGame != g_LTClient->IsConnected())
	{
		try
		{
			if (pFont != 0 && !IsBadReadPtr(pFont, sizeof(*pFont)))
				pFont->DestroyDeviceObjects();
			if (pSprite != 0 && !IsBadReadPtr(pSprite, sizeof(*pSprite)))
				pSprite->Destroy();
		} catch (...) {}
		pSprite->Create(pDevice);
		pFont->Recreate(pDevice);
		Kills = 0;
		Deaths = 0;
		Headshots = 0;
		//TargetObj = 0;
		InGame = g_LTClient->IsConnected();
	}

	if (InGame)
	{
		if (Menu->Items[MENU_CROSS].Current > 0)
		{
			D3DVIEWPORT9 Viewport;
			pDevice->GetViewport(&Viewport);

			D3DRECT rcLines[2];
			rcLines[0].x1 = (Viewport.Width / 2) - 25 - 1;
			rcLines[0].y1 = (Viewport.Height / 2) - 1;
			rcLines[0].x2 = (Viewport.Width / 2) + 25 - 1;
			rcLines[0].y2 = (Viewport.Height / 2);
			rcLines[1].x1 = (Viewport.Width / 2) - 1;
			rcLines[1].y1 = (Viewport.Height / 2) - 25 - 1;
			rcLines[1].x2 = (Viewport.Width / 2);
			rcLines[1].y2 = (Viewport.Height / 2) + 25 - 1;
			if (Menu->Items[MENU_CROSS].Current == 1)
				pDevice->Clear(2, rcLines, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 255, 0, 0), 0.0f, 0);
			else if (Menu->Items[MENU_CROSS].Current == 2)
				pDevice->Clear(2, rcLines, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 255, 0), 0.0f, 0);
			else
				pDevice->Clear(2, rcLines, D3DCLEAR_TARGET, D3DCOLOR_ARGB(255, 0, 0, 255), 0.0f, 0);
		}
	}

	if (GetAsyncKeyState(VK_INSERT) & 1)
	{
		Menu->ShowMenu(!Menu->IsShown());
		pFont->DestroyDeviceObjects();
		pSprite->Destroy();
		pFont->Recreate(pDevice);
		pSprite->Create(pDevice);
	}

	if (Menu->IsShown())
	{
		//if (SUCCEEDED(Menu->pDevice->BeginScene()))
		{
			Menu->Render();

		//	Menu->pDevice->EndScene();
		}
	}
}

void cBase::CrashToDesktop(int Status, LPDIRECT3DDEVICE9 pDevice)//lucafox for help on testing :)
{
	if(Status > 0)
	{
		if(g_LTClient->IsConnected())
		{
			Sleep(200);

			if(ValidPointer(g_LTClient))//209 Leave
			{
				CAutoMessage Msg;
				Msg.Writeuint8(108);//104
				Msg.WriteWString((wchar_t *) "MASIVE STRING FOR CRASH TO DESKTOP FROM WWW.Pimz.NET YAY MASIVE STRING !@#@#*& ASDJ *()(*& !(!*@U(_ &*)(*& !21#98& AAAAAAAAAAAAAAAAAAA!AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA@#@#*& ASDJ *()(*& AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA@#@#*& ASDJ *()(*& AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA@#@#*& ASDJ *()(*& AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA@#@#*& ASDJ *()(*&  MASSIVE STRING END");
				Msg.Writeuint8(0);
				g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
				Sleep(150);
				Msg.Reset();
				Msg.Writeuint8(209);
				Msg.Writeuint8(0);
				g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
				Status = 0;
			}
		}
	}
}

void cBase::VoiceSpam(int Status, LPDIRECT3DDEVICE9 pDevice)
{
	if(Status > 0)
	{
		if(g_LTClient->IsConnected())
		{
			if(ValidPointer(g_LTClient))
			{
				CAutoMessage Msg;
				Msg.Writeuint8(190);
				Msg.Writeuint8(1); //1 parameter
				Msg.Writeuint8(1); //2 parameter
				Msg.Writeuint16(3); //3 push voice (mean gogogo etc etc)
				g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
				g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
				keybd_event(0x33,0,0,0);
			}
		}
	}
}

void cBase::LeaveGame(int Status, LPDIRECT3DDEVICE9 pDevice)
{
	if(Status > 0)
	{
		if(g_LTClient->IsConnected())
		{
			CAutoMessage Msg;
			Msg.Writeuint8(209);
			g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
			Status = 0;
		}
	}
}

void cBase::Suicide(int Status, LPDIRECT3DDEVICE9 pDevice)
{
	if(Status > 0)
	{
		if(g_LTClient->IsConnected())
		{
			CAutoMessage Msg;
			Msg.Writeuint8(161);
			g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
			Status = 0;
		}
	}
}

void cBase::Unknown(int Status, LPDIRECT3DDEVICE9 pDevice)
{
	if(Status > 0)
	{
		if(g_LTClient->IsConnected())
		{
			CAutoMessage Msg;
			Msg.Writeuint16(999);
			g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
			Status = 0;
		}
	}
}

void cBase::GodMod(int Status, LPDIRECT3DDEVICE9 pDevice)
{
	if(Status > 0)
	{
		if(g_LTClient->IsConnected())
		{
			if(ValidPointer(g_LTClient))
			{
				CAutoMessage Msg;        
				Msg.Writeint16(135);
				Msg.Writeuint8(0);
				g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
				keybd_event(0x33,0,0,0);
			}
		}
	}
}

void cBase::WeaponSpawn(int WEPID, LPDIRECT3DDEVICE9 pDevice)
{
	if(g_LTClient->IsConnected())
	{
		Sleep(200);
		if(ValidPointer(g_LTClient))
		{
			if(GetAsyncKeyState(VK_NUMPAD1) &1)
			{
				CAutoMessage Msg;
				Msg.Writeuint8(50);
				Msg.Writeuint8(1);	
				Msg.Writeuint8(1);
				Msg.Writeuint8(1);
				Msg.Writeuint8(1);	
				Msg.Writeuint8(1);
				if(WEPID == 1)
				{
					Msg.Writeuint16(293);//Napalm
				} else if(WEPID == 2)
				{
					Msg.Writeuint16(265);//Nuke Thing Meteor
				} else if(WEPID == 3)
				{
					Msg.Writeuint16(280);//First Aid
				} else if(WEPID == 4)
				{
					Msg.Writeuint16(330);//Turret
				} else if(WEPID == 5)
				{
					Msg.Writeuint16(425);
				}
					Msg.Writeuint8(1);
				Msg.Writeuint8(1);
				g_LTClient->SendToServer (Msg.Read (), MESSAGE_GUARANTEED);
				keybd_event(0x33, 0,0,0);
			}
		}
	}
}
	

void cBase::EliteVoteKick(LPDIRECT3DDEVICE9 pDevice)
{
	if(g_LTClient->IsConnected())
	{
         if(ValidPointer(g_LTClient))
		 {
		    CAutoMessage Msg;
            Msg.Writeuint8(206);
            Msg.Writeuint8(1);
			g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
		 }
	}
}

void cBase::InstantRespawn(LPDIRECT3DDEVICE9 pDevice)
{
	if(g_LTClient->IsConnected())
	{
        if(ValidPointer(g_LTClient))
        {
			CAutoMessage Msg;
			Msg.Writeuint8(113);
			g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
			keybd_event(0x33,0,0,0);
		}
	}
}

void cBase::KickHostil(LPDIRECT3DDEVICE9 pDevice)
{
	if(ValidPointer(g_LTClient))
    {
        CAutoMessage Msg;
        Msg.Writeuint8(101);
        Msg.Writeuint8(1);
        Msg.Writeuint8(1);
        Msg.Writeuint8(1);
        Msg.Writeuint8(100);
        Msg.Writeuint8(1);
        Msg.Writeuint8(1);
        g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
        keybd_event(0x33,0,0,0);
	}
}

void cBase::DoServerCrash(int Status, LPDIRECT3DDEVICE9 pDevice)
{
	if(Status > 0)
	{
		if(g_LTClient->IsConnected())
		{
			if(ValidPointer(g_LTClient))
			{
				CAutoMessage Msg;
				Msg.Writeuint8(153);
				Msg.Writeuint8(1);
				Msg.Writeuint8(1);
				Msg.Writeuint8(1);
				Msg.Writeuint8(200);
				Msg.Writeuint8(1);
				Msg.Writeuint8(1);
				g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
				Status = 0;
			}
		}
	}
}

void cBase::LifeTaker(int Status, LPDIRECT3DDEVICE9 pDevice)
{
    if(Status)
    {
        if(ValidPointer(g_LTClient))
        {
            CAutoMessage Msg;
			Msg.Writeuint8(50);
			Msg.Writeuint8(1);
			Msg.Writeuint8(1);
			Msg.Writeuint8(1);
			Msg.Writeuint8(1);
			Msg.Writeuint8(1);
			Msg.Writeuint8(110);
			Msg.Writeuint8(1);
			Msg.Writeuint8(1);
            g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
            keybd_event(0x33,0,0,0);
        }
        else
        {
            g_LTClient = *(CLTClient**)ADDR_LTCLIENT;
            g_CommonLT = g_LTClient->Common();
            return;
        }
    }
} 
void cBase::Spammer(int Status, LPDIRECT3DDEVICE9 pDevice)
{
	if(Status > 0)
	{
		if(g_LTClient->IsConnected())
		{
			if(ValidPointer(g_LTClient))
			{
				CAutoMessage Msg;
				Msg.Writeuint8(104);
				Msg.WriteWString((wchar_t*)L"Get Your Hacks For Free at www.Pimz.net! - Flengo" );
				Msg.WriteWString((wchar_t*)L"GM");
				Msg.Writeint32(0);
				g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
				Sleep(175);
				Msg.Writeuint8(104);
				Msg.WriteWString((wchar_t*)L"Get Your Hacks For Free at www.Pimz.net! - Flengo"  );
				Msg.WriteWString((wchar_t*)L"GM" );
				Msg.Writeint32(0);
				g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);			Msg.Writeuint8(104);
				Sleep(175);
				Msg.WriteWString((wchar_t*)L"Get Your Hacks For Free at www.Pimz.net! - Flengo"  );
				Msg.WriteWString((wchar_t*)L"GM" );
				Msg.Writeint32(0);
				g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
				Sleep(175);
				Msg.Writeuint8(104);
				Msg.WriteWString((wchar_t*)L"Get Your Hacks For Free at www.Pimz.net! - Flengo"  );
				Msg.WriteWString((wchar_t*)L"GM" );
				Msg.Writeint32(0);
				g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
				Sleep(175);
				Msg.Writeuint8(104);
				Msg.WriteWString((wchar_t*)L"Get Your Hacks For Free at www.Pimz.net! - Flengo"  );
				Msg.WriteWString((wchar_t*)L"GM" );
				Msg.Writeint32(0);
				g_LTClient->SendToServer(Msg.Read(), MESSAGE_GUARANTEED);
				Status = 0;
			}
		}
	}
}