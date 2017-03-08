// win32_askHistoryCut.h
//

#pragma once

#include <sstream> 
#include "DisplayFunctor.h"
#include "HistoryGeneration.h"
#include "HistorySystem.h"

class AskHistoryCut : public DisplayFunctor
{
public:
    AskHistoryCut( ) : 
        m_pHistSys( nullptr ) 
    {};

    void SetHistorySystem( HistorySystem const * pHistSys  )
    {
        m_pHistSys = pHistSys;
    }

    virtual bool operator() ( bool const bWait ) const
    {
        std::wostringstream wBuffer;
        HIST_GENERATION  genCurrent  = m_pHistSys->GetCurrentGeneration( );
        HIST_GENERATION  genYoungest = m_pHistSys->GetYoungestGeneration( );
        assert( genCurrent < genYoungest );
        wBuffer << L"Gen " << ( genCurrent.GetLong( ) + 1 ) << L" - " << genYoungest.GetLong( ) << L" will be deleted.";
        return IDOK == MessageBox( nullptr, L"Cut off history?", wBuffer.str( ).c_str( ), MB_OKCANCEL | MB_SYSTEMMODAL );
    }

private:
    HistorySystem const * m_pHistSys;
};
