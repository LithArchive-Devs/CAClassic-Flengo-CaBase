#include "ADE32.h"
#include "CDetour.h"

INT GetDetourLenAuto(PBYTE &pbFuncOrig,INT minDetLen);
#define JUNK_CODE             \
    __asm{push eax}            \
    __asm{xor eax, eax}        \
    __asm{setpo al}            \
    __asm{push edx}            \
    __asm{xor edx, eax}        \
    __asm{sal edx, 2}          \
    __asm{xchg eax, edx}       \
    __asm{pop edx}             \
    __asm{or eax, ecx}         \
    __asm{pop eax}
#define DETOUR_MAX_SRCH_OPLEN 64

#define JMP32_SZ 5
#define BIT32_SZ 4

#define SIG_SZ 3
#define SIG_OP_0 0xCC
#define SIG_OP_1 0x90
#define SIG_OP_2 0xC3
#define DETOUR_LEN_AUTO 0 // Finds the detour length automatically

static DWORD dwOldProt;

int GetDetourLen(int patchType)
{
	switch(patchType)
	{
		case DETOUR_TYPE_JMP:
			return 5;

		case DETOUR_TYPE_PUSH_RET:
		case DETOUR_TYPE_NOP_JMP:
			return 6;
		
		case DETOUR_TYPE_NOP_NOP_JMP:
		case DETOUR_TYPE_STC_JC:
		case DETOUR_TYPE_CLC_JNC:
			return 7;
		
		default:
			return 0;
	}
}

