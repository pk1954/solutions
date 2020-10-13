// NNetError.h 
//
// NNetModel

#pragma once

#include <exception>
#include "script.h"
#include "errhndl.h"
#include "NNetModelWriterInterface.h"
#include "NNetModel.h"

using std::to_wstring;
using std::wcout;
using std::endl;

struct ShapeException: public exception
{
};

class NNetErrorHandler : public ShapeErrorHandler
{
public:
    NNetErrorHandler( Script * const pScript, NNetModel const * const pModel )
        : m_pScript( pScript ),
          m_pModel( pModel )
    {}

    static void CheckShapeId
    ( 
        Script          & script, 
        NNetModel const & model,
        ShapeId   const   id 
    )
    {        
        Scanner & scanner    { script.GetScanner() };
        wstring   strShapeId { to_wstring( id.GetValue() ) };
        if ( IsUndefined( id ) )
        {
            ScriptErrorHandler::HandleSemanticError
            (
                scanner,
                wstring( L"Invalid shape id: " ) + strShapeId,
                L"ShapeId != NO_SHAPE" 
            );
        }
        else if ( ! model.GetShapes().IsValidShapeId( id ) )
        {
            ScriptErrorHandler::HandleSemanticError
            (
                scanner,
                wstring( L"Invalid shape id: " ) + strShapeId,
                L"id < " + to_wstring( model.GetSizeOfShapeList() )
            );
        }
        else if ( model.IsShapeNullPtr( id ) )
        {
            ScriptErrorHandler::HandleSemanticError
            (
                scanner,
                wstring( L"Shape is not defined: " ) + strShapeId,
                L"Defined ShapeId"
            );
        }
    };

    virtual void operator()( ShapeId const id ) 
    {
        CheckShapeId( * m_pScript, * m_pModel, id );
        throw ShapeException();
    }

private:
    Script          * const m_pScript;
    NNetModel const * const m_pModel;
};

inline bool ProcessNNetScript
( 
    Script                   * pScript,
    NNetModelWriterInterface * pModelInterface,
    wstring            const & wstrPath
) 
{
    bool bSuccess { false };
    if ( ! wstrPath.empty( ) )
    {
        NNetErrorHandler errHndl { pScript, & pModelInterface->GetModel() };
        pModelInterface->SetShapeErrorHandler( & errHndl );
        try
        {
            wcout << L"*** Processing script file " << wstrPath << endl;
            bSuccess = pScript->ScrProcess( wstrPath );
        }
        catch ( ShapeException e ) 
        { 
        }
        pModelInterface->SetShapeErrorHandler( nullptr );
    }
    return bSuccess;
}

