// ModuleInterface.h : 
//
// NNetModel

#pragma once

#include <vector>;
#include "MoreTypes.h"
#include "BaseKnot.h"

using std::vector;

using PUPBaseKnot = unique_ptr<BaseKnot> const *;

class ModuleInterface
{
public:
    ModuleInterface( vector<PUPBaseKnot> const & list )
    {
        assert( list.size() >= 2 );
        PUPBaseKnot pFirst;
        PUPBaseKnot pLast;
        m_list     .resize( list.size() - 2 );
        m_umPntRoot.resize( list.size() - 2 );
        findMostDistantPair( list, pFirst, pLast );
    }

private:
    vector <PUPBaseKnot>     m_list;
    vector <MicroMeterPoint> m_umPntRoot;

    bool findMostDistantPair
    ( 
        vector<PUPBaseKnot> const & list, 
        PUPBaseKnot               & pFirst, 
        PUPBaseKnot               & pLast 
    )
    {
        if ( list.size() < 2 )
            return false;
        float fMaxDistSquare { 0.0f };
        for ( auto p1 : list )
        for ( auto p2 : list )
        {
            float fDistSquare { DistSquare((*p1)->GetPosition(), (*p1)->GetPosition()) };
            if ( fDistSquare > fMaxDistSquare )
            {
                fMaxDistSquare = fDistSquare;
                pFirst = p1;
                pLast  = p2;
            }
        }
        return true;
    }
};