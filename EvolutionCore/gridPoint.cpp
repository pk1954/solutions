// gridPoint.cpp :
//

#include "stdafx.h"
#include <limits.h>
#include <iostream>
#include <iomanip>
#include "gridPoint.h"

std::wostream & operator << ( std::wostream & out, GridPoint const gp )
{
//lint -e747  Significant prototype coercion with setw
    out << L" " << std::setw(3) << gp.GetX() << L" " << std::setw(3) << gp.GetY() << L" ";
    return out;
}
