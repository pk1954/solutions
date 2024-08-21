// GridPointList.ixx : 
//
// EvolutionCore

export module EvolutionCore:GridPointList;

import :GridPoint;
import :GridField;
import :GridModel;

#ifdef _DEBUG
#define CHECK_GRIDPOINT_LIST(grid) CheckGridPointList(grid)
#else 
#define CHECK_GRIDPOINT_LIST(grid)
#endif

export class GridPointList
{
public:
    GridPointList() 
        : m_gpOldest  (GP_NULL),
          m_gpYoungest(GP_NULL),
          m_iCount(0)
    { };

    int GetSize() const { return m_iCount; }; 

    GridPoint const GetOldestGp  () const { return m_gpOldest;   };
    GridPoint const GetYoungestGp() const { return m_gpYoungest; };

    bool ListIsEmpty() const { return m_gpOldest.IsNull(); }

    void SetOldest  (GridPoint const gp) { m_gpOldest   = gp; }; 
    void SetYoungest(GridPoint const gp) { m_gpYoungest = gp; }; 

    bool IsOldest  (GridPoint const gp) const { return m_gpOldest   == gp; }
    bool IsYoungest(GridPoint const gp) const { return m_gpYoungest == gp; }

    void ResetGpList()
    {
        m_gpOldest  .Set2Null();
        m_gpYoungest.Set2Null();
        m_iCount = 0;
    };

    void AddGridPointToList     (GridModel &, GridField &);
    void ReplaceGridPointInList (GridModel &, GridField &, GridField &);
    void DeleteGridPointFromList(GridModel &, GridField &);

    void CheckGridPointList(GridModel const &) const;

private:
    GridPoint m_gpOldest;
    GridPoint m_gpYoungest;
    int       m_iCount;  // number of elements in list
};
