# TEST_1.SPC
#
# Testee: HistoryTest.exe
#

# Negative Specs

1="Error"

# Positive Specs

"Create 10 history slots"
"Iterate thru generations"
"HistoryCache::Save2CacheSlot 1"
"HistCacheItem::CopyCacheItem Gen 29 -> 18"
"Now backwards"
"HistorySystemImpl::ApproachHistGen 7"
"HistCacheItem::CopyCacheItem Gen 0 -> 1"
"Generation:  0 -  0 Slots:  0  13  21  23  24  25  26  27  28  29 "
"HistoryTest finished"

