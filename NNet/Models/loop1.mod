# NNetModel
# Created 27-04-2020 21-57-32
# Computer name: DESKTOP-D6AR9AH
# User name: Peter

Protocol version 1.5

GlobalParameter PulseSpeed = 0.1
GlobalParameter PulseWidth = 2000
GlobalParameter SignalLoss = 0
GlobalParameter Threshold = 20
GlobalParameter PeakVoltage = 10
GlobalParameter RefractoryPeriod = 500
GlobalParameter TimeResolution = 100

NrOfShapes = 14

CreateShape 0 inputNeuron (561.303|216.951)
CreateShape 1 neuron (497|808)
CreateShape 2 neuron (800|400)
CreateShape 5 knot (500|400)
CreateShape 6 knot (496|538)
CreateShape 3 pipe (5->2)
CreateShape 4 pipe (6->1)
CreateShape 7 pipe (2->6)
CreateShape 8 pipe (6->5)
CreateShape 9 pipe (0->5)

ShapeParameter InputNeuron 0 PulseRate = 50

