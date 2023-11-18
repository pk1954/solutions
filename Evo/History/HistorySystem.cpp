// HistorySystem.cpp
//


#include <cassert>
#include "HistorySystem.h"
#include "HistorySystemImpl.h"

HistorySystem * HistorySystem::CreateHistorySystem()
{
	return new HistorySystemImpl();
}

size_t HistorySystem::GetSlotWrapperSize()
{ 
	return sizeof(HistCacheItem);  
}
