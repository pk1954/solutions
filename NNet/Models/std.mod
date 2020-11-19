# NNetModel
# Created Wed Oct 28 19:55:14 2020

# Computer name: DESKTOP-D6AR9AH
# User name: Peter

Protocol version 1.6


Description ""
Description ""

GlobalParameter PulseSpeed = 0.1
GlobalParameter PulseWidth = 2000
GlobalParameter SignalLoss = 0
GlobalParameter Threshold = 14
GlobalParameter PeakVoltage = 10
GlobalParameter RefractoryPeriod = 500
GlobalParameter TimeResolution = 100

NrOfShapes =  11

CreateShape 0 inputNeuron ( 400| 200)
CreateShape 1 neuron ( 400| 800)
CreateShape 3 knot ( 535.05| 450.192)
CreateShape 4 knot ( 402.767| 340.43)
CreateShape 7 knot ( 119.6| 525.288)
CreateShape 2 pipe (7-> 1)
CreateShape 5 pipe (4-> 3)
CreateShape 6 pipe (0-> 4)
CreateShape 8 pipe (3-> 7)
CreateShape 9 pipe (4-> 7)
CreateShape 10 pipe (3-> 1)

ShapeParameter InputNeuron 0 PulseRate =  50


NrOfTracks 0

