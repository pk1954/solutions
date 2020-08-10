# NNetModel
# Created 10-08-2020 12-47-44
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

NrOfShapes = 10

CreateShape 0 inputNeuron (482.49|174.149)
CreateShape 1 neuron (516.992|808.573)
CreateShape 2 neuron (790.777|362.254)
CreateShape 5 knot (499.819|403.928)
CreateShape 6 knot (530.478|575.405)
CreateShape 3 pipe (5->2)
CreateShape 4 pipe (6->1)
CreateShape 7 pipe (2->6)
CreateShape 8 pipe (6->5)
CreateShape 9 pipe (0->5)

ShapeParameter InputNeuron 0 PulseRate = 50

