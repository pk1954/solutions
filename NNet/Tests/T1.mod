# NNetModel
# Created Sun Nov  8 15:12:58 2020

# Computer name: DESKTOP-D6AR9AH
# User name: Peter

Protocol version 1.6


Description ""

GlobalParameter PulseSpeed = 0.1
GlobalParameter PulseWidth = 2000
GlobalParameter SignalLoss = 0
GlobalParameter Threshold = 14
GlobalParameter PeakVoltage = 10
GlobalParameter RefractoryPeriod = 500
GlobalParameter TimeResolution = 100

NrOfShapes =  7

CreateShape 0 inputNeuron ( 400| 200)
CreateShape 1 neuron ( 400| 800)
CreateShape 3 neuron ( 594.19| 475.617)
CreateShape 4 knot ( 403.483| 505.082)
CreateShape 2 pipe(4->1)
CreateShape 5 pipe(3->4)
CreateShape 6 pipe(0->4)

ShapeParameter InputNeuron 0 PulseRate =  50


NrOfTracks 0

