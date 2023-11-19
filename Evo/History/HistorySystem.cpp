// HistorySystem.cpp
//

module;

#include <cassert>

module HistorySystem;

import HistorySystemImpl;

HistorySystem * HistorySystem::CreateHistorySystem()
{
	return new HistorySystemImpl();
}

size_t HistorySystem::GetSlotWrapperSize()
{ 
	return sizeof(HistCacheItem);  
}
