// ModelData.h : 
//

#pragma once

class ModelData 
{
public:
    virtual ~ModelData( ) {};

	// call back functions 

	virtual void CopyModelData( ModelData const * const )     = 0;   
    virtual void OnAppCommand( int16_t const, int16_t const ) = 0;
};

class ModelFactory
{
public:
	virtual ModelData * CreateModelData( ) const = 0;
};
