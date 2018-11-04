// dump.cpp
//

#include "stdafx.h"
#include <fstream>
#include <iomanip>
#include "grid_model.h"
#include "dump.h"

using namespace std;

wostream * DUMP::m_pDumpStream = & wcout;

wostream & operator << ( wostream & out, IndId const & id )
{
    out << id.GetLong( );
    return out;
};

wostream & operator << ( wostream & out, PlannedActivity const & plan )
{
//lint -e747  Significant prototype coercion with setw

   out << L" action=" << setw(8) << GetActionTypeName( plan.GetActionType( ) );

   if ( plan.GetTarget( ).IsNull( ) )
       out << setw( 15 ) << L" ";
   else
       out << L" target=" << setw( 3 ) << plan.GetTarget( );

   if ( plan.GetPartner( ).IsNull( ) )
       out << setw( 15 ) << L" ";
   else
       out << L" target=" << setw( 3 ) << plan.GetPartner( );

   return out;
//lint +e747 
}

void DUMP::SetDumpStream( wostream * const pDumpStream ) 
{
    m_pDumpStream = pDumpStream;
};

void DUMP::Flush( ) 
{
    (void)m_pDumpStream->flush( );
};

void DUMP::DumpNL( )
{
    *m_pDumpStream << endl;
}

void DUMP::Dump( wchar_t const * const str )
{
    *m_pDumpStream << str;
}

void DUMP::Dump( GridField const & gf )
{
    *m_pDumpStream << gf;
    *m_pDumpStream << endl;
}

void DUMP::Dump( Grid const & grid, GridPoint const gp )
{
    *m_pDumpStream << grid.GetGridField( gp );
    *m_pDumpStream << endl;
}
