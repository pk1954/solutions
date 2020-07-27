// NNetSimuConsole.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include "SCRIPT.H"
#include "UtilityWrappers.h"
#include "NNetModelWriterInterface.h"
#include "NNetWrappers.h"

int main()
{
    NNetModelWriterInterface m_modelWriterInterface { };

    DefineUtilityWrapperFunctions( );
    DefineNNetWrappers( & m_modelWriterInterface );

    std::cout << "Hello World!\n";
    Script::ProcessScript( L"Test1.in" );
}
