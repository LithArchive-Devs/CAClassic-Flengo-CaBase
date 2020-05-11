#ifndef _CDETOURS_H
#define _CDETOURS_H

#include "Files.h"

typedef enum detour_types_s
{
	DETOUR_TYPE_NOT_SET = -1,
	DETOUR_TYPE_OBS_RAND,
	DETOUR_TYPE_JMP,
	DETOUR_TYPE_PUSH_RET,
	DETOUR_TYPE_NOP_JMP,
	DETOUR_TYPE_NOP_NOP_JMP,
	DETOUR_TYPE_STC_JC,
	DETOUR_TYPE_CLC_JNC,
	DETOUR_TYPE_OBS_ADD,
	DETOUR_TYPE_PUSH_FUNC,
	DETOUR_TYPE_CALL_FUNC,
	DETOUR_TYPE_OBS_XOR,
	DETOUR_TYPE_OBS_STACKADD,
	DETOUR_TYPE_OBS_ROR,
	DETOUR_TYPE_OBS_ADDNOT,
	DETOUR_TYPE_FLENGO,

};
class cDetours
{
	public:
		LPVOID LegoMonster(LPVOID lpFuncOrig, LPVOID lpFuncDetour, int patchType, int detourLen);
		DWORD DetourCreateE9( DWORD  SrcVA, DWORD  DstVA, DWORD  Size );
		LPVOID DetourCreate ( BYTE *orig, const BYTE *det, int iPatchType);
		DWORD CreateJMP_EAX( DWORD  SrcVA, DWORD  DstVA, DWORD  Size );
		DWORD CreateJMP_REL( DWORD  SrcVA, DWORD  DstVA, DWORD  Size );
		void * CreateJMP(BYTE *org, CONST BYTE *det, INT len);
		LPVOID CreateJZ(LPVOID lpFuncOrig, LPVOID lpFuncDetour);
		void *memcpy_s( void* pvAddress, const void* pvBuffer, size_t stLen );
	//private:
		INT GetDetourLenAuto(PBYTE &pbFuncOrig,INT minDetLen);
};



#endif