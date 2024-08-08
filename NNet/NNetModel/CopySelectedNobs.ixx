// CopySelectedNobs.ixx
//
// NNetModel

export module NNetModel:CopySelectedNobs;

import std;
import :NNetModelWriterInterface;
import :UPNobList;
import :PosNob;
import :Nob;

using std::unordered_map;
using std::unique_ptr;

export class CopySelectedNobs
{
public:

    static unique_ptr<UPNobList> Do(NNetModelWriterInterface &);

private:
    inline static unordered_map<Nob const *, Nob       *> m_mapModel2copy; // maps model to copy
    inline static unordered_map<Nob       *, Nob const *> m_mapCopy2model; // maps copy to model

    inline static size_t  m_nrOfNobs;

    static Nob       * model2copy(Nob   const * const);
    static Nob const & copy2model(Nob * const);

    static void add2copy(Nob const &);
};
