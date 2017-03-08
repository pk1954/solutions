// history.h : 
//

#pragma once

class History
{
public:
	virtual ~History( ) {};
	virtual void Add( ) = 0;
	virtual bool Forward( ) = 0;
	virtual bool Backward( ) = 0;
	virtual long GetGeneration( ) = 0;
	virtual long GetMemorySize() = 0;
	virtual void Reset() = 0;
	virtual void Discard() = 0;
#ifdef _DEBUG
	virtual void Dump() = 0;
#endif

	static History * GetInstance( char *, long, long );
};