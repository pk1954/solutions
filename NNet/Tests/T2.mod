# NNetModel
# Created Tue May 11 19:11:06 2021

# Computer name: DESKTOP-D6AR9AH
# User name: Peter

Protocol version 1.7



GlobalParameter PulseSpeed = 120
GlobalParameter PulseWidth = 2000
GlobalParameter SignalLoss = 0
GlobalParameter Threshold = 20
GlobalParameter PeakVoltage = 10
GlobalParameter RefractoryPeriod = 500
GlobalParameter TimeResolution = 100

NrOfNobs = 9

CreateNob 0 outputNeuron(297.404|304.528)
CreateNob 1 outputNeuron(454.632|349.185)
CreateNob 2 outputNeuron(630.077|348.905)
CreateNob 3 knot(331.518|83.376)
CreateNob 5 knot(551.346|36.0098)
CreateNob 7 knot(686.157|26.2937)
CreateNob 4 pipe(3->0)
CreateNob 6 pipe(5->1)
CreateNob 8 pipe(7->2)



NrOfTracks 0