LPVOID cDetours::LegoMonster(LPVOID lpFuncOrig, LPVOID lpFuncDetour, int patchType, int detourLen)
{
	LPVOID lpMallocPtr = NULL;
	JUNK_CODE
	DWORD dwProt = NULL;
	JUNK_CODE
	PBYTE pbMallocPtr = NULL;
	JUNK_CODE
	PBYTE pbFuncOrig = (PBYTE)lpFuncOrig;
	JUNK_CODE
	PBYTE pbFuncDetour = (PBYTE)lpFuncDetour;
	JUNK_CODE
	PBYTE pbPatchBuf = NULL;
	JUNK_CODE
	int minDetLen = 0;
	JUNK_CODE
	int detLen = 12;
	JUNK_CODE

	if((minDetLen = GetDetourLen(patchType)) == 0)
		return NULL;

	if(detourLen != DETOUR_LEN_AUTO)
		detLen = detourLen;

	else if((detLen = GetDetourLenAuto(pbFuncOrig, minDetLen)) < minDetLen)
		return NULL;

	if((lpMallocPtr = (LPVOID)malloc(detLen+JMP32_SZ+SIG_SZ)) == NULL)
		return NULL;

	pbMallocPtr = (PBYTE)lpMallocPtr;
	JUNK_CODE

	VirtualProtect(lpFuncOrig, detLen, PAGE_READWRITE, &dwProt);
	JUNK_CODE

	memcpy(lpMallocPtr, lpFuncOrig, detLen);
	JUNK_CODE
	pbMallocPtr += detLen;
	JUNK_CODE
	pbMallocPtr[0] = 0xE9;
	JUNK_CODE
	*(DWORD*)(pbMallocPtr+1) = (DWORD)((pbFuncOrig+detLen)-pbMallocPtr)-JMP32_SZ;
	JUNK_CODE
	pbMallocPtr += JMP32_SZ;
	JUNK_CODE
	pbMallocPtr[0] = SIG_OP_0;
	JUNK_CODE
	pbMallocPtr[1] = SIG_OP_1;
	JUNK_CODE
	pbMallocPtr[2] = SIG_OP_2;
	JUNK_CODE

	pbPatchBuf = new BYTE[detLen];
	JUNK_CODE
	memset(pbPatchBuf, 0x90, detLen);
	JUNK_CODE

	switch(patchType)
	{
		case DETOUR_TYPE_JMP:
			JUNK_CODE
			pbPatchBuf[0] = 0xE9;
			JUNK_CODE
			*(DWORD*)&pbPatchBuf[1] = (DWORD)(pbFuncDetour - pbFuncOrig) - 5;
			JUNK_CODE
			break;

		case DETOUR_TYPE_PUSH_RET:
			JUNK_CODE
			pbPatchBuf[0] = 0x68;
			JUNK_CODE
			*(DWORD*)&pbPatchBuf[1] = (DWORD)pbFuncDetour;
			JUNK_CODE
			pbPatchBuf[5] = 0xC3;
			break;

		case DETOUR_TYPE_NOP_JMP:
			JUNK_CODE
			pbPatchBuf[0] = 0x90;
			JUNK_CODE
			pbPatchBuf[1] = 0xE9;
			JUNK_CODE
			*(DWORD*)&pbPatchBuf[2] = (DWORD)(pbFuncDetour - pbFuncOrig) - 6;
			JUNK_CODE
			break;


		case DETOUR_TYPE_FLENGO:
			JUNK_CODE
			pbPatchBuf[0] = 0xCC;
			JUNK_CODE
			pbPatchBuf[1] = 0xCC;
			JUNK_CODE
			pbPatchBuf[2] = 0xE9;
			JUNK_CODE
			*(DWORD*)&pbPatchBuf[3] = (DWORD)(pbFuncDetour - pbFuncOrig) - 7;
			JUNK_CODE
			break;

		case DETOUR_TYPE_NOP_NOP_JMP:
			JUNK_CODE
			pbPatchBuf[0] = 0x90;
			JUNK_CODE
			pbPatchBuf[1] = 0xF9;
			JUNK_CODE
			pbPatchBuf[2] = 0xE9;
			JUNK_CODE
			*(DWORD*)&pbPatchBuf[3] = (DWORD)(pbFuncDetour - pbFuncOrig) - 7;
			JUNK_CODE
			break;


		case DETOUR_TYPE_STC_JC:
			JUNK_CODE
			pbPatchBuf[0] = 0xF9;
			JUNK_CODE
			pbPatchBuf[1] = 0x0F;
			JUNK_CODE
			pbPatchBuf[2] = 0x82;
			JUNK_CODE
			*(DWORD*)&pbPatchBuf[3] = (DWORD)(pbFuncDetour - pbFuncOrig) - 7;
			JUNK_CODE
			break;

		case DETOUR_TYPE_CLC_JNC:
			JUNK_CODE
			pbPatchBuf[0] = 0x90;
			JUNK_CODE
			pbPatchBuf[0] = 0xF8;
			JUNK_CODE
			pbPatchBuf[1] = 0x0F;
			JUNK_CODE
			pbPatchBuf[2] = 0x83;
			JUNK_CODE
			*(DWORD*)&pbPatchBuf[4] = (DWORD)(pbFuncDetour - pbFuncOrig) - 7;
			JUNK_CODE
			break;
		
		default:
			return NULL;
	}

	for(int i=0; i<detLen; i++)
		pbFuncOrig[i] = pbPatchBuf[i];

	delete [] pbPatchBuf;
	JUNK_CODE

	VirtualProtect(lpFuncOrig, detLen, dwProt, &dwOldProt);
	JUNK_CODE

	return lpMallocPtr;
}

LPVOID cDetours::DetourCreate ( BYTE *orig, const BYTE *det, int iPatchType)
{
        BYTE    *jmp = NULL;
		int len = 0; 
        int             iMinLen = 0;

        if ( !(iMinLen = GetDetourLen(iPatchType)) )
                return 0;

        if ( len != 0 && len < iMinLen )
                return 0;

        // Try and find the end of the instruction automatically
        if ( len == 0 )
        {
                len = GetDetourLenAuto( orig, iMinLen );

                if ( len < iMinLen )
                        return 0;
        }

      //  if ( !LegoMonster(jmp, orig, det, iPatchType, len) )
                return 0;

        return jmp - len;
}


INT cDetours::GetDetourLenAuto(PBYTE &pbFuncOrig,INT minDetLen)
{
    INT len = 0;
    PBYTE pbCurOp = pbFuncOrig;

    while(len < minDetLen)
    {
        int i = oplen(pbCurOp);   
        
        if(i == 0 || i == -1)
            return 0;

        if(len > 64)
            return 0;

        len += i;
        pbCurOp += i;
    }
    return len;
}

