#pragma once

template <typename T>
class SafePtr
{
	T* pT;
public:
	SafePtr()
	{pT = 0;}
	SafePtr(T* _pT)
	{pT = _pT;}
	~SafePtr()
	{delete pT;}

	T* Get()
	{return pT;}

	SafePtr<T>& operator =(T* _pT)
	{pT = _pT; return *this;}

	SafePtr<T>& operator =(SafePtr<T>& _pT)
	{pT = _pT.pT; _pT.pT = 0; return *this;}

	bool operator ==(T* _pT) const
	{return (pT == _pT);}

	bool operator !=(T* _pT) const
	{return (pT != _pT);}

	T* operator->()
	{return pT;}

	operator T*()
	{return pT;}
};