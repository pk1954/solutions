// GridPointList.cpp
//
// EvoCoreLib

module EvoCoreLib:GridPointList;

import :EvolutionDump;
import :GridPoint;
import :GridField;
import :GridModel;

void GridPointList::CheckGridPointList(GridModel const & grid) const
{
//    EvolutionDump::Dump(L" CheckGridPointList start");

    if (ListIsEmpty())
    {
        Assert(m_iCount == 0);
        Assert(GetYoungestGp().IsNull());
//        EvolutionDump::Dump(L" CheckGridPointList empty");
        return;
    }

    GridPoint gpRun  = GetOldestGp(); 
    int       iCount = 0;

    for (;;)
    {
        Assert(grid.IsAlive(gpRun));
        ++ iCount;
//       EvolutionDump::Dump(grid, gpRun);
//        EvolutionDump::Flush();
        if (IsYoungest(gpRun))
            break;
        else
        {
            GridPoint gpJunior = grid.GetJuniorGp(gpRun);
            Assert(IsInGrid(gpJunior));
            if (grid.GetSeniorGp(gpJunior) != gpRun)
            {
                EvolutionDump::Dump(L" CheckGridPointList detected inconsistency");
                Assert(false);
            }
            gpRun = gpJunior;
        }
    } 
   
    Assert(iCount == m_iCount);

//    EvolutionDump::Dump(L" CheckGridPointList end");
//	EvolutionDump::DumpNL();
}

// Add: New element is added to start of list

void GridPointList::AddGridPointToList(GridModel & grid, GridField & gf)
{
//  CHECK_GRIDPOINT_LIST(grid);
    
    GridPoint gp = gf.GetGridPoint();

    gf.SetSeniorGp(m_gpYoungest);
    gf.SetJuniorGp(GP_NULL);

    if (ListIsEmpty())
        SetOldest(gp);
    else
        grid.SetJuniorGp(m_gpYoungest, gp);

    SetYoungest(gp);

    ++m_iCount;

//  CHECK_GRIDPOINT_LIST(grid);
}

void GridPointList::ReplaceGridPointInList(GridModel & grid, GridField & gf, GridField & gfNew)
{
    GridPoint gpJunior = gf.GetJuniorGp();
    GridPoint gpSenior = gf.GetSeniorGp();
    GridPoint gp       = gf   .GetGridPoint();
    GridPoint gpNew    = gfNew.GetGridPoint();

    gfNew.SetJuniorGp(gpJunior);
    gfNew.SetSeniorGp(gpSenior);

    if (IsYoungest(gp))
        SetYoungest(gpNew);
    else
        grid.SetSeniorGp(gpJunior, gpNew );
              
    if (IsOldest(gp))
        SetOldest(gpNew);
    else
        grid.SetJuniorGp(gpSenior, gpNew );
              
    gf.CutConnections();
//	CHECK_GRIDPOINT_LIST(grid);
}

// Delete: Element is deleted from list

void GridPointList::DeleteGridPointFromList(GridModel & grid, GridField & gf)
{
//	CHECK_GRIDPOINT_LIST(grid);
	GridPoint gpJunior = gf.GetJuniorGp();
    GridPoint gpSenior = gf.GetSeniorGp();
    GridPoint gp       = gf.GetGridPoint();
   
    if (IsYoungest(gp)) // last one in list
        SetYoungest(gpSenior);
    else
        grid.SetSeniorGp(gpJunior, gpSenior);

    if (IsOldest(gp)) // first one in list
        SetOldest(gpJunior);
    else
        grid.SetJuniorGp(gpSenior, gpJunior);

    gf.CutConnections();

    --m_iCount;
//	CHECK_GRIDPOINT_LIST(grid);
}
