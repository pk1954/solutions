// ModelData.h : 
//

#pragma once

class ModelData 
{
public:
    virtual ~ModelData( ) {};

	virtual void CopyModelData( ModelData const * const ) = 0;
	virtual void OnReset( )                               = 0;
	virtual void OnNextGeneration( )                      = 0;
    virtual void OnAppCommand( unsigned short, short )    = 0;
};

class ModelFactory
{
public:
	virtual ModelData * CreateModelData( ) const = 0;
};
