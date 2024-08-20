
module;

#include <excpt.h>

export module test;

export class test
{
	void X() 
	{
		__try{}
		__except (EXCEPTION_EXECUTE_HANDLER) {}
    }
};