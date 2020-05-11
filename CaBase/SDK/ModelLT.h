#pragma once

#pragma warning(disable:4996)

#include <windows.h>
#include "ltbasedefs.h"
#include "ltvector.h"
#include "ltbasetypes.h"

class CModelLT
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
	virtual void function8();				//0x0020
	virtual LTRESULT GetPiece(int* hObj, const char* pPieceName, HMODELPIECE* hPiece);	//0x0024
	virtual void function10();				//0x0028
	virtual LTRESULT SetPieceHiddenStatus(int* hObj, HMODELPIECE hPiece, bool bHidden);	//0x002C
	virtual void function12();				//0x0030
	virtual LTRESULT GetBoneNode(int* Obj, char* nodeName, unsigned int &Node );	//0x0034
	virtual void function14();				//0x0038
	virtual LTRESULT GetNodeTransform(/*HOBJECT*/int* hObj, HMODELNODE hNode, Transform* transform, bool bWorldSpace);	//0x003C
};