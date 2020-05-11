#include "LTClient.h"

extern CLTClient* g_LTClient;
extern CCommonLT* g_CommonLT;

bool VisibleFilterFn(HOBJECT hObj, void* UserParam)
{
	uint32 ObjFlags;
	g_CommonLT->GetObjectFlags(hObj, OFT_Flags, ObjFlags);
	if (!(ObjFlags & FLAG_RAYHIT))
		return false;
	if (!(ObjFlags & FLAG_VISIBLE))
		return false;
	return true;
}

bool IsVisible(const LTVector& StartPos, const LTVector& EndPos)
{
	CAIntersectQuery IQuery;
	ZeroMemory(&IQuery, sizeof(CAIntersectQuery));
	IQuery.Start = StartPos;
	IQuery.End = EndPos;
//	IQuery.Flags = INTERSECT_OBJECTS | IGNORE_NONSOLID | INTERSECT_HPOLY;
	IQuery.FilterFn = VisibleFilterFn;
	CAIntersectInfo IInfo;
	if (g_LTClient->IntersectSegment(IQuery, &IInfo))
		return false;

	return true;
}