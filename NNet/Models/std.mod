# NNetModel
# Created Sat Jan 16 13:37:04 2021

# Computer name: DESKTOP-D6AR9AH
# User name: Peter

Protocol version 1.7

NrOfShapes = 3

GlobalParameter PulseSpeed = 120
GlobalParameter PulseWidth = 2000
GlobalParameter SignalLoss = 0
GlobalParameter Threshold = 14
GlobalParameter PeakVoltage = 10
GlobalParameter RefractoryPeriod = 500
GlobalParameter TimeResolution = 100

CreateShape 0 inputNeuron(400|200)
CreateShape 1 neuron(400|800)
CreateShape 2 pipe(0->1)

ShapeParameter InputNeuron 0 PulseRate = 50


NrOfTracks 0

