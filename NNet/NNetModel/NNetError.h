// NNetError.h 
//
// NNetModel

#pragma once

#include <exception>
#include "script.h"
#include "errhndl.h"
#include "NNetModel.h"
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
    NNetErrorHandler( Script * pScript, NNetModel * const pModel )
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
        else if ( model.IsInvalidShapeId( id ) )
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
    Script    *       m_pScript;
    NNetModel * const m_pModel;
};

inline bool ProcessNNetScript
( 
    Script        * pScript,
    NNetModel     * pModel,
    wstring const & wstrPath
) 
{
    bool bSuccess { false };
    if ( ! wstrPath.empty( ) )
    {
        NNetErrorHandler errHndl { pScript, pModel };
        pModel->SetShapeErrorHandler( & errHndl );
        try
        {
            wcout << L"Processing script file " << wstrPath << endl;
            bSuccess = pScript->ScrProcess( wstrPath );
        }
        catch ( ShapeException e ) 
        { 
        }
        pModel->SetShapeErrorHandler( nullptr );
    }
    return bSuccess;
}

