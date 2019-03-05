// gridField.cpp :
//

#include "stdafx.h"
#include <iomanip>
#include <iomanip>
#include "gridField.h"

long         GridField::m_lFertilizerYield;
ENERGY_UNITS GridField::m_enMaxFertilizer;
ENERGY_UNITS GridField::m_enFoodReserve;
ENERGY_UNITS GridField::m_enMaxFood;

std::wostream & operator << ( std::wostream & out, GridField const & gf )
{
//lint -e747  Significant prototype coercion with setw
    out << gf.GetGridPoint( );
    out << L" en="           << std::setw(8) << gf.GetEnergy( );
//  out << L" last action: " << std::setw(9) << GetActionTypeName( gf.GetLastAction() );
//lint +e747 
    return out;
}