LPVOID cDetours::CreateJZ(LPVOID lpFuncOrig, LPVOID lpFuncDetour)
{
    LPVOID lpMallocPtr = NULL;
    PBYTE pbMallocPtr = NULL;
    PBYTE pbFuncOrig = (PBYTE)lpFuncOrig;
    PBYTE pbFuncDetour = (PBYTE)lpFuncDetour;
    PBYTE pbPatchBuf = NULL;
    INT detLen = GetDetourLenAuto(pbFuncOrig,10);
    if((lpMallocPtr = (LPVOID)GlobalAlloc(GPTR,detLen + 5 + 3 )) == NULL)
        return NULL;
    pbMallocPtr = (PBYTE)lpMallocPtr;
    DWORD lpfnOldProtect = NULL,lpNewProtect = PAGE_READWRITE;
    VirtualProtect(lpFuncOrig, detLen,lpNewProtect,&lpfnOldProtect);
    memcpy(lpMallocPtr, lpFuncOrig, detLen);

    pbMallocPtr += detLen;
    pbMallocPtr[0] = 0xE9;
    *(DWORD*)(pbMallocPtr+1) = (DWORD)((pbFuncOrig+detLen)-pbMallocPtr) - 5;
    pbMallocPtr += 0x05;
    pbMallocPtr[0] = 0xCC;
    pbMallocPtr[1] = 0x90;
    pbMallocPtr[2] = 0xC3;
    pbPatchBuf = (BYTE *)GlobalAlloc(GPTR,detLen);
    memset(pbPatchBuf, 0x90, detLen);
    pbPatchBuf[0] = 0x33;
    pbPatchBuf[1] = 0xC0;
    pbPatchBuf[2] = 0x85;
    pbPatchBuf[3] = 0xC0;
    pbPatchBuf[4] = 0x0F;
    pbPatchBuf[5] = 0x84;
    *(DWORD*)&pbPatchBuf[6] = (DWORD)(pbFuncDetour - pbFuncOrig) - 10;
	int i;
    for(i = 0; i < detLen; i++)
          pbFuncOrig[i] = pbPatchBuf[i];
    GlobalFree(pbPatchBuf);
    VirtualProtect(lpFuncOrig, detLen,lpfnOldProtect,&lpNewProtect);
    return lpMallocPtr;
} 

DWORD cDetours::CreateJMP_EAX( DWORD  SrcVA, DWORD  DstVA, DWORD  Size )
{
	DWORD DetourVA, dwProtect, i;
#define SIZEOF_MOVEAX_JMPEAX 7
	if ( SrcVA && DstVA && Size >= SIZEOF_MOVEAX_JMPEAX )
	{
		DetourVA = (DWORD) VirtualAlloc( 
			NULL, Size + SIZEOF_MOVEAX_JMPEAX, 
			MEM_COMMIT, PAGE_EXECUTE_READWRITE );
		if ( DetourVA && VirtualProtect( (VOID*)SrcVA, Size, PAGE_EXECUTE_READWRITE, &dwProtect ) )
		{
			for ( i=0; i < Size; i++ ) {
				*(BYTE*)( DetourVA + i ) = *(BYTE*)( SrcVA + i );
			}
			*(BYTE*)( DetourVA + Size + 0 ) = 0xB8;
			*(DWORD*)( DetourVA + Size + 1 ) = ( SrcVA + Size );
			*(WORD*)( DetourVA + Size + 5 ) = 0xE0FF;
			*(BYTE*)( SrcVA + 0 ) = 0xB8;
			*(DWORD*)( SrcVA + 1 ) = ( DstVA );
			*(WORD*)( SrcVA + 5 ) = 0xE0FF;
			VirtualProtect( (VOID*)SrcVA, Size, dwProtect, &dwProtect );
			VirtualProtect( (VOID*)DetourVA, Size + 
				SIZEOF_MOVEAX_JMPEAX, PAGE_EXECUTE_READ, &dwProtect );
	    	return DetourVA;
		}
	}
	return (0);
}

