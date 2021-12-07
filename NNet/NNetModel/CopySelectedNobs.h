// CopySelectedNobs.h
//
// Commands

#pragma once

#include <unordered_map>
#include "UPNobList.h"

class NNetModelWriterInterface;
class BaseKnot;
class Nob;

using std::unordered_map;

class CopySelectedNobs
{
public:

    static UPNobList Do(NNetModelWriterInterface &);

private:
    inline static unordered_map<Nob const *, Nob       *> m_mapModel2copy; // maps model to copy
    inline static unordered_map<Nob       *, Nob const *> m_mapCopy2model; // maps copy to model

    inline static UPNobList m_nobs2Add;
    inline static size_t    m_nrOfNobs;

    static Nob       * model2copy(Nob const * const);
    static Nob const & copy2model(Nob       * const); 

    static void add2copy(Nob const &, UPNob);
    static void addMissingKnot(BaseKnot const &);
};
