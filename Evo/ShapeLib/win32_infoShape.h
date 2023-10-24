// win32_infoShape.cpp
//

module;

#include <iomanip>     
import Shape;

class EvolutionCore;

class InfoShape : public Shape
{
public:
	virtual void FillBuffer(EvolutionCore const * const, GridPoint const);
};
