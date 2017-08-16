# TEST_1.SPC
#
# Testee: HistoryTest.exe
#

# Negative Specs

1="Error"

# Positive Specs

"Create 10 history slots"
"Iterate thru generations"
"Generation:  1 -  1 Slots:  0  1"
"Generation:  29 -  29 Slots:  0  13  21  23  24  25  26  27  28  29"
"Now backwards"
"HistorySystemImpl::ApproachHistGen 7"
"Generation:  19 -  14 Slots:  0  13  21  23  24  25  26  27  28  29"
"Generation:  0 -  0 Slots:  0  13  21  23  24  25  26  27  28  29"
"HistoryTest finished"

