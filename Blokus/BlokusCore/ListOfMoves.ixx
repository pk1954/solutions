// ListOfMoves.ixx
//
// BlokusCore

export module BlokusCore:ListOfMoves;

import std;
import :BlokusMove;

using std::vector;
using std::ranges::find_if;
using std::ranges::subrange;

export using MoveIter = vector<BlokusMove>::const_iterator;

export class ListOfMoves
{
public:

    void   Clear()       { m_list.clear(); }
    bool   Empty() const { return m_list.empty(); }
    size_t Size () const { return m_list.size(); }

    BlokusMove const &operator[] (size_t const index) const
	{ 
		return m_list.at(index); 
	}

    void Add(BlokusMove const move)
    {
        m_list.push_back(move);
    }

    void Apply2AllC(auto const& func) const
    {
        for (auto i : m_list)
            func(i);
    };

    MoveIter GetBegin(PieceTypeId const idPieceType) const
    { 
        return find_if(m_list, [idPieceType](BlokusMove const &move) { return move.GetPieceTypeId() == idPieceType; });
    }

    MoveIter GetEnd(PieceTypeId const idPieceType) const
    { 
        auto start = GetBegin(idPieceType);
        auto end   = getEnd(idPieceType, start);
        return end;
    }

    void GetNext
    (
        PieceTypeId const idPieceType,
        MoveIter         &actIter
    ) const
    { 
        auto start = GetBegin(idPieceType);
        auto end   = getEnd(idPieceType, start);
        if (++actIter == end)
            actIter = start;
    }

    subrange<MoveIter> GetMovesWithMaxNrOfCells() const 
    { 
        int      const iMaxSize { m_list.begin()->NrOfCells() };
        MoveIter const start = m_list.begin();
        MoveIter const end   = find_if
        (
            m_list, 
            [iMaxSize](BlokusMove const &move) 
            { 
                return move.NrOfCells() != iMaxSize; 
            }
        );
        return subrange(start, end); 
    }

    subrange<MoveIter> GetMoves(PieceTypeId const idPieceType) const 
    { 
        auto start = GetBegin(idPieceType);
        auto end   = getEnd(idPieceType, start);
        return subrange(start, end); 
    }

private:

    vector<BlokusMove> m_list;

    MoveIter getEnd
    (
        PieceTypeId const idPieceType,
        MoveIter    const start
    ) const
    { 
        return find_if_not(start, m_list.end(), [idPieceType](BlokusMove const &move) { return move.GetPieceTypeId() == idPieceType; });
    }

};