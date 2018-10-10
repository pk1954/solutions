// gridField.cpp :
//

#include "stdafx.h"
#include <iomanip>
#include <iomanip>
#include "gridField.h"

int   GridField::m_iFertilizerYield;
int   GridField::m_iMaxFertilizer;
short GridField::m_sFoodReserve;
short GridField::m_sMaxFood;

std::wostream & operator << ( std::wostream & out, GridField const & gf )
{
//lint -e747  Significant prototype coercion with setw
    out << gf.GetGridPoint( );
    out << L" en="           << setw(8) << gf.GetEnergy( );
//  out << L" last action: " << setw(9) << GetActionTypeName( gf.GetLastAction() );
//lint +e747 
    return out;
}