void* cDetours::memcpy_s( void* pvAddress, const void* pvBuffer, size_t stLen )
{
	MEMORY_BASIC_INFORMATION mbi;
	VirtualQuery( ( void* )pvAddress, &mbi, sizeof( mbi ) );
	VirtualProtect( mbi.BaseAddress, mbi.RegionSize, PAGE_EXECUTE_READWRITE, &mbi.Protect );
	void* pvRetn = memcpy( ( void* )pvAddress, ( void* )pvBuffer, stLen );
	VirtualProtect( mbi.BaseAddress, mbi.RegionSize, mbi.Protect, &mbi.Protect );
	FlushInstructionCache( GetCurrentProcess( ), ( void* )pvAddress, stLen );
	return pvRetn;
}
DWORD cDetours::DetourCreateE9( DWORD  SrcVA, DWORD  DstVA, DWORD  Size )
{

 DWORD DetourVA, dwProtect, i;
JUNK_CODE
#define SIZEOF_JMP_REL  5
JUNK_CODE
 if ( SrcVA && DstVA && Size >= SIZEOF_JMP_REL )
 {
	 JUNK_CODE
  DetourVA = (DWORD) VirtualAlloc( 
   NULL, Size + SIZEOF_JMP_REL, 
   MEM_COMMIT, PAGE_EXECUTE_READWRITE );
	 JUNK_CODE

  if ( DetourVA && VirtualProtect( (VOID*)SrcVA, Size, PAGE_EXECUTE_READWRITE, &dwProtect ) )
  {
   for ( i=0; i < Size; i++ ) {
	   JUNK_CODE
    *(BYTE*)( DetourVA + i ) = *(BYTE*)( SrcVA + i );
	   JUNK_CODE
   }
   
   *(BYTE*)( DetourVA + Size + 0 ) = 0xE9;//0xE9 ALL
   JUNK_CODE
   *(DWORD*)( DetourVA + Size + 1 ) = ( SrcVA - DetourVA - SIZEOF_JMP_REL );
   JUNK_CODE

              *(BYTE*)( SrcVA + 0 ) = 0xE9;
   JUNK_CODE
   *(DWORD*)( SrcVA + 1 ) = ( DstVA - SrcVA - SIZEOF_JMP_REL );
   JUNK_CODE

   VirtualProtect( (VOID*)SrcVA, Size, dwProtect, &dwProtect );
   JUNK_CODE

   VirtualProtect( (VOID*)DetourVA, Size + 
   SIZEOF_JMP_REL, PAGE_EXECUTE_READ, &dwProtect );
   JUNK_CODE
	    
      return DetourVA;
  }
 }
 return (0);
}

DWORD cDetours::CreateJMP_REL( DWORD  SrcVA, DWORD  DstVA, DWORD  Size )
{
	DWORD DetourVA, dwProtect, i;

#define SIZEOF_JMP_REL  5

	if ( SrcVA && DstVA && Size >= SIZEOF_JMP_REL )
	{
		DetourVA = (DWORD) VirtualAlloc( 
			NULL, Size + SIZEOF_JMP_REL, 
			MEM_COMMIT, PAGE_EXECUTE_READWRITE );

		if ( DetourVA && VirtualProtect( (VOID*)SrcVA, Size, PAGE_EXECUTE_READWRITE, &dwProtect ) )
		{
			for ( i=0; i < Size; i++ ) {
				*(BYTE*)( DetourVA + i ) = *(BYTE*)( SrcVA + i );
			}
			
			*(BYTE*)( DetourVA + Size + 0 ) = 0xE9;
			*(DWORD*)( DetourVA + Size + 1 ) = ( SrcVA - DetourVA - SIZEOF_JMP_REL );

	    	        *(BYTE*)( SrcVA + 0 ) = 0xE9;
			*(DWORD*)( SrcVA + 1 ) = ( DstVA - SrcVA - SIZEOF_JMP_REL );

			VirtualProtect( (VOID*)SrcVA, Size, dwProtect, &dwProtect );

			VirtualProtect( (VOID*)DetourVA, Size + 
				SIZEOF_JMP_REL, PAGE_EXECUTE_READ, &dwProtect );

	    	return DetourVA;
		}
	}
	return (0);
}