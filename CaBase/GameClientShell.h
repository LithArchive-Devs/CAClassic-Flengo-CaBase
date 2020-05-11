#pragma once

#include <Windows.h>
#include "Files.h"

class CClientInfoMgr;
class CInterfaceMgr;
class CPlayerMgr;
class CSFXMgr;

class CGameClientShell
{
public:
	virtual void function0();				//0x0000
	virtual void function1();				//0x0004
	virtual void function2();				//0x0008
	virtual void function3();				//0x000C
	virtual void function4();				//0x0010
	virtual void function5();				//0x0014
	virtual void function6();				//0x0018
	virtual void function7();				//0x001C
	virtual void function9();				//0x0020
	virtual void function10();				//0x0024
	virtual void function11();				//0x0028
	virtual void function12();				//0x002C
	virtual void function13();				//0x0030
	virtual void function14();				//0x0034
	virtual void function15();				//0x0038
	virtual void function16();				//0x003C
	virtual void function17();				//0x0040
	virtual void function18();				//0x0044
	virtual void function19();				//0x0048
	virtual void function20();				//0x004C
	virtual void function21();				//0x0050
	virtual void function22();				//0x0054
	virtual void function23();				//0x0058
	virtual void function24();				//0x005C
	virtual void function25();				//0x0060
	virtual void function26();				//0x0064
	virtual void function27();				//0x0068
	virtual void function28();				//0x006C
	virtual void function29();				//0x0070
	virtual void function30();				//0x0074
	virtual void function31();				//0x0078
	virtual void function32();				//0x007C
	virtual void function33();				//0x0080
	virtual void function34();				//0x0084
	virtual void function35();				//0x0088
	virtual void function36();				//0x008C
	virtual void function37();				//0x0090
	virtual void function38();				//0x0094
	virtual void function39();				//0x0098
	virtual void function40();				//0x009C
	virtual void function41();				//0x00A0
	virtual void function42();				//0x00A4
	virtual void function43();				//0x00A8
	virtual void function44();				//0x00AC
	virtual CClientInfoMgr* GetClientInfoMgr();	//0x00B0
	virtual CInterfaceMgr* GetInterfaceMgr();	//0x00B4
	virtual CPlayerMgr* GetPlayerMgr();		//0x00B8
	virtual void function48();				//0x00BC
	virtual void function49();				//0x00C0
	virtual void function50();				//0x00C4
	virtual void function51();				//0x00C8
	virtual void function52();				//0x00CC
	virtual void function53();				//0x00D0
	virtual void function54();				//0x00D4
	virtual void function55();				//0x00D8
	virtual CSFXMgr* GetSFXMgr();			//0x00DC
	virtual void function57();				//0x00E0
	virtual void function58();				//0x00E4
	virtual void function59();				//0x00E8
	virtual void function60();				//0x00EC
};