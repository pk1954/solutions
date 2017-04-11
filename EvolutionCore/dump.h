// dump.h : 
//

#pragma once

#include <fstream>
#include <iostream>

class  Grid;
class  GridField;
// class  GenerationCmd;
// struct PlannedActivity;
class GridPoint;

class DUMP
{
public:
    static void SetDumpStream( std::wofstream * );

    static void DumpNL( );
    static void Dump( wchar_t const * );
    static void Dump( GridField const & );
//    static void Dump( PlannedActivity const & );
    static void Dump( Grid const &, GridPoint const & );
//    static void Dump( GenerationCmd const & );
    static void Flush( );

private:
    static std::wofstream * m_pDumpStream;
};


